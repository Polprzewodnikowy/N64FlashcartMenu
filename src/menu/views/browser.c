#include <fatfs/ff.h>
#include <libdragon.h>
#include <stdlib.h>
#include "../menu.h"
#include "../menu_res_setup.h"
#include "../../utils/str_utils.h"


#define BROWSER_LIST_ROWS   21


static int compare_entry (const void *pa, const void *pb) {
    entry_t *a = (entry_t *) (pa);
    entry_t *b = (entry_t *) (pb);

    if (a->type != b->type) {
        if (a->type == ENTRY_TYPE_DIR) {
            return -1;
        } else if (b->type == ENTRY_TYPE_DIR) {
            return 1;
        } else if (a->type == ENTRY_TYPE_ROM) {
            return -1;
        } else if (b->type == ENTRY_TYPE_ROM) {
            return 1;
        } else if (a->type == ENTRY_TYPE_SAVE) {
            return -1;
        } else if (b->type == ENTRY_TYPE_SAVE) {
            return 1;
        }
    }

    return strcasecmp((const char *) (a->name), (const char *) (b->name));
}

static void load_directory (menu_t *menu) {
    DIR dir;
    FILINFO info;

    for (int i = 0; i < menu->browser.entries; i++) {
        free(menu->browser.list[i].name);
    }
    menu->browser.entries = 0;
    menu->browser.selected = -1;

    if (f_opendir(&dir, path_get(menu->browser.directory)) != FR_OK) {
        menu->next_mode = MENU_MODE_ERROR;
        return;
    }

    while (true) {
        if (f_readdir(&dir, &info) != FR_OK) {
            menu->next_mode = MENU_MODE_ERROR;
            return;
        }

        size_t length = strlen(info.fname);
        if (length == 0) {
            break;
        }

        if (info.fattrib & AM_SYS) {
            continue;
        }

        entry_t *entry = &menu->browser.list[menu->browser.entries];

        entry->name = malloc((length + 1) * sizeof(char));
        strcpy(entry->name, info.fname);

        if (info.fattrib & AM_DIR) {
            entry->type = ENTRY_TYPE_DIR;
        // TODO: use something like `ext_is_n64_rom(info.fname)` instead of `str_endswith(info.fname, ".xxx")`
        } else if (str_endswith(info.fname, ".n64") || str_endswith(info.fname, ".z64") || str_endswith(info.fname, ".v64") || str_endswith(info.fname, ".N64")) {
            entry->type = ENTRY_TYPE_ROM;
        } else if (str_endswith(info.fname, ".sav")) {
            entry->type = ENTRY_TYPE_SAVE;
        } else {
            entry->type = ENTRY_TYPE_UNKNOWN;
        }

        menu->browser.entries += 1;
        if (menu->browser.entries == BROWSER_LIST_SIZE) {
            break;
        }
    }

    f_closedir(&dir);

    if (menu->browser.entries > 0) {
        menu->browser.selected = 0;
    }

    qsort(menu->browser.list, menu->browser.entries, sizeof(entry_t), compare_entry);
}

void push_directory (menu_t *menu, char *directory) {
    path_push(menu->browser.directory, directory);
    load_directory(menu);
}

void pop_directory (menu_t *menu) {
    path_t *current_directory = path_clone(menu->browser.directory);
    path_pop(menu->browser.directory);
    load_directory(menu);
    for (int i = 0; i < menu->browser.entries; i++) {
        if (strcmp(menu->browser.list[i].name, path_last_get(current_directory)) == 0) {
            menu->browser.selected = i;
            break;
        }
    }
    path_free(current_directory);
}


static void process (menu_t *menu) {
    int scroll_speed = menu->actions.fast ? BROWSER_LIST_ROWS : 1;

    if (menu->browser.entries > 1) {
        if (menu->actions.go_up) {
            menu->browser.selected -= scroll_speed;
            if (menu->browser.selected < 0) {
                menu->browser.selected = 0;
            }
        } else if (menu->actions.go_down) {
            menu->browser.selected += scroll_speed;
            if (menu->browser.selected >= menu->browser.entries) {
                menu->browser.selected = menu->browser.entries - 1;
            }
        }
    }

    if (menu->actions.enter) {
        entry_t *entry = &menu->browser.list[menu->browser.selected];

        switch (entry->type) {
            case ENTRY_TYPE_DIR:
                push_directory(menu, entry->name);
                break;

            case ENTRY_TYPE_ROM:
                menu->next_mode = MENU_MODE_LOAD;
                break;

            default:
                menu->next_mode = MENU_MODE_FILE_INFO;
                break;
        }
    } else if (menu->actions.back) {
        pop_directory(menu);
    } else if (menu->actions.info) {
        menu->next_mode = MENU_MODE_FILE_INFO;
    } else if (menu->actions.settings) {
        menu->next_mode = MENU_MODE_CREDITS;
    }
}

static void draw (menu_t *menu, surface_t *d) {
    int x = 24;
    int y = 35;

    int starting_position = 0;
    int entries_drawn = 0;

    if (menu->browser.entries > BROWSER_LIST_ROWS && menu->browser.selected >= (BROWSER_LIST_ROWS / 2)) {
        starting_position = menu->browser.selected - (BROWSER_LIST_ROWS / 2);
        if (starting_position >= menu->browser.entries - BROWSER_LIST_ROWS) {
            starting_position = menu->browser.entries - BROWSER_LIST_ROWS;
        }
    }

    graphics_fill_screen(d, graphics_make_color(0, 0, 0, 255));

    graphics_draw_text(d, (d->width / 2) - 36, vertical_start_position, "FILE MENU");
    graphics_draw_line(d, 0, 30, d->width, 30, 0xff);

    char str_buffer[1024];

    for (int i = starting_position; i < menu->browser.entries; i++) {
        if (i == menu->browser.selected) {
            uint32_t color;
            switch (menu->browser.list[i].type) {
                case ENTRY_TYPE_ROM:
                    color = graphics_make_color(0, 64, 0, 0);
                    break;
                default:
                    color = graphics_make_color(64, 64, 64, 0);
                    break;
            }
            graphics_draw_box(d, x, y, (640 - x * 2), font_vertical_pixels, color);
        }
        snprintf(str_buffer, 1024, "%.74s", menu->browser.list[i].name);
        graphics_draw_text(d, x, y, str_buffer);

        y += font_vertical_pixels;

        entries_drawn += 1;
        if (entries_drawn == BROWSER_LIST_ROWS) {
            break;
        }
    }

    graphics_draw_line(d, 0, d->height - overscan_vertical_pixels - font_vertical_pixels, d->width, d->height - overscan_vertical_pixels - font_vertical_pixels, 0xff);

    sprintf(str_buffer, "Current Directory: SD:%s\nFile: %d of %d\n\n", path_get(menu->browser.directory), menu->browser.selected + 1, menu->browser.entries);

    graphics_draw_text(d, (d->width / 2) - 160, d->height - overscan_vertical_pixels, str_buffer);

    display_show(d);
}


void view_browser_init (menu_t *menu) {
    if (!menu->browser.valid) {
        menu->browser.valid = true;
        load_directory(menu);
    }
}

void view_browser_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}
