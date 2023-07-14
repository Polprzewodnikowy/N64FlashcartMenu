#include <stdlib.h>
#include <string.h>

#include <fatfs/ff.h>
#include <libdragon.h>

#include "fragments/fragments.h"
#include "utils/fs.h"
#include "views.h"


static const char *rom_extensions[] = { "z64", "n64", "v64", NULL };
static const char *emulator_extensions[] = { "nes", NULL }; //  "gb", "gbc", "smc", "gen", "smd",
static const char *save_extensions[] = { "sav", NULL };
static const char *music_extensions[] = { "mp3", NULL };


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
        } else if (a->type == ENTRY_TYPE_EMULATOR) {
            return -1;
        } else if (b->type == ENTRY_TYPE_EMULATOR) {
            return 1;
        } else if (a->type == ENTRY_TYPE_SAVE) {
            return -1;
        } else if (b->type == ENTRY_TYPE_SAVE) {
            return 1;
        } else if (a->type == ENTRY_TYPE_MUSIC) {
            return -1;
        } else if (b->type == ENTRY_TYPE_MUSIC) {
            return 1;
        }
    }

    return strcasecmp((const char *) (a->name), (const char *) (b->name));
}

static bool load_directory (menu_t *menu) {
    DIR dir;
    FILINFO info;

    for (int i = menu->browser.entries - 1; i >= 0; i--) {
        free(menu->browser.list[i].name);
    }
    menu->browser.entries = 0;
    menu->browser.selected = -1;

    if (f_opendir(&dir, path_get(menu->browser.directory)) != FR_OK) {
        return true;
    }

    while (menu->browser.entries < BROWSER_LIST_SIZE) {
        if (f_readdir(&dir, &info) != FR_OK) {
            return true;
        }

        size_t length = strlen(info.fname);

        if (length == 0) {
            break;
        }

        if (info.fattrib & AM_SYS) {
            continue;
        }
        if ((info.fattrib & AM_HID) && !menu->settings.show_hidden_files) {
            continue;
        }

        entry_t *entry = &menu->browser.list[menu->browser.entries];

        entry->name = strdup(info.fname);
        if (!entry->name) {
            f_closedir(&dir);
            return true;
        }

        if (info.fattrib & AM_DIR) {
            entry->type = ENTRY_TYPE_DIR;
        } else if (file_has_extensions(info.fname, rom_extensions)) {
            entry->type = ENTRY_TYPE_ROM;
        }else if (file_has_extensions(info.fname, emulator_extensions)) {
            entry->type = ENTRY_TYPE_EMULATOR;
        } else if (file_has_extensions(info.fname, save_extensions)) {
            entry->type = ENTRY_TYPE_SAVE;
        } else if (file_has_extensions(info.fname, music_extensions)) {
            entry->type = ENTRY_TYPE_MUSIC;
        } else {
            entry->type = ENTRY_TYPE_OTHER;
        }

        entry->size = info.fsize;

        menu->browser.entries += 1;
    }

    if (f_closedir(&dir) != FR_OK) {
        return true;
    }

    if (menu->browser.entries > 0) {
        menu->browser.selected = 0;
    }

    qsort(menu->browser.list, menu->browser.entries, sizeof(entry_t), compare_entry);

    return false;
}

static bool push_directory (menu_t *menu, char *directory) {
    path_t *previous_directory = path_clone(menu->browser.directory);

    path_push(menu->browser.directory, directory);

    if (load_directory(menu)) {
        path_free(menu->browser.directory);
        menu->browser.directory = previous_directory;
        return true;
    }

    path_free(previous_directory);

    return false;
}

static bool pop_directory (menu_t *menu) {
    path_t *previous_directory = path_clone(menu->browser.directory);

    path_pop(menu->browser.directory);

    if (load_directory(menu)) {
        path_free(menu->browser.directory);
        menu->browser.directory = previous_directory;
        return true;
    }

    for (int i = 0; i < menu->browser.entries; i++) {
        if (strcmp(menu->browser.list[i].name, path_last_get(previous_directory)) == 0) {
            menu->browser.selected = i;
            break;
        }
    }

    path_free(previous_directory);

    return false;
}

static void format_size (char *buffer, int size) {
    if (size < 8 * 1024) {
        sprintf(buffer, "%4d B ", size);
    } else if (size < 8 * 1024 * 1024) {
        sprintf(buffer, "%4d kB", size / 1024);
    } else if (size < 1 * 1024 * 1024 * 1024) {
        sprintf(buffer, "%4d MB", size / 1024 / 1024);
    } else {
        sprintf(buffer, "%4d GB", size / 1024 / 1024 / 1024);
    }
}

static void format_entry (char *buffer, entry_t *entry, bool selected) {
    int cutoff_length = (entry->type == ENTRY_TYPE_DIR ? 57 : 49);
    int name_length = strlen(entry->name);
    strcpy(buffer, "");
    if (entry->type == ENTRY_TYPE_DIR) {
        strcat(buffer, "/");
    }
    if (name_length > cutoff_length) {
        strncat(buffer, entry->name, cutoff_length - 1);
        strcat(buffer, "…");
    } else {
        strcat(buffer, entry->name);
    }
}


