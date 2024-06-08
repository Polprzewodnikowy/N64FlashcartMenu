#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../fonts.h"
#include "utils/fs.h"
#include "views.h"
#include "../sound.h"


static const char *rom_extensions[] = { "z64", "n64", "v64", "rom", NULL };
static const char *disk_extensions[] = { "ndd", NULL };
static const char *emulator_extensions[] = { "nes", "sfc", "smc", "gb", "gbc", "sms", "gg", "sg", NULL };
// TODO: "eep", "sra", "srm", "fla" could be used if transfered from different flashcarts.
static const char *save_extensions[] = { "sav", NULL };
static const char *image_extensions[] = { "png", NULL };
static const char *text_extensions[] = { "txt", "ini", "yml", "yaml", NULL };
static const char *music_extensions[] = { "mp3", NULL };

static const char *hidden_paths[] = {
    "/menu.bin",
    "/menu",
    "/N64FlashcartMenu.n64",
    "/ED64",
    "/ED64P",
    "/sc64menu.n64",
    "/System Volume Information",
    NULL,
};


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
        } else if (a->type == ENTRY_TYPE_DISK) {
            return -1;
        } else if (b->type == ENTRY_TYPE_DISK) {
            return 1;
        } else if (a->type == ENTRY_TYPE_EMULATOR) {
            return -1;
        } else if (b->type == ENTRY_TYPE_EMULATOR) {
            return 1;
        } else if (a->type == ENTRY_TYPE_SAVE) {
            return -1;
        } else if (b->type == ENTRY_TYPE_SAVE) {
            return 1;
        } else if (a->type == ENTRY_TYPE_IMAGE) {
            return -1;
        } else if (b->type == ENTRY_TYPE_IMAGE) {
            return 1;
        } else if (a->type == ENTRY_TYPE_MUSIC) {
            return -1;
        } else if (b->type == ENTRY_TYPE_MUSIC) {
            return 1;
        } else if (a->type == ENTRY_TYPE_TEXT) {
            return -1;
        } else if (b->type == ENTRY_TYPE_TEXT) {
            return 1;
        }
    }

    return strcasecmp((const char *) (a->name), (const char *) (b->name));
}

static void browser_list_free (menu_t *menu) {
    for (int i = menu->browser.entries - 1; i >= 0; i--) {
        free(menu->browser.list[i].name);
    }

    free(menu->browser.list);

    menu->browser.list = NULL;
    menu->browser.entries = 0;
    menu->browser.entry = NULL;
    menu->browser.selected = -1;
}

static bool load_directory (menu_t *menu) {
    int result;
    dir_t info;

    browser_list_free(menu);

    path_t *path = path_clone(menu->browser.directory);

    result = dir_findfirst(path_get(path), &info);

    while (result == 0) {
        bool hide = false;

        if (!menu->settings.show_protected_entries) {
            path_push(path, info.d_name);

            for (int i = 0; hidden_paths[i] != NULL; i++) {
                if (strcmp(strip_fs_prefix(path_get(path)), hidden_paths[i]) == 0) {
                    hide = true;
                    break;
                }
            }

            path_pop(path);
        }

        if (!hide) {
            menu->browser.list = realloc(menu->browser.list, (menu->browser.entries + 1) * sizeof(entry_t));

            entry_t *entry = &menu->browser.list[menu->browser.entries++];

            entry->name = strdup(info.d_name);
            if (!entry->name) {
                path_free(path);
                browser_list_free(menu);
                return true;
            }

            if (info.d_type == DT_DIR) {
                entry->type = ENTRY_TYPE_DIR;
            } else if (file_has_extensions(entry->name, rom_extensions)) {
                entry->type = ENTRY_TYPE_ROM;
            } else if (file_has_extensions(entry->name, disk_extensions)) {
                entry->type = ENTRY_TYPE_DISK;
            }else if (file_has_extensions(entry->name, emulator_extensions)) {
                entry->type = ENTRY_TYPE_EMULATOR;
            } else if (file_has_extensions(entry->name, save_extensions)) {
                entry->type = ENTRY_TYPE_SAVE;
            } else if (file_has_extensions(entry->name, image_extensions)) {
                entry->type = ENTRY_TYPE_IMAGE;
            } else if (file_has_extensions(entry->name, text_extensions)) {
                entry->type = ENTRY_TYPE_TEXT;
            } else if (file_has_extensions(entry->name, music_extensions)) {
                entry->type = ENTRY_TYPE_MUSIC;
            } else {
                entry->type = ENTRY_TYPE_OTHER;
            }

            entry->size = info.d_size;
        }

        result = dir_findnext(path_get(path), &info);
    }

    path_free(path);

    if (result < -1) {
        browser_list_free(menu);
        return true;
    }

    if (menu->browser.entries > 0) {
        menu->browser.selected = 0;
        menu->browser.entry = &menu->browser.list[menu->browser.selected];
    }

    qsort(menu->browser.list, menu->browser.entries, sizeof(entry_t), compare_entry);

    return false;
}

