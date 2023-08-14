#include <stdlib.h>
#include <time.h>

#include <libdragon.h>

#include "actions.h"
#include "boot/boot.h"
#include "flashcart/flashcart.h"
#include "fonts.h"
#include "menu_state.h"
#include "menu.h"
#include "mp3_player.h"
#include "png_decoder.h"
#include "settings.h"
#include "utils/fs.h"
#include "views/views.h"


#define TV_TYPE_RAM         *((uint32_t *) (0x80000300))

#define CACHE_DIRECTORY     "sd:/menu/cache"
#define BACKGROUND_CACHE    "sd:/menu/cache/background.data"


static menu_t *menu;
static bool boot_pending;
static tv_type_t tv_type;
static volatile int frame_counter = 0;


static void frame_counter_handler (void) {
    frame_counter += 1;
}

static void frame_counter_reset (void) {
    frame_counter = 0;
}

static void menu_init (boot_params_t *boot_params) {
    controller_init();
    timer_init();
    rtc_init();
    audio_init(44100, 3);
    mixer_init(2);
    rspq_init();
    rdpq_init();

    fonts_init();
    mp3player_mixer_init();

    boot_pending = false;

    menu = calloc(1, sizeof(menu_t));
    assert(menu != NULL);

    menu->mode = MENU_MODE_NONE;
    menu->next_mode = MENU_MODE_STARTUP;

    menu->flashcart_error = flashcart_init();
    if (menu->flashcart_error != FLASHCART_OK) {
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

    tv_type = get_tv_type();
    if ((tv_type == TV_PAL) && menu->settings.pal60) {
        // HACK: Set TV type to NTSC, so PAL console would output 60 Hz signal instead.
        TV_TYPE_RAM = TV_NTSC;
    }

    display_init(RESOLUTION_640x480, DEPTH_16_BPP, 2, GAMMA_NONE, ANTIALIAS_OFF);

    register_VI_handler(frame_counter_handler);
}

static void menu_deinit (menu_t *menu) {
    // NOTE: Restore previous TV type so boot procedure wouldn't passthrough wrong value.
    TV_TYPE_RAM = tv_type;

    path_free(menu->browser.directory);
    free(menu);

    component_background_free();

    flashcart_deinit();

    rdpq_close();
    rspq_close();
    mixer_close();
    audio_close();
    rtc_close();
    timer_close();

    unregister_VI_handler(frame_counter_handler);

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
    { view_load_init, view_load_display }, // MENU_MODE_LOAD
    { view_load_emulator_init, view_load_emulator_display }, // MENU_MODE_EMULATOR_LOAD
    { view_error_init, view_error_display }, // MENU_MODE_ERROR
    { view_fault_init, view_fault_display }, // MENU_MODE_FAULT
    { NULL, NULL }, // MENU_MODE_BOOT
};

void menu_run (boot_params_t *boot_params) {
    menu_init(boot_params);

    int audio_buffer_length = audio_get_buffer_length();

    while (!boot_pending && (exception_reset_time() < RESET_TIME_LENGTH)) {
        surface_t *display = (frame_counter >= 2) ? display_try_get() : NULL;

        if (display != NULL) {
            frame_counter_reset();

            actions_update(menu);

            if (views[menu->mode].show) {
                views[menu->mode].show(menu, display);
            } else {
                rdpq_attach_clear(display, NULL);
                rdpq_detach_show();
            }

            while (menu->mode != menu->next_mode) {
                menu->mode = menu->next_mode;

                if (views[menu->mode].init) {
                    views[menu->mode].init(menu);
                }

                if (menu->mode == MENU_MODE_BOOT) {
                    boot_pending = true;
                }
            }

            time(&menu->current_time);
        }

        while (audio_can_write()) {
            short *audio_buffer = audio_write_begin();
            mixer_poll(audio_buffer, audio_buffer_length);
            audio_write_end();
        }

        png_decoder_poll();
    }

    menu_deinit(menu);

    while (exception_reset_time() > 0) {
        // Do nothing if reset button was pressed
    }
}
