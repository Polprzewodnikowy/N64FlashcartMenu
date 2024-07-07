#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include <libdragon.h>

#include "actions.h"
#include "boot/boot.h"
#include "flashcart/flashcart.h"
#include "fonts.h"
#include "hdmi.h"
#include "menu_state.h"
#include "menu.h"
#include "mp3_player.h"
#include "png_decoder.h"
#include "settings.h"
#include "sound.h"
#include "usb_comm.h"
#include "utils/fs.h"
#include "views/views.h"


#define MENU_DIRECTORY          "/menu"
#define MENU_SETTINGS_FILE      "config.ini"
#define MENU_CUSTOM_FONT_FILE   "custom.font64"

#define MENU_CACHE_DIRECTORY    "cache"
#define BACKGROUND_CACHE_FILE   "background.data"

#define FRAMERATE_DIVIDER       (2)
#define LAG_REPORT              (false)


static menu_t *menu;
static tv_type_t tv_type;
static volatile int frame_counter = 0;

extern tv_type_t __boot_tvtype;


static void frame_counter_handler (void) {
    frame_counter += 1;
}

static void frame_counter_reset (void) {
#if LAG_REPORT
    static int accumulated = 0;
    if (frame_counter > FRAMERATE_DIVIDER) {
        accumulated += frame_counter - FRAMERATE_DIVIDER;
        debugf(
            "LAG: %d additional frame(s) displayed since last draw (accumulated: %d)\n",
            frame_counter - FRAMERATE_DIVIDER,
            accumulated
        );
    }
#endif
    frame_counter = 0;
}

static void menu_init (boot_params_t *boot_params) {
    joypad_init();
    timer_init();
    rtc_init();
    rspq_init();
    rdpq_init();
    dfs_init(DFS_DEFAULT_LOCATION);

    sound_init_default();

    JOYPAD_PORT_FOREACH (port) {
        joypad_set_rumble_active(port, false);
    }

    menu = calloc(1, sizeof(menu_t));
    assert(menu != NULL);

    menu->mode = MENU_MODE_NONE;
    menu->next_mode = MENU_MODE_STARTUP;

    menu->flashcart_err = flashcart_init(&menu->storage_prefix);
    if (menu->flashcart_err != FLASHCART_OK) {
        menu->next_mode = MENU_MODE_FAULT;
    }

    path_t *path = path_init(menu->storage_prefix, MENU_DIRECTORY);

    directory_create(path_get(path));

    path_push(path, MENU_SETTINGS_FILE);
    settings_init(path_get(path));
    settings_load(&menu->settings);
    path_pop(path);

    path_push(path, MENU_CUSTOM_FONT_FILE);
    fonts_init(path_get(path));
    path_pop(path);

    path_push(path, MENU_CACHE_DIRECTORY);
    directory_create(path_get(path));

    path_push(path, BACKGROUND_CACHE_FILE);
    component_background_init(path_get(path));

    path_free(path);

    menu->boot_params = boot_params;

    menu->browser.directory = path_init(menu->storage_prefix, menu->settings.default_directory);
    if (!directory_exists(path_get(menu->browser.directory))) {
        path_free(menu->browser.directory);
        menu->browser.directory = path_init(menu->storage_prefix, "/");
    }

    hdmi_clear_game_id();

    tv_type = get_tv_type();
    if ((tv_type == TV_PAL) && menu->settings.pal60_enabled) {
        // HACK: Set TV type to NTSC, so PAL console would output 60 Hz signal instead.
        __boot_tvtype = TV_NTSC;
    }

    if (menu->settings.sound_enabled) {
        sound_init_sfx();
    }

    display_init(RESOLUTION_640x480, DEPTH_16_BPP, 2, GAMMA_NONE, FILTERS_DISABLED);

    register_VI_handler(frame_counter_handler);
}

static void menu_deinit (menu_t *menu) {
    unregister_VI_handler(frame_counter_handler);

    // NOTE: Restore previous TV type so boot procedure wouldn't passthrough wrong value.
    __boot_tvtype = tv_type;

    hdmi_send_game_id(menu->boot_params);

    path_free(menu->load.disk_path);
    path_free(menu->load.rom_path);
    for (int i = 0; i < menu->browser.entries; i++) {
        free(menu->browser.list[i].name);
    }
    free(menu->browser.list);
    path_free(menu->browser.directory);
    free(menu);

    component_background_free();

    flashcart_deinit();

    sound_deinit();

    rdpq_close();
    rspq_close();
    rtc_close();
    timer_close();
    joypad_close();

    display_close();
}

typedef const struct {
    menu_mode_t id;
    void (*init) (menu_t *menu);
    void (*show) (menu_t *menu, surface_t *display);
} view_t;

static view_t menu_views[] = {
    { MENU_MODE_STARTUP, view_startup_init, view_startup_display },
    { MENU_MODE_BROWSER, view_browser_init, view_browser_display },
    { MENU_MODE_FILE_INFO, view_file_info_init, view_file_info_display },
    { MENU_MODE_SYSTEM_INFO, view_system_info_init, view_system_info_display },
    { MENU_MODE_IMAGE_VIEWER, view_image_viewer_init, view_image_viewer_display },
    { MENU_MODE_TEXT_VIEWER, view_text_viewer_init, view_text_viewer_display },
    { MENU_MODE_MUSIC_PLAYER, view_music_player_init, view_music_player_display },
    { MENU_MODE_CREDITS, view_credits_init, view_credits_display },
    { MENU_MODE_SETTINGS_EDITOR, view_settings_init, view_settings_display },
    { MENU_MODE_RTC, view_rtc_init, view_rtc_display },
    { MENU_MODE_JOYPAD_CPAK, view_joypad_controller_pak_init, view_joypad_controller_pak_display },
    { MENU_MODE_FLASHCART, view_flashcart_info_init, view_flashcart_info_display },
    { MENU_MODE_LOAD_ROM, view_load_rom_init, view_load_rom_display },
    { MENU_MODE_LOAD_DISK, view_load_disk_init, view_load_disk_display },
    { MENU_MODE_LOAD_EMULATOR, view_load_emulator_init, view_load_emulator_display },
    { MENU_MODE_ERROR, view_error_init, view_error_display },
    { MENU_MODE_FAULT, view_fault_init, view_fault_display },
};

static view_t *menu_get_view (menu_mode_t id) {
    for (int i = 0; i < sizeof(menu_views) / sizeof(view_t); i++) {
        if (menu_views[i].id == id) {
            return &menu_views[i];
        }
    }
    return NULL;
}


void menu_run (boot_params_t *boot_params) {
    menu_init(boot_params);

    while (true) {
        surface_t *display = (frame_counter >= FRAMERATE_DIVIDER) ? display_try_get() : NULL;

        if (display != NULL) {
            frame_counter_reset();

            actions_update(menu);

            view_t *view = menu_get_view(menu->mode);
            if (view && view->show) {
                view->show(menu, display);
            } else {
                rdpq_attach_clear(display, NULL);
                rdpq_detach_wait();
                display_show(display);
            }

            if (menu->mode == MENU_MODE_BOOT) {
                break;
            }

            while (menu->mode != menu->next_mode) {
                menu->mode = menu->next_mode;

                view_t *next_view = menu_get_view(menu->next_mode);
                if (next_view && next_view->init) {
                    next_view->init(menu);
                }
            }

            time(&menu->current_time);
        }

        sound_poll();

        png_decoder_poll();

        usb_comm_poll(menu);
    }

    menu_deinit(menu);

    while (exception_reset_time() > 0) {
        // Do nothing if reset button was pressed
    }
}