static void process (menu_t *menu) {
    int scroll_speed = menu->actions.fast ? 10 : 1;

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
                if (push_directory(menu, entry->name)) {
                    menu->browser.valid = false;
                    menu->next_mode = MENU_MODE_ERROR;
                }
                break;
            case ENTRY_TYPE_ROM:
                menu->next_mode = MENU_MODE_LOAD;
                break;
            case ENTRY_TYPE_EMULATOR:
                menu->next_mode = MENU_MODE_EMULATOR_LOAD;
                break;
            case ENTRY_TYPE_MUSIC:
                menu->next_mode = MENU_MODE_MUSIC_PLAYER;
                break;
            default:
                menu->next_mode = MENU_MODE_FILE_INFO;
                break;
        }
    } else if (menu->actions.back && !path_is_root(menu->browser.directory)) {
        if (pop_directory(menu)) {
            menu->browser.valid = false;
            menu->next_mode = MENU_MODE_ERROR;
        }
    } else if (menu->actions.file_info) {
        if (menu->browser.selected >= 0) {
            menu->next_mode = MENU_MODE_FILE_INFO;
        }
    } else if (menu->actions.system_info) {
        menu->next_mode = MENU_MODE_SYSTEM_INFO;
    } else if (menu->actions.settings) {
        menu->next_mode = MENU_MODE_CREDITS;
    }
}

static void draw (menu_t *menu, surface_t *d) {
    char buffer[64];

    layout_t *layout = layout_get();

    const int text_x = layout->offset_x + layout->offset_text_x;
    int text_y = layout->offset_y + layout->offset_text_y;
    const int text_file_size_x = text_x + 478;
    const int text_other_actions_x = text_x + 450;
    const int highlight_offset = 2;

    const color_t bg_color = RGBA32(0x00, 0x00, 0x00, 0xFF);
    const color_t highlight_color = RGBA32(0x3F, 0x3F, 0x3F, 0xFF);
    const color_t text_color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF);
    const color_t directory_color = RGBA32(0xFF, 0xFF, 0x70, 0xFF);
    const color_t save_color = RGBA32(0x70, 0xFF, 0x70, 0xFF);
    const color_t music_color = RGBA32(0x70, 0xBC, 0xFF, 0xFF);
    const color_t other_color = RGBA32(0xA0, 0xA0, 0xA0, 0xFF);

    int starting_position = 0;

    if (menu->browser.entries > layout->main_lines && menu->browser.selected >= (layout->main_lines / 2)) {
        starting_position = menu->browser.selected - (layout->main_lines / 2);
        if (starting_position >= menu->browser.entries - layout->main_lines) {
            starting_position = menu->browser.entries - layout->main_lines;
        }
    }

    rdpq_attach(d, NULL);
    rdpq_clear(bg_color);

    // Layout
    fragment_borders(d);
    fragment_scrollbar(d, menu->browser.selected, menu->browser.entries);

    // Highlight
    if (menu->browser.entries > 0) {
        rdpq_set_mode_fill(highlight_color);
        rdpq_fill_rectangle(
            layout->offset_x,
            text_y + highlight_offset + ((menu->browser.selected - starting_position) * layout->line_height),
            d->width - layout->offset_x - layout->scrollbar_width,
            text_y + layout->line_height + highlight_offset + ((menu->browser.selected - starting_position) * layout->line_height)
        );
    }

    // Text start
    fragment_text_start(text_color);

    // Main screen
    for (int i = starting_position; i < menu->browser.entries; i++) {
        if (i == (starting_position + layout->main_lines)) {
            break;
        }

        entry_t *entry = &menu->browser.list[i];

        switch (entry->type) {
            case ENTRY_TYPE_DIR: fragment_text_set_color(directory_color); break;
            case ENTRY_TYPE_SAVE: fragment_text_set_color(save_color); break;
            case ENTRY_TYPE_OTHER: fragment_text_set_color(other_color); break;
            case ENTRY_TYPE_MUSIC: fragment_text_set_color(music_color); break;
            default: fragment_text_set_color(text_color); break;
        }

        format_entry(buffer, entry, i == menu->browser.selected);
        fragment_textf(text_x, text_y, buffer);

        if (entry->type != ENTRY_TYPE_DIR) {
            format_size(buffer, entry->size);
            fragment_text_set_color(text_color);
            fragment_textf(text_file_size_x, text_y, buffer);
        }

        text_y += layout->line_height;
    }

    if (menu->browser.entries == 0) {
        fragment_text_set_color(other_color);
        fragment_textf(text_x, text_y, "** empty directory **");
    }

    // Actions bar
    fragment_text_set_color(text_color);
    text_y = layout->actions_y + layout->offset_text_y;
    if (menu->browser.entries > 0) {
        switch (menu->browser.list[menu->browser.selected].type) {
            case ENTRY_TYPE_DIR:
                fragment_textf(text_x, text_y, "A: Enter");
                break;
            case ENTRY_TYPE_EMULATOR:
            case ENTRY_TYPE_ROM:
                fragment_textf(text_x, text_y, "A: Load");
                break;
            case ENTRY_TYPE_MUSIC:
                fragment_textf(text_x, text_y, "A: Play");
                break;
            default:
                fragment_textf(text_x, text_y, "A: Info");
                break;
        }
        fragment_textf(text_other_actions_x, text_y, "R: Info");
    }
    text_y += layout->line_height;
    if (!path_is_root(menu->browser.directory)) {
        fragment_textf(text_x, text_y, "B: Back");
    }
    fragment_textf(text_other_actions_x, text_y, "L: Settings");

    rdpq_detach_show();
}


void view_browser_init (menu_t *menu) {
    if (!menu->browser.valid) {
        if (load_directory(menu)) {
            path_free(menu->browser.directory);
            menu->browser.directory = path_init(NULL);
            menu->next_mode = MENU_MODE_ERROR;
        } else {
            menu->browser.valid = true;
        }
    }
}

void view_browser_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}