static bool reload_directory (menu_t *menu) {
    int selected = menu->browser.selected;

    if (load_directory(menu)) {
        return true;
    }

    menu->browser.selected = selected;
    if (menu->browser.selected >= menu->browser.entries) {
        menu->browser.selected = menu->browser.entries - 1;
    }
    menu->browser.entry = menu->browser.selected >= 0 ? &menu->browser.list[menu->browser.selected] : NULL;

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
            menu->browser.entry = &menu->browser.list[menu->browser.selected];
            break;
        }
    }

    path_free(previous_directory);

    return false;
}

static void show_properties (menu_t *menu, void *arg) {
    menu->next_mode = MENU_MODE_FILE_INFO;
}

static void delete_entry (menu_t *menu, void *arg) {
    path_t *path = path_clone_push(menu->browser.directory, menu->browser.entry->name);

    if (remove(path_get(path))) {
        menu->browser.valid = false;
        if (menu->browser.entry->type == ENTRY_TYPE_DIR) {
            menu_show_error(menu, "Couldn't delete directory\nDirectory might not be empty");
        } else {
            menu_show_error(menu, "Couldn't delete file");
        }
        path_free(path);
        return;
    }

    path_free(path);

    if (reload_directory(menu)) {
        menu->browser.valid = false;
        menu_show_error(menu, "Couldn't refresh directory contents after delete operation");
    }
}

static void set_default_directory (menu_t *menu, void *arg) {
    free(menu->settings.default_directory);
    menu->settings.default_directory = strdup(strip_fs_prefix(path_get(menu->browser.directory)));
    settings_save(&menu->settings);
}

static component_context_menu_t entry_context_menu = {
    .list = {
        { .text = "Show entry properties", .action = show_properties },
        { .text = "Delete selected entry", .action = delete_entry },
        { .text = "Set current directory as default", .action = set_default_directory },
        COMPONENT_CONTEXT_MENU_LIST_END,
    }
};

static void set_menu_next_mode (menu_t *menu, void *arg) {
    menu_mode_t next_mode = (menu_mode_t) (arg);
    menu->next_mode = next_mode;
}

static component_context_menu_t settings_context_menu = {
    .list = {
        { .text = "Edit settings", .action = set_menu_next_mode, .arg = (void *) (MENU_MODE_SETTINGS_EDITOR) },
        { .text = "Show system info", .action = set_menu_next_mode, .arg = (void *) (MENU_MODE_SYSTEM_INFO) },
        { .text = "Show credits", .action = set_menu_next_mode, .arg = (void *) (MENU_MODE_CREDITS) },
        { .text = "Adjust RTC", .action = set_menu_next_mode, .arg = (void *) (MENU_MODE_RTC) },
        { .text = "Show cart info", .action = set_menu_next_mode, .arg = (void *) (MENU_MODE_FLASHCART) },
        COMPONENT_CONTEXT_MENU_LIST_END,
    }
};

