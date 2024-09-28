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
#include "cart_load.h"


#define MENU_DIRECTORY          "/menu"
#define MENU_SETTINGS_FILE      "config.ini"
#define MENU_CUSTOM_FONT_FILE   "custom.font64"

#define MENU_CACHE_DIRECTORY    "cache"
#define BACKGROUND_CACHE_FILE   "background.data"

#define INTERLACED              (true)
#define FPS_LIMIT               (30.0f)


static menu_t *menu;


static void menu_init (boot_params_t *boot_params) {    
    menu = calloc(1, sizeof(menu_t));
    assert(menu != NULL);

    menu->boot_params = boot_params;

    menu->mode = MENU_MODE_NONE;
    menu->next_mode = MENU_MODE_STARTUP;

    menu->flashcart_err = flashcart_init(&menu->storage_prefix);
    if (menu->flashcart_err != FLASHCART_OK) {
        menu->next_mode = MENU_MODE_FAULT;
    }

    joypad_init();
    timer_init();
    rtc_init();
    rspq_init();
    rdpq_init();
    dfs_init(DFS_DEFAULT_LOCATION);

    actions_init();
    sound_init_default();
    sound_init_sfx();

    hdmi_clear_game_id();

    path_t *path = path_init(menu->storage_prefix, MENU_DIRECTORY);

    directory_create(path_get(path));

    path_push(path, MENU_SETTINGS_FILE);
    settings_init(path_get(path));
    settings_load(&menu->settings);
    path_pop(path);

    resolution_t resolution = {
        .width = 640,
        .height = 480,
        .interlaced = INTERLACED ? INTERLACE_HALF : INTERLACE_OFF,
        .pal60 = menu->settings.pal60_enabled,
    };
    display_init(resolution, DEPTH_16_BPP, 2, GAMMA_NONE, INTERLACED ? FILTERS_DISABLED : FILTERS_RESAMPLE);
    display_set_fps_limit(FPS_LIMIT);

    path_push(path, MENU_CUSTOM_FONT_FILE);
    fonts_init(path_get(path));
    path_pop(path);

    path_push(path, MENU_CACHE_DIRECTORY);
    directory_create(path_get(path));

    path_push(path, BACKGROUND_CACHE_FILE);
    component_background_init(path_get(path));

    path_free(path);

    sound_use_sfx(menu->settings.sound_enabled);

    menu->browser.directory = path_init(menu->storage_prefix, menu->settings.default_directory);
    if (!directory_exists(path_get(menu->browser.directory))) {
        path_free(menu->browser.directory);
        menu->browser.directory = path_init(menu->storage_prefix, "/");
    }
}

static void menu_deinit (menu_t *menu) {
    hdmi_send_game_id(menu->boot_params);

    component_background_free();

    path_free(menu->load.disk_path);
    path_free(menu->load.rom_path);
    for (int i = 0; i < menu->browser.entries; i++) {
        free(menu->browser.list[i].name);
    }
    free(menu->browser.list);
    path_free(menu->browser.directory);
    free(menu);

    display_close();

    sound_deinit();

    rdpq_close();
    rspq_close();
    rtc_close();
    timer_close();
    joypad_close();

    flashcart_deinit();
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

static void draw_progress (float progress) {
    surface_t *d = (progress >= 1.0f) ? display_get() : display_try_get();

    if (d) {
        rdpq_attach(d, NULL);

        component_background_draw();

        component_loader_draw(progress);

        rdpq_detach_show();
    }
}

void menu_run (boot_params_t *boot_params) {
    menu_init(boot_params);

    // FIXME: rather than use a controller button, would it be better to use the cart button?
    JOYPAD_PORT_FOREACH (port) { // FIXME: is this the best code location
        for (int i = 0; i < 50; i++) { // something like this is needed to poll enough.
            joypad_poll();
        }
        joypad_buttons_t b_held = joypad_get_buttons_held(port);

        if (menu->settings.rom_autoload_enabled && b_held.start) {
            menu->settings.rom_autoload_enabled = false;
            menu->settings.rom_autoload_path = "";
            settings_save(&menu->settings);
        } else if (menu->settings.rom_autoload_enabled) {
            // FIXME: currently errors with Corrupted rompak TOC: entry size too big (0xe6320000)
            // menu->load.rom_path = path_init("", menu->settings.rom_autoload_path);
            // menu->next_mode = MENU_MODE_BOOT;
            // menu->boot_params->device_type = BOOT_DEVICE_TYPE_ROM;
            // menu->boot_params->tv_type = BOOT_TV_TYPE_PASSTHROUGH;
            // menu->boot_params->detect_cic_seed = true;
            // menu->boot_params->cheat_list = NULL;

            // cart_load_err_t err = cart_load_n64_rom_and_save(menu, draw_progress);
            // if (err != CART_LOAD_OK) {
            //     menu_show_error(menu, cart_load_convert_error_message(err));
            //     return;
            // }

            // path_free(menu->load.rom_path);

            // menu_deinit(menu);
            // return;
        }
    }

    while (true) {
        surface_t *display = display_try_get();

        if (display != NULL) {
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
