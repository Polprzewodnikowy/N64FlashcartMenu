#include <errno.h>
#include <miniz.h>
#include <miniz_zip.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../fonts.h"
#include "utils/fs.h"
#include "views.h"
#include "../sound.h"



static const char *archive_extensions[] = { "zip", NULL };
static const char *cheat_extensions[] = {"cht", "cheats", "datel", "gameshark", NULL};
static const char *disk_extensions[] = { "ndd", NULL };
static const char *emulator_extensions[] = { "nes", "sfc", "smc", "gb", "gbc", "sms", "gg", "sg", "chf", NULL };
static const char *image_extensions[] = { "png", NULL };
static const char *music_extensions[] = { "mp3", NULL };
static const char *n64_rom_extensions[] = { "z64", "n64", "v64", "rom", NULL };
static const char *patch_extensions[] = { "bps", "ips", "aps", "ups", "xdelta", NULL };
// TODO: "eep", "sra", "srm", "fla" could be used if transfered from different flashcarts.
static const char *save_extensions[] = { "sav", NULL };
static const char *text_extensions[] = { "txt", "ini", "yml", "yaml", NULL };

static const char *hidden_root_paths[] = {
    "/menu.bin",
    "/menu",
    "/N64FlashcartMenu.n64",
    "/ED64",
    "/ED64P",
    "/sc64menu.n64",
    // Windows garbage
    "/System Volume Information",
    // macOS garbage
    "/.fseventsd",
    "/.Spotlight-V100",
    "/.Trashes",
    "/.VolumeIcon.icns",
    "/.metadata_never_index",
    NULL,
};

struct substr { const char *str; size_t len; };
#define substr(str) ((struct substr){ str, sizeof(str) - 1 })

static const struct substr hidden_basenames[] = {
    substr("desktop.ini"), // Windows Explorer settings
    substr("Thumbs.db"),   // Windows Explorer thumbnails
    substr(".DS_Store"),   // macOS Finder settings
};
#define HIDDEN_BASENAMES_COUNT (sizeof(hidden_basenames) / sizeof(hidden_basenames[0]))

static const struct substr hidden_prefixes[] = {
    substr("._"), // macOS "AppleDouble" metadata files
};
#define HIDDEN_PREFIXES_COUNT (sizeof(hidden_prefixes) / sizeof(hidden_prefixes[0]))


/**
 * Determines whether a filesystem path should be treated as hidden.
 *
 * @param path Path to evaluate.
 * @returns `true` if the path matches a hidden root path, its basename exactly matches a hidden basename,
 *          or its basename begins with a hidden prefix; `false` otherwise.
 */
static bool path_is_hidden (path_t *path) {
    char *stripped_path = strip_fs_prefix(path_get(path));

    // Check for hidden files based on full path
    for (size_t i = 0; hidden_root_paths[i] != NULL; i++) {
        if (strcmp(stripped_path, hidden_root_paths[i]) == 0) {
            return true;
        }
    }

    char *basename = file_basename(stripped_path);
    size_t basename_len = strlen(basename);

    // Check for hidden files based on filename
    for (size_t i = 0; i < HIDDEN_BASENAMES_COUNT; i++) {
        if (basename_len == hidden_basenames[i].len &&
            strncmp(basename, hidden_basenames[i].str, hidden_basenames[i].len) == 0) {
            return true;
        }
    }
    // Check for hidden files based on filename prefix
    for (size_t i = 0; i < HIDDEN_PREFIXES_COUNT; i++) {
        if (basename_len > hidden_prefixes[i].len &&
            strncmp(basename, hidden_prefixes[i].str, hidden_prefixes[i].len) == 0) {
            return true;
        }
    }

    return false;
}

/**
 * Compare two browser entries for ordering by type precedence then name.
 *
 * Entries are ordered so that directories come first, then archives, disks,
 * emulators, images, music, ROMs, ROM cheats, ROM patches, saves, texts, and
 * other types; entries of the same type are ordered case-insensitively by name.
 *
 * @param pa Pointer to the first entry_t to compare.
 * @param pb Pointer to the second entry_t to compare.
 * @returns A negative value if `pa` should sort before `pb`, zero if they are
 * equal in ordering, and a positive value if `pa` should sort after `pb`.
 */