static void process (menu_t *menu) {
    if (component_context_menu_process(menu, &entry_context_menu)) {
        return;
    }

    if (component_context_menu_process(menu, &settings_context_menu)) {
        return;
    }

    int scroll_speed = menu->actions.go_fast ? 10 : 1;

    if (menu->browser.entries > 1) {
        if (menu->actions.go_up) {
            menu->browser.selected -= scroll_speed;
            if (menu->browser.selected < 0) {
                menu->browser.selected = 0;
            }
            sound_play_effect(SFX_CURSOR);
        } else if (menu->actions.go_down) {
            menu->browser.selected += scroll_speed;
            if (menu->browser.selected >= menu->browser.entries) {
                menu->browser.selected = menu->browser.entries - 1;
            }
            sound_play_effect(SFX_CURSOR);
        }
        menu->browser.entry = &menu->browser.list[menu->browser.selected];
    }

    if (menu->actions.enter && menu->browser.entry) {
        sound_play_effect(SFX_ENTER);
        switch (menu->browser.entry->type) {
            case ENTRY_TYPE_DIR:
                if (push_directory(menu, menu->browser.entry->name)) {
                    menu->browser.valid = false;
                    menu_show_error(menu, "Couldn't open next directory");
                }
                break;
            case ENTRY_TYPE_ROM:
                menu->next_mode = MENU_MODE_LOAD_ROM;
                break;
            case ENTRY_TYPE_DISK:
                menu->next_mode = MENU_MODE_LOAD_DISK;
                break;
            case ENTRY_TYPE_EMULATOR:
                menu->next_mode = MENU_MODE_LOAD_EMULATOR;
                break;
            case ENTRY_TYPE_IMAGE:
                menu->next_mode = MENU_MODE_IMAGE_VIEWER;
                break;
            case ENTRY_TYPE_TEXT:
                menu->next_mode = MENU_MODE_TEXT_VIEWER;
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
            menu_show_error(menu, "Couldn't open last directory");
        }
        sound_play_effect(SFX_EXIT);
    } else if (menu->actions.options && menu->browser.entry) {
        component_context_menu_show(&entry_context_menu);
        sound_play_effect(SFX_SETTING);
    } else if (menu->actions.settings) {
        component_context_menu_show(&settings_context_menu);
        sound_play_effect(SFX_SETTING);
    }
}


static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    component_background_draw();

    component_layout_draw();

    component_file_list_draw(menu->browser.list, menu->browser.entries, menu->browser.selected);

    const char *action = NULL;

    if (menu->browser.entry) {
        switch (menu->browser.entry->type) {
            case ENTRY_TYPE_DIR: action = "A: Enter"; break;
            case ENTRY_TYPE_ROM: action = "A: Load"; break;
            case ENTRY_TYPE_DISK: action = "A: Load"; break;
            case ENTRY_TYPE_IMAGE: action = "A: Show"; break;
            case ENTRY_TYPE_TEXT: action = "A: View"; break;
            case ENTRY_TYPE_MUSIC: action = "A: Play"; break;
            default: action = "A: Info"; break;
        }
    }

    component_actions_bar_text_draw(
        ALIGN_LEFT, VALIGN_TOP,
        "%s\n"
        "^%02XB: Back^00",
        menu->browser.entries == 0 ? "" : action,
        path_is_root(menu->browser.directory) ? STL_GRAY : STL_DEFAULT
    );

    component_actions_bar_text_draw(
        ALIGN_RIGHT, VALIGN_TOP,
        "Start: Settings\n"
        "^%02XR: Options^00",
        menu->browser.entries == 0 ? STL_GRAY : STL_DEFAULT
    );

    if (menu->current_time >= 0) {
        component_actions_bar_text_draw(
            ALIGN_CENTER, VALIGN_TOP,
            "\n"
            "%s",
            ctime(&menu->current_time)
        );
    }

    component_context_menu_draw(&entry_context_menu);

    component_context_menu_draw(&settings_context_menu);

    rdpq_detach_show();
}


void view_browser_init (menu_t *menu) {
    if (!menu->browser.valid) {
        component_context_menu_init(&entry_context_menu);
        component_context_menu_init(&settings_context_menu);
        if (load_directory(menu)) {
            path_free(menu->browser.directory);
            menu->browser.directory = path_init(menu->storage_prefix, "");
            menu_show_error(menu, "Error while opening initial directory");
        } else {
            menu->browser.valid = true;
        }
    }

    if (menu->browser.reload) {
        menu->browser.reload = false;
        if (reload_directory(menu)) {
            menu_show_error(menu, "Error while reloading current directory");
            menu->browser.valid = false;
        }
    }
}

void view_browser_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);
}
