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


#define TV_TYPE_RAM         *((uint32_t *) (0x80000300))

#define CACHE_DIRECTORY     "sd:/menu/cache"
#define BACKGROUND_CACHE    "sd:/menu/cache/background.data"

#define FRAMERATE_DIVIDER   (2)
#define LAG_REPORT          (false)


static menu_t *menu;
static tv_type_t tv_type;
static volatile int frame_counter = 0;


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

    sound_init_default();

    menu = calloc(1, sizeof(menu_t));
    assert(menu != NULL);

    menu->mode = MENU_MODE_NONE;
    menu->next_mode = MENU_MODE_STARTUP;

    menu->flashcart_err = flashcart_init();
    if (menu->flashcart_err != FLASHCART_OK) {
        menu->next_mode = MENU_MODE_FAULT;
    }

    menu->error_message = NULL;

    settings_load(&menu->settings);

    directory_create(CACHE_DIRECTORY);

    component_background_init(BACKGROUND_CACHE);

    menu->boot_params = boot_params;

    bool default_directory_exists = directory_exists(menu->settings.default_directory);
    char *init_directory = default_directory_exists ? menu->settings.default_directory : "";

    menu->browser.valid = false;
    menu->browser.directory = path_init("sd:/", init_directory);

    menu->load.rom_path = NULL;

    hdmi_clear_game_id();

    tv_type = get_tv_type();
    if ((tv_type == TV_PAL) && menu->settings.pal60_enabled) {
        // HACK: Set TV type to NTSC, so PAL console would output 60 Hz signal instead.
        TV_TYPE_RAM = TV_NTSC;
    }

    display_init(RESOLUTION_640x480, DEPTH_16_BPP, 2, GAMMA_NONE, FILTERS_DISABLED);

    register_VI_handler(frame_counter_handler);
    
    fonts_init();
}

static void menu_deinit (menu_t *menu) {
    unregister_VI_handler(frame_counter_handler);

    // NOTE: Restore previous TV type so boot procedure wouldn't passthrough wrong value.
    TV_TYPE_RAM = tv_type;

    hdmi_send_game_id(menu->boot_params);

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


// NOTE: Keep this array in sync with menu_mode_t
static struct views_s {
    void (*init) (menu_t *menu);
    void (*show) (menu_t *menu, surface_t *display);
} views[__MENU_MODE_COUNT] = {
    { NULL, NULL }, // MENU_MODE_NONE
    { view_startup_init, view_startup_display }, // MENU_MODE_STARTUP
    { view_browser_init, view_browser_display }, // MENU_MODE_BROWSER
    { view_file_info_init, view_file_info_display }, // MENU_MODE_FILE_INFO
    { view_system_info_init, view_system_info_display }, // MENU_MODE_SYSTEM_INFO
    { view_image_viewer_init, view_image_viewer_display }, // MENU_MODE_IMAGE_VIEWER
    { view_music_player_init, view_music_player_display }, // MENU_MODE_MUSIC_PLAYER
    { view_credits_init, view_credits_display }, // MENU_MODE_CREDITS
    { view_load_rom_init, view_load_rom_display }, // MENU_MODE_LOAD_ROM
    { view_load_disk_init, view_load_disk_display }, // MENU_MODE_LOAD_DISK
    { view_load_emulator_init, view_load_emulator_display }, // MENU_MODE_LOAD_EMULATOR
    { view_error_init, view_error_display }, // MENU_MODE_ERROR
    { view_fault_init, view_fault_display }, // MENU_MODE_FAULT
    { NULL, NULL }, // MENU_MODE_BOOT
};

void menu_run (boot_params_t *boot_params) {
    menu_init(boot_params);

    while (exception_reset_time() < RESET_TIME_LENGTH) {
        surface_t *display = (frame_counter >= FRAMERATE_DIVIDER) ? display_try_get() : NULL;

        if (display != NULL) {
            frame_counter_reset();

            actions_update(menu);

            if (views[menu->mode].show) {
                views[menu->mode].show(menu, display);
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

                if (views[menu->mode].init) {
                    views[menu->mode].init(menu);
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