static int compare_entry (const void *pa, const void *pb) {
    entry_t *a = (entry_t *) (pa);
    entry_t *b = (entry_t *) (pb);

    if (a->type != b->type) {
        if (a->type == ENTRY_TYPE_DIR) {
            return -1;
        } else if (b->type == ENTRY_TYPE_DIR) {
            return 1;
        } else if (a->type == ENTRY_TYPE_ARCHIVE) {
            return -1;
        } else if (b->type == ENTRY_TYPE_ARCHIVE) {
            return 1;
        } else if (a->type == ENTRY_TYPE_DISK) {
            return -1;
        } else if (b->type == ENTRY_TYPE_DISK) {
            return 1;
        } else if (a->type == ENTRY_TYPE_EMULATOR) {
            return -1;
        } else if (b->type == ENTRY_TYPE_EMULATOR) {
            return 1;
        } else if (a->type == ENTRY_TYPE_IMAGE) {
            return -1;
        } else if (b->type == ENTRY_TYPE_IMAGE) {
            return 1;
        } else if (a->type == ENTRY_TYPE_MUSIC) {
            return -1;
        } else if (b->type == ENTRY_TYPE_MUSIC) {
            return 1;
        } else if (a->type == ENTRY_TYPE_ROM) {
            return -1;
        } else if (b->type == ENTRY_TYPE_ROM) {
            return 1;
        } else if (a->type == ENTRY_TYPE_ROM_CHEAT) {
            return -1;
        } else if (b->type == ENTRY_TYPE_ROM_CHEAT) {
            return 1;
        } else if (a->type == ENTRY_TYPE_ROM_PATCH) {
            return -1;
        } else if (b->type == ENTRY_TYPE_ROM_PATCH) {
            return 1;
        } else if (a->type == ENTRY_TYPE_SAVE) {
            return -1;
        } else if (b->type == ENTRY_TYPE_SAVE) {
            return 1;
        } else if (a->type == ENTRY_TYPE_TEXT) {
            return -1;
        } else if (b->type == ENTRY_TYPE_TEXT) {
            return 1;
        }
    }

    return strcasecmp((const char *) (a->name), (const char *) (b->name));
}

/**
 * Free browser list resources and reset browser state for the given menu.
 *
 * If an archive is open, closes the ZIP reader, then frees each entry name
 * and the list array. Resets archive flag, entries count, list pointer,
 * current entry pointer, and selected index to their empty/default values.
 *
 * @param menu Menu whose browser resources will be freed and state reset.
 */
static void browser_list_free (menu_t *menu) {
    if (menu->browser.archive) {
        mz_zip_reader_end(&menu->browser.zip);
    }
    menu->browser.archive = false;

    for (int i = menu->browser.entries - 1; i >= 0; i--) {
        free(menu->browser.list[i].name);
    }

    free(menu->browser.list);

    menu->browser.list = NULL;
    menu->browser.entries = 0;
    menu->browser.entry = NULL;
    menu->browser.selected = -1;
}

/**
 * Load ZIP archive entries into the browser state.
 *
 * Initializes the ZIP reader for the current browser directory, populates
 * menu->browser.list with the archive's entries (marked as archived), sets
 * archive mode and entry count, and sorts the resulting list. On success the
 * first entry is selected when entries exist.
 *
 * @param menu Browser menu whose browser state will be updated.
 * @returns `true` if an error occurred while initializing or reading the archive,
 *          `false` otherwise.
 */
