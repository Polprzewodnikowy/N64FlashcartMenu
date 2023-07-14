#include <stdlib.h>

#include <libdragon.h>

#include "boot/boot.h"
#include "actions.h"
#include "assets.h"
#include "flashcart/flashcart.h"
#include "menu_state.h"
#include "menu.h"
#include "mp3player.h"
#include "settings.h"
#include "utils/fs.h"
#include "views/views.h"


#define TV_TYPE_RAM     *((uint32_t *) (0x80000300))


static menu_t *menu;
static bool boot_pending;
static tv_type_t tv_type;


static void menu_init (boot_params_t *boot_params) {
    controller_init();
    timer_init();
    rtc_init();
    audio_init(44100, 2);
    mixer_init(2);
    rspq_init();
    rdpq_init();

    assets_init();
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

    settings_load(&menu->settings);

    menu->boot_params = boot_params;

    bool default_directory_exists = directory_exists(menu->settings.default_directory);

    menu->browser.valid = false;
    menu->browser.directory = path_init(default_directory_exists ? menu->settings.default_directory : NULL);

    tv_type = get_tv_type();
    if ((tv_type == TV_PAL) && menu->settings.pal60) {
        // HACK: Set TV type to NTSC, so PAL console would output 60 Hz signal instead.
        TV_TYPE_RAM = TV_NTSC;
    }

    display_init(RESOLUTION_640x480, DEPTH_16_BPP, 2, GAMMA_NONE, ANTIALIAS_OFF);
}

static void menu_deinit (menu_t *menu) {
    // NOTE: Restore previous TV type so boot procedure wouldn't passthrough wrong value.
    TV_TYPE_RAM = tv_type;

    flashcart_deinit();

    path_free(menu->browser.directory);
    free(menu);

    rdpq_close();
    rspq_close();
    mixer_close();
    audio_close();
    rtc_close();
    timer_close();
    display_close();
}


void menu_run (boot_params_t *boot_params) {
    menu_init(boot_params);

    int audio_buffer_length = audio_get_buffer_length();

    while (!boot_pending && (exception_reset_time() == 0)) {
        surface_t *display = display_try_get();

        if (display != NULL) {
            actions_update(menu);

            switch (menu->mode) {
                case MENU_MODE_STARTUP:
                    view_startup_display(menu, display);
                    break;

                case MENU_MODE_BROWSER:
                    view_browser_display(menu, display);
                    break;

                case MENU_MODE_FILE_INFO:
                    view_file_info_display(menu, display);
                    break;

                case MENU_MODE_SYSTEM_INFO:
                    view_system_info_display(menu, display);
                    break;

                case MENU_MODE_MUSIC_PLAYER:
                    view_music_player_display(menu, display);
                    break;

                case MENU_MODE_CREDITS:
                    view_credits_display(menu, display);
                    break;

                case MENU_MODE_LOAD:
                    view_load_display(menu, display);
                    break;

                case MENU_MODE_EMULATOR_LOAD:
                    view_load_emulator_display(menu, display);
                    break;

                case MENU_MODE_ERROR:
                    view_error_display(menu, display);
                    break;

                case MENU_MODE_FAULT:
                    view_fault_display(menu, display);
                    break;

                default:
                    rdpq_attach_clear(display, NULL);
                    rdpq_detach_show();
                    break;
            }

            while (menu->mode != menu->next_mode) {
                menu->mode = menu->next_mode;

                switch (menu->next_mode) {
                    case MENU_MODE_STARTUP:
                        view_startup_init(menu);
                        break;

                    case MENU_MODE_BROWSER:
                        view_browser_init(menu);
                        break;

                    case MENU_MODE_FILE_INFO:
                        view_file_info_init(menu);
                        break;

                    case MENU_MODE_SYSTEM_INFO:
                        view_system_info_init(menu);
                        break;

                    case MENU_MODE_MUSIC_PLAYER:
                        view_music_player_init(menu);
                        break;

                    case MENU_MODE_CREDITS:
                        view_credits_init(menu);
                        break;

                    case MENU_MODE_LOAD:
                        view_load_init(menu);
                        break;

                    case MENU_MODE_EMULATOR_LOAD:
                        view_load_emulator_init(menu);
                        break;

                    case MENU_MODE_ERROR:
                        view_error_init(menu);
                        break;

                    case MENU_MODE_FAULT:
                        view_fault_init(menu);
                        break;

                    case MENU_MODE_BOOT:
                        boot_pending = true;
                        break;

                    default:
                        break;
                }
            }
        }

        if (audio_can_write()) {
            short *audio_buffer = audio_write_begin();
            mixer_poll(audio_buffer, audio_buffer_length);
            audio_write_end();
        }
    }

    menu_deinit(menu);

    while (exception_reset_time() > 0) {
        // Do nothing if reset button was pressed
    }
}
