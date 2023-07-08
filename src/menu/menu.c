#include <stdlib.h>

#include <libdragon.h>

#include "actions.h"
#include "menu_state.h"
#include "menu.h"
#include "settings.h"
#include "utils/fs.h"
#include "views/views.h"


static menu_t *menu;
static bool boot_pending;


static void menu_init (settings_t *settings) {
    controller_init();
    audio_init(44100, 2);
    mixer_init(2);
    display_init(RESOLUTION_640x480, DEPTH_16_BPP, 2, GAMMA_NONE, ANTIALIAS_OFF);
    rspq_init();
    rdpq_init();

    boot_pending = false;

    menu = calloc(1, sizeof(menu_t));
    assert(menu != NULL);

    menu->mode = MENU_MODE_NONE;
    menu->next_mode = MENU_MODE_STARTUP;

    menu->assets.font = rdpq_font_load("assets:/font");
    menu->assets.font_height = 16;

    menu->browser.valid = false;
    if (file_exists(settings->last_state.directory)) {
        menu->browser.directory = path_init(settings->last_state.directory);
    } else {
        menu->browser.directory = path_init(NULL);
    }
    menu->browser.show_hidden = false;
}

static void menu_deinit (menu_t *menu) {
    path_free(menu->browser.directory);
    // NOTE: font is not loaded dynamically due to hack in assets.c, so there's no need to free it
    // rdpq_font_free(menu->assets.font);
    free(menu);

    rdpq_close();
    rspq_close();
    display_close();
    mixer_close();
    audio_close();
}


void menu_run (settings_t *settings) {
    menu_init(settings);

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

                case MENU_MODE_PLAYER:
                    view_player_display(menu, display);
                    break;

                case MENU_MODE_CREDITS:
                    view_credits_display(menu, display);
                    break;

                case MENU_MODE_LOAD:
                    view_load_display(menu, display);
                    break;

                case MENU_MODE_ERROR:
                    view_error_display(menu, display);
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

                    case MENU_MODE_PLAYER:
                        view_player_init(menu);
                        break;

                    case MENU_MODE_CREDITS:
                        view_credits_init(menu);
                        break;

                    case MENU_MODE_LOAD:
                        view_load_init(menu);
                        break;

                    case MENU_MODE_ERROR:
                        view_error_init(menu);
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