static bool load_archive (menu_t *menu) {
    browser_list_free(menu);

    mz_zip_zero_struct(&menu->browser.zip);
    if (!mz_zip_reader_init_file(&menu->browser.zip, path_get(menu->browser.directory), 0)) {
        return true;
    }

    menu->browser.archive = true;
    menu->browser.entries = (int32_t)mz_zip_reader_get_num_files(&menu->browser.zip);
    menu->browser.list = malloc(menu->browser.entries * sizeof(entry_t));
    if (!menu->browser.list) {
        browser_list_free(menu);
        return true;
    }

    for (int32_t i = 0; i < menu->browser.entries; i++) {
        entry_t *entry = &menu->browser.list[i];

        mz_zip_archive_file_stat info;
        if (!mz_zip_reader_file_stat(&menu->browser.zip, i, &info)) {
            browser_list_free(menu);
            return true;
        }

        entry->name = strdup(info.m_filename);
        if (!entry->name) {
            browser_list_free(menu);
            return true;
        }

        entry->type = ENTRY_TYPE_ARCHIVED;
        entry->size = info.m_uncomp_size;
        entry->index = i;
    }

    if (menu->browser.entries > 0) {
        menu->browser.selected = 0;
        menu->browser.entry = &menu->browser.list[menu->browser.selected];
    }

    qsort(menu->browser.list, menu->browser.entries, sizeof(entry_t), compare_entry);

    return false;
}

