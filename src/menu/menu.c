#include <stdlib.h>

#include <libdragon.h>

#include "actions.h"
#include "menu.h"
#include "views.h"


static menu_t *menu_init (settings_t *settings) {
    menu_t *menu = calloc(1, sizeof(menu_t));

    menu->mode = MENU_MODE_INIT;
    menu->next_mode = MENU_MODE_BROWSER;

    menu->browser.valid = false;
    menu->browser.directory = path_init(NULL); // TODO: load starting directory from settings

    return menu;
}

static void menu_deinit (menu_t *menu) {
    path_free(menu->browser.directory);
    free(menu);
}


void menu_run (settings_t *settings) {
    menu_t *menu = menu_init(settings);

    bool running = true;

    while (running) {
        surface_t *display = display_try_get();

        if (display != NULL) {
            actions_update(menu);

            switch (menu->mode) {
                case MENU_MODE_INIT:
                    view_init_display(menu, display);
                    break;

                case MENU_MODE_BROWSER:
                    view_browser_display(menu, display);
                    break;

                case MENU_MODE_FILE_INFO:
                    view_file_info_display(menu, display);
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
                    break;
            }

            while (menu->mode != menu->next_mode) {
                menu->mode = menu->next_mode;

                switch (menu->next_mode) {
                    case MENU_MODE_BROWSER:
                        view_browser_init(menu);
                        break;

                    case MENU_MODE_FILE_INFO:
                        view_file_info_init(menu);
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
                        running = false;
                        break;

                    default:
                        break;
                }
            }
        }
    }

    menu_deinit(menu);
}