/**
 * Populate the browser list from the current directory stored in menu->browser.directory.
 *
 * Builds menu->browser.list and related state (entries count, selected index, and entry pointer)
 * by iterating the filesystem directory, skipping hidden entries and the "saves" folder
 * according to menu settings, and classifying each visible entry by file type/extension.
 *
 * @param menu Menu context whose browser state will be updated.
 * @returns `true` on error (list freed and browser state reset), `false` on success.
 */
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
            hide = path_is_hidden(path);
            path_pop(path);
        }

        if (!menu->settings.show_saves_folder) {
            path_push(path, info.d_name);
            // Skip the "saves" directory if it is hidden (this is case sensitive)
            if (strcmp(info.d_name, "saves") == 0) { // TODO: use SAVES_SUBDIRECTORY from cart_load.c
                hide = true;
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
            } else if (file_has_extensions(entry->name, n64_rom_extensions)) {
                entry->type = ENTRY_TYPE_ROM;
            } else if (file_has_extensions(entry->name, disk_extensions)) {
                entry->type = ENTRY_TYPE_DISK;
            } else if (file_has_extensions(entry->name, patch_extensions)) {
                entry->type = ENTRY_TYPE_ROM_PATCH;
            } else if (file_has_extensions(entry->name, cheat_extensions)) {
                entry->type = ENTRY_TYPE_ROM_CHEAT;
            } else if (file_has_extensions(entry->name, emulator_extensions)) {
                entry->type = ENTRY_TYPE_EMULATOR;
            } else if (file_has_extensions(entry->name, save_extensions)) {
                entry->type = ENTRY_TYPE_SAVE;
            } else if (file_has_extensions(entry->name, image_extensions)) {
                entry->type = ENTRY_TYPE_IMAGE;
            } else if (file_has_extensions(entry->name, text_extensions)) {
                entry->type = ENTRY_TYPE_TEXT;
            } else if (file_has_extensions(entry->name, music_extensions)) {
                entry->type = ENTRY_TYPE_MUSIC;
            } else if (file_has_extensions(entry->name, archive_extensions)) {
                entry->type = ENTRY_TYPE_ARCHIVE;
            } else {
                entry->type = ENTRY_TYPE_OTHER;
            }

            entry->size = info.d_size;
            entry->index = menu->browser.entries - 1;
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

/**
 * Reloads the current directory listing and restores the previous selection index.
 *
 * Updates menu->browser.list and related state by reloading the directory; if the
 * previous selection index is out of range after reload, selects the last entry.
 *
 * @param menu Menu instance whose browser state will be reloaded and updated.
 * @returns `true` if reloading the directory failed, `false` otherwise.
 */
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

/**
 * Pushes a new directory onto the browser path and loads its contents.
 *
 * @param menu Menu state whose browser directory will be modified.
 * @param directory Path component to push onto the current browser directory.
 * @param archive If true, treat the pushed path as a ZIP archive and open it.
 * @returns `true` if loading the new directory/archive failed and the previous directory was restored, `false` otherwise.
 */
static bool push_directory (menu_t *menu, char *directory, bool archive) {
    path_t *previous_directory = path_clone(menu->browser.directory);

    path_push(menu->browser.directory, directory);

    if (archive ? load_archive(menu) : load_directory(menu)) {
        path_free(menu->browser.directory);
        menu->browser.directory = previous_directory;
        return true;
    }

    path_free(previous_directory);

    return false;
}

/**
 * Pop the current directory from the browser path and load its parent.
 *
 * Attempts to move the browser up one directory level, reloads the new current
 * directory, and if successful restores the selected entry to the entry whose
 * name matches the directory that was popped. On failure the previous browser
 * directory state is restored.
 *
 * @param menu Menu whose browser directory and entries will be updated.
 * @returns `true` if loading the parent directory failed and the previous state was restored, `false` otherwise.
 */
static bool pop_directory (menu_t *menu) {
    path_t *previous_directory = path_clone(menu->browser.directory);

    path_pop(menu->browser.directory);

    if (load_directory(menu)) {
        path_free(menu->browser.directory);
        menu->browser.directory = previous_directory;
        return true;
    }

    for (uint16_t i = 0; i < menu->browser.entries; i++) {
        if (strcmp(menu->browser.list[i].name, path_last_get(previous_directory)) == 0) {
            menu->browser.selected = i;
            menu->browser.entry = &menu->browser.list[menu->browser.selected];
            break;
        }
    }

    path_free(previous_directory);

    return false;
}

/**
 * Set the browser to the directory containing `file` and select `file` in the list.
 *
 * This replaces the current browser directory with the parent directory of `file`,
 * loads that directory into the browser, and sets the browser's selected entry and
 * entry pointer to the list item whose name matches `file`'s basename. If loading
 * the directory fails, the original directory is restored.
 *
 * @param menu Browser menu state to modify.
 * @param file Path of the file to select; only its parent directory and basename are used.
 * @return `true` if loading the target directory failed and the previous directory was restored, `false` if the selection succeeded.
 */
static bool select_file (menu_t *menu, path_t *file) {
    path_t *previous_directory = path_clone(menu->browser.directory);

    path_free(menu->browser.directory);
    menu->browser.directory = path_clone(file);
    path_pop(menu->browser.directory);

    if (load_directory(menu)) {
        path_free(menu->browser.directory);
        menu->browser.directory = previous_directory;
        return true;
    }

    for (uint16_t i = 0; i < menu->browser.entries; i++) {
        if (strcmp(menu->browser.list[i].name, path_last_get(file)) == 0) {
            menu->browser.selected = i;
            menu->browser.entry = &menu->browser.list[menu->browser.selected];
            break;
        }
    }

    path_free(previous_directory);

    return false;
}

/**
 * Selects the appropriate next menu mode for showing the current entry's properties.
 *
 * Sets menu->next_mode to file extraction mode when the current entry is from an archive,
 * otherwise sets it to the regular file info mode.
 *
 * @param menu Menu state containing the browser and current entry.
 * @param arg  Unused.
 */
static void show_properties (menu_t *menu, void *arg) {
    menu->next_mode = menu->browser.entry->type == ENTRY_TYPE_ARCHIVED ? MENU_MODE_EXTRACT_FILE : MENU_MODE_FILE_INFO;
}

/**
 * Delete the currently selected browser entry and refresh the directory listing.
 *
 * Attempts to remove the filesystem entry represented by the menu's current selection.
 * On failure, marks the browser state invalid and shows a context-appropriate error
 * message. After a successful removal, reloads the directory and, on reload failure,
 * marks the browser invalid and shows an error.
 *
 * @param menu Pointer to the menu containing the browser state and current entry.
 * @param arg  Unused; provided to match the context-menu callback signature.
 */
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

/**
 * Mark the currently selected entry for extraction and transition the menu to extract-file mode.
 * @param menu Menu state to modify; sets the pending extract flag and updates next_mode.
 * @param arg Unused.
 */
static void extract_entry (menu_t *menu, void *arg) {
    menu->load_pending.extract_file = true;
    menu->next_mode = MENU_MODE_EXTRACT_FILE;
}

/**
 * Save the current browser directory (with filesystem prefix removed) as the default and persist settings.
 *
 * @param menu Menu whose browser.directory will be stored as the default.
 */
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

static component_context_menu_t archive_context_menu = {
    .list = {
        { .text = "Show entry properties", .action = show_properties },
        { .text = "Extract selected entry", .action = extract_entry },
        COMPONENT_CONTEXT_MENU_LIST_END,
    }
};

/**
 * Set the menu's next mode to the mode value encoded in `arg`.
 *
 * @param menu The menu whose next_mode will be set.
 * @param arg A `menu_mode_t` value passed as a `void *`; the function casts this pointer to `menu_mode_t` and assigns it to `menu->next_mode`.
 */
static void set_menu_next_mode (menu_t *menu, void *arg) {
    menu_mode_t next_mode = (menu_mode_t) (arg);
    menu->next_mode = next_mode;
}

static component_context_menu_t settings_context_menu = {
    .list = {
        { .text = "Controller Pak manager", .action = set_menu_next_mode, .arg = (void *) (MENU_MODE_CONTROLLER_PAKFS) },
        { .text = "Menu settings", .action = set_menu_next_mode, .arg = (void *) (MENU_MODE_SETTINGS_EDITOR) },
        { .text = "Time (RTC) settings", .action = set_menu_next_mode, .arg = (void *) (MENU_MODE_RTC) },
        { .text = "Menu information", .action = set_menu_next_mode, .arg = (void *) (MENU_MODE_CREDITS) },
        { .text = "Flashcart information", .action = set_menu_next_mode, .arg = (void *) (MENU_MODE_FLASHCART) },
        { .text = "N64 information", .action = set_menu_next_mode, .arg = (void *) (MENU_MODE_SYSTEM_INFO) },
        COMPONENT_CONTEXT_MENU_LIST_END,
    }
};

/**
 * Handle user input and update the browser menu state accordingly.
 *
 * Processes context menus and navigation actions for the browser view, including
 * cursor movement, opening/closing directories (including archives), showing
 * context menus, selecting entries to change the next application mode, and
 * invoking error messages or sounds when operations fail or succeed.
 *
 * @param menu Pointer to the menu state to operate on; this function reads and
 *             mutates fields such as browser selection, browser validity, and
 *             next_mode.
 */
static void process (menu_t *menu) {
    if (ui_components_context_menu_process(menu, menu->browser.archive ? &archive_context_menu : &entry_context_menu)) {
        return;
    }

    if (ui_components_context_menu_process(menu, &settings_context_menu)) {
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
            case ENTRY_TYPE_ARCHIVE:
                if (push_directory(menu, menu->browser.entry->name, true)) {
                    menu->browser.valid = false;
                    menu_show_error(menu, "Couldn't open file archive");
                }
                break;
            case ENTRY_TYPE_ARCHIVED:
                menu->next_mode = MENU_MODE_EXTRACT_FILE;
                break;
            case ENTRY_TYPE_DIR:
                if (push_directory(menu, menu->browser.entry->name, false)) {
                    menu->browser.valid = false;
                    menu_show_error(menu, "Couldn't open next directory");
                }
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
            case ENTRY_TYPE_MUSIC:
                menu->next_mode = MENU_MODE_MUSIC_PLAYER;
                break;
            case ENTRY_TYPE_ROM:
                menu->next_mode = MENU_MODE_LOAD_ROM;
                break;
            case ENTRY_TYPE_ROM_CHEAT:
                menu->next_mode = MENU_MODE_FILE_INFO; // FIXME: Implement MENU_MODE_LOAD_ROM_CHEAT.
                break;
            case ENTRY_TYPE_ROM_PATCH:
                menu->next_mode = MENU_MODE_FILE_INFO; // FIXME: Implement MENU_MODE_LOAD_ROM_PATCH.
                break;
            case ENTRY_TYPE_TEXT:
                menu->next_mode = MENU_MODE_TEXT_VIEWER;
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
        ui_components_context_menu_show(menu->browser.archive ? &archive_context_menu : &entry_context_menu);
        sound_play_effect(SFX_SETTING);
    } else if (menu->actions.settings) {
        ui_components_context_menu_show(&settings_context_menu);
        sound_play_effect(SFX_SETTING);
    } else if (menu->actions.go_right) {
        menu->next_mode = MENU_MODE_HISTORY;
        sound_play_effect(SFX_CURSOR);
    } else if (menu->actions.go_left) {
        menu->next_mode = MENU_MODE_FAVORITE;
        sound_play_effect(SFX_CURSOR);
    }
}

/**
 * Render the browser view onto the given drawing surface.
 *
 * Draws the background, tab headers, tabbed layout, and the file list, then renders the action bars
 * (left, right, and center) with context-sensitive labels based on the currently selected entry type,
 * displays the current time or a tab-change hint, and draws the entry/archive and settings context menus.
 *
 * @param menu The menu state containing browser data (directory, entries, current selection, time, and context flags).
 * @param d    The destination surface to render into.
 */
static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    ui_components_background_draw();

    ui_components_tabs_common_draw(0);

    ui_components_layout_draw_tabbed();

    ui_components_file_list_draw(menu->browser.list, menu->browser.entries, menu->browser.selected);

    const char *action = NULL;

    if (menu->browser.entry) {
        switch (menu->browser.entry->type) {
            case ENTRY_TYPE_DIR: action = "A: Enter"; break;
            case ENTRY_TYPE_ROM: action = "A: Load"; break;
            case ENTRY_TYPE_DISK: action = "A: Load"; break;
            case ENTRY_TYPE_IMAGE: action = "A: Show"; break;
            case ENTRY_TYPE_TEXT: action = "A: View"; break;
            case ENTRY_TYPE_MUSIC: action = "A: Play"; break;
            case ENTRY_TYPE_ARCHIVE: action = "A: Open"; break;
            default: action = "A: Info"; break;
        }
    }

    ui_components_actions_bar_text_draw(
        STL_DEFAULT,
        ALIGN_LEFT, VALIGN_TOP,
        "%s\n"
        "^%02XB: Back^00",
        menu->browser.entries == 0 ? "" : action,
        path_is_root(menu->browser.directory) ? STL_GRAY : STL_DEFAULT
    );

    ui_components_actions_bar_text_draw(
        STL_DEFAULT,
        ALIGN_RIGHT, VALIGN_TOP,
        "^%02XStart: Settings^00\n"
        "^%02XR:  Options^00",
        menu->browser.entries == 0 ? STL_GRAY : STL_DEFAULT
    );

    if (menu->current_time >= 0) {
        ui_components_actions_bar_text_draw(
            STL_DEFAULT,
            ALIGN_CENTER, VALIGN_TOP,
            "C-Up/Down: Fast Scroll\n"
            "%s",
            ctime(&menu->current_time)
        );
    } else {
        ui_components_actions_bar_text_draw(
            STL_DEFAULT,
            ALIGN_CENTER, VALIGN_TOP,
            "< Change Tab >\n"
            "\n"
        );
    }

    ui_components_context_menu_draw(menu->browser.archive ? &archive_context_menu : &entry_context_menu);

    ui_components_context_menu_draw(&settings_context_menu);

    rdpq_detach_show();
}


/**
 * Initialize and prepare the browser view state for the given menu.
 *
 * Ensures browser context menus are initialized and attempts to load the initial
 * directory. If loading the initial directory fails the browser directory is
 * reset to the menu's storage prefix and an error is shown. Processes a pending
 * selection (if menu->browser.select_file is set) by navigating to that file and
 * clears the pending selection; on failure the browser is marked invalid and an
 * error is shown. If the reload flag is set, attempts to reload the current
 * directory and clears the flag; on failure an error is shown and the browser
 * is marked invalid. Sets or clears menu->browser.valid according to success.
 *
 * @param menu Menu instance whose browser view will be initialized and prepared.
 */
void view_browser_init (menu_t *menu) {
    if (!menu->browser.valid) {
        ui_components_context_menu_init(&entry_context_menu);
        ui_components_context_menu_init(&archive_context_menu);
        ui_components_context_menu_init(&settings_context_menu);
        if (load_directory(menu)) {
            path_free(menu->browser.directory);
            menu->browser.directory = path_init(menu->storage_prefix, "");
            menu_show_error(menu, "Error while opening initial directory");
        } else {
            menu->browser.valid = true;
        }
    }

    if (menu->browser.select_file) {
        if (select_file(menu, menu->browser.select_file)) {
            menu->browser.valid = false;
            menu_show_error(menu, "Error while navigating to file");
        }
        path_free(menu->browser.select_file);
        menu->browser.select_file = NULL;
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