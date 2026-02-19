/**
 * @file vcpak_select.c
 * @brief Virtual Controller Pak selection view.
 * @ingroup view
 *
 * This view allows users to select which virtual Controller Pak to use
 * when launching a game that supports Controller Pak saves.
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <libdragon.h>

#include "views.h"
#include "../sound.h"
#include "../fonts.h"
#include "../path.h"
#include "../virtual_cpak.h"
#include "../rom_info.h"
#include "utils/cpakfs_utils.h"


#define MAX_VISIBLE_ENTRIES 10
#define CREATE_NEW_INDEX    -1


static vcpak_list_t pak_list;  // pak_list.selected: -1 = "Create New", 0+ = pak entry index
static int scroll_offset;
static bool show_no_cpak_warning;
static bool show_delete_confirm;
static bool show_create_error;
static char create_error_message[128];
static char new_pak_filename[64];
static char new_pak_full_path[256];


static void reset_state(void) {
    scroll_offset = 0;
    show_no_cpak_warning = false;
    show_delete_confirm = false;
    show_create_error = false;
    create_error_message[0] = '\0';
    new_pak_filename[0] = '\0';
    new_pak_full_path[0] = '\0';
}

static void move_selection(int delta) {
    // Range: -1 (Create New) to pak_list.count-1
    int new_sel = pak_list.selected + delta;

    if (new_sel < CREATE_NEW_INDEX) {
        new_sel = pak_list.count - 1;  // Wrap to bottom
    } else if (new_sel >= pak_list.count) {
        new_sel = CREATE_NEW_INDEX;  // Wrap to top
    }

    pak_list.selected = new_sel;

    // Adjust scroll offset to keep selection visible
    // Account for "Create New" taking position 0 in display
    int display_pos = new_sel + 1;  // +1 because Create New is at 0
    if (display_pos < scroll_offset) {
        scroll_offset = display_pos;
    } else if (display_pos >= scroll_offset + MAX_VISIBLE_ENTRIES) {
        scroll_offset = display_pos - MAX_VISIBLE_ENTRIES + 1;
    }
}

static void process(menu_t *menu) {
    // Handle warning dialog
    if (show_no_cpak_warning) {
        if (menu->actions.enter || menu->actions.back) {
            sound_play_effect(SFX_ENTER);
            show_no_cpak_warning = false;
        }
        return;
    }

    // Handle create error dialog
    if (show_create_error) {
        if (menu->actions.enter || menu->actions.back) {
            sound_play_effect(SFX_EXIT);
            show_create_error = false;
        }
        return;
    }

    // Handle delete confirmation
    if (show_delete_confirm) {
        if (menu->actions.enter) {
            sound_play_effect(SFX_ENTER);
            // Delete the selected pak file
            int sel = pak_list.selected;
            if (sel >= 0 && sel < pak_list.count) {
                remove(pak_list.entries[sel].full_path);
                // Reload the list
                vcpak_list_free(&pak_list);
                vcpak_list_paks(menu->storage_prefix,
                               menu->load.rom_info.game_code,
                               menu->load.rom_info.settings.last_cpak_file,
                               &pak_list);
            }
            show_delete_confirm = false;
        } else if (menu->actions.back) {
            sound_play_effect(SFX_EXIT);
            show_delete_confirm = false;
        }
        return;
    }

    // Normal navigation
    if (menu->actions.go_up) {
        sound_play_effect(SFX_CURSOR);
        move_selection(-1);
    } else if (menu->actions.go_down) {
        sound_play_effect(SFX_CURSOR);
        move_selection(1);
    } else if (menu->actions.back) {
        sound_play_effect(SFX_EXIT);
        vcpak_list_free(&pak_list);
        menu->next_mode = MENU_MODE_LOAD_ROM;
    } else if (menu->actions.enter) {
        sound_play_effect(SFX_ENTER);

        int sel = pak_list.selected;

        if (sel == CREATE_NEW_INDEX) {
            // Create new pak
            // Generate filename
            vcpak_generate_filename(menu->storage_prefix,
                                   menu->load.rom_info.game_code,
                                   menu->load.rom_info.title,
                                   new_pak_filename,
                                   sizeof(new_pak_filename));

            // Build full path
            char game_dir[256];
            vcpak_get_game_directory(menu->storage_prefix,
                                     menu->load.rom_info.game_code,
                                     game_dir, sizeof(game_dir));
            path_t *full_path = path_create(game_dir);
            path_push(full_path, new_pak_filename);
            strncpy(new_pak_full_path, path_get(full_path), sizeof(new_pak_full_path) - 1);
            new_pak_full_path[sizeof(new_pak_full_path) - 1] = '\0';
            path_free(full_path);

            // Ensure directory exists
            vcpak_err_t dir_err = vcpak_ensure_game_directory(menu->storage_prefix,
                                        menu->load.rom_info.game_code);
            if (dir_err != VCPAK_OK) {
                show_create_error = true;
                snprintf(create_error_message, sizeof(create_error_message),
                         "Failed to create pak directory.\nError code: %d", dir_err);
                return;
            }

            // Create empty pak file
            vcpak_err_t create_err = vcpak_create_empty(new_pak_full_path);
            if (create_err != VCPAK_OK) {
                show_create_error = true;
                snprintf(create_error_message, sizeof(create_error_message),
                         "Failed to create pak file.\nError: %d", create_err);
                return;
            }

            // Set up load state
            menu->load.vcpak_enabled = true;
            strncpy(menu->load.vcpak_selected, new_pak_full_path,
                    sizeof(menu->load.vcpak_selected) - 1);
            menu->load.vcpak_selected[sizeof(menu->load.vcpak_selected) - 1] = '\0';

            // Save as last used
            rom_config_setting_set_last_cpak(menu->load.rom_path,
                                             &menu->load.rom_info,
                                             new_pak_filename);

            vcpak_list_free(&pak_list);
            menu->load_pending.rom_file = true;
            menu->next_mode = MENU_MODE_LOAD_ROM;
        } else if (sel >= 0 && sel < pak_list.count) {
            // Selected existing pak
            menu->load.vcpak_enabled = true;
            strncpy(menu->load.vcpak_selected,
                    pak_list.entries[sel].full_path,
                    sizeof(menu->load.vcpak_selected) - 1);

            // Save as last used
            rom_config_setting_set_last_cpak(menu->load.rom_path,
                                             &menu->load.rom_info,
                                             pak_list.entries[sel].filename);

            vcpak_list_free(&pak_list);
            menu->load_pending.rom_file = true;
            menu->next_mode = MENU_MODE_LOAD_ROM;
        }
    } else if (menu->actions.options) {
        // R button: Delete selected pak (if not "Create New")
        int sel = pak_list.selected;
        if (sel >= 0 && sel < pak_list.count) {
            sound_play_effect(SFX_SETTING);
            show_delete_confirm = true;
        }
    }
}

static void draw(menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    ui_components_background_draw();
    ui_components_layout_draw();

    // Title
    ui_components_main_text_draw(
        STL_DEFAULT,
        ALIGN_CENTER, VALIGN_TOP,
        "SELECT CONTROLLER PAK\n"
    );

    // Game info
    ui_components_main_text_draw(
        STL_GREEN,
        ALIGN_CENTER, VALIGN_TOP,
        "\n"
        "%.20s (%.4s)\n",
        menu->load.rom_info.title,
        menu->load.rom_info.game_code
    );

    // Draw pak list
    int y_offset = 4;  // Starting line for list

    // "Create New Pak" option - only show if not scrolled past
    if (scroll_offset == 0) {
        menu_font_type_t style = (pak_list.selected == CREATE_NEW_INDEX) ? STL_GREEN : STL_DEFAULT;
        char prefix = (pak_list.selected == CREATE_NEW_INDEX) ? '>' : ' ';

        ui_components_main_text_draw(
            style,
            ALIGN_LEFT, VALIGN_TOP,
            "\n\n\n\n"
            "%c [Create New Controller Pak]\n",
            prefix
        );
        y_offset++;
    }

    // Existing pak entries
    // When scroll_offset == 0: show paks 0 to MAX_VISIBLE_ENTRIES-2 (Create New takes one slot)
    // When scroll_offset > 0: show paks (scroll_offset-1) to (scroll_offset-1+MAX_VISIBLE_ENTRIES-1)
    int first_pak = (scroll_offset == 0) ? 0 : scroll_offset - 1;
    int max_visible_paks = (scroll_offset == 0) ? MAX_VISIBLE_ENTRIES - 1 : MAX_VISIBLE_ENTRIES;

    for (int i = 0; i < max_visible_paks; i++) {
        int entry_idx = first_pak + i;
        if (entry_idx >= pak_list.count) break;

        vcpak_entry_t *entry = &pak_list.entries[entry_idx];
        menu_font_type_t style = (pak_list.selected == entry_idx) ? STL_GREEN : STL_DEFAULT;
        char prefix = (pak_list.selected == entry_idx) ? '>' : ' ';
        const char *marker = entry->is_last_used ? " *" : "";

        // Build line number string for vertical positioning
        int num_newlines = y_offset + i + 4;
        char newlines[32];
        if (num_newlines > (int)(sizeof(newlines) - 1)) {
            num_newlines = sizeof(newlines) - 1;
        }
        memset(newlines, '\n', num_newlines);
        newlines[num_newlines] = '\0';

        ui_components_main_text_draw(
            style,
            ALIGN_LEFT, VALIGN_TOP,
            "%s%c %.50s%s\n",
            newlines,
            prefix,
            entry->filename,
            marker
        );
    }

    // Show count info
    if (pak_list.count > 0) {
        ui_components_main_text_draw(
            STL_GRAY,
            ALIGN_RIGHT, VALIGN_TOP,
            "\n\n\n"
            "%d pak(s) available\n",
            pak_list.count
        );
    }

    // No physical pak warning indicator
    if (menu->load.vcpak_no_physical) {
        ui_components_main_text_draw(
            STL_ORANGE,
            ALIGN_CENTER, VALIGN_TOP,
            "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
            "Warning: No Controller Pak in Port 1\n"
        );
    }

    // Actions bar
    ui_components_actions_bar_text_draw(
        STL_DEFAULT,
        ALIGN_LEFT, VALIGN_TOP,
        "A: Select\n"
        "B: Cancel\n"
    );

    if (pak_list.selected >= 0 && pak_list.selected < pak_list.count) {
        ui_components_actions_bar_text_draw(
            STL_DEFAULT,
            ALIGN_RIGHT, VALIGN_TOP,
            "R: Delete Pak\n"
            "* = Last Used\n"
        );
    } else {
        ui_components_actions_bar_text_draw(
            STL_DEFAULT,
            ALIGN_RIGHT, VALIGN_TOP,
            "\n"
            "* = Last Used\n"
        );
    }

    // Dialogs
    if (show_no_cpak_warning) {
        ui_components_messagebox_draw(
            "No Controller Pak detected in Port 1.\n\n"
            "The game will launch but Controller Pak\n"
            "saves will not work.\n\n"
            "Press A or B to continue."
        );
    }

    if (show_delete_confirm) {
        int sel = pak_list.selected;
        if (sel >= 0 && sel < pak_list.count) {
            ui_components_messagebox_draw(
                "Delete this Controller Pak file?\n\n"
                "%.50s\n\n"
                "A: Delete    B: Cancel",
                pak_list.entries[sel].filename
            );
        }
    }

    if (show_create_error) {
        ui_components_messagebox_draw(
            "ERROR: Could not create pak\n\n"
            "%s\n\n"
            "Press A or B to continue.",
            create_error_message
        );
    }

    rdpq_detach_show();
}


void view_vcpak_select_init(menu_t *menu) {
    reset_state();

    // Initialize vcpak load state
    menu->load.vcpak_enabled = false;
    menu->load.vcpak_selected[0] = '\0';
    menu->load.vcpak_no_physical = false;

    // Check for physical pak
    if (!has_cpak(0)) {
        menu->load.vcpak_no_physical = true;
        show_no_cpak_warning = true;
    }

    // Ensure game directory exists
    vcpak_ensure_game_directory(menu->storage_prefix,
                                 menu->load.rom_info.game_code);

    // Load list of available paks for this game
    vcpak_list_paks(menu->storage_prefix,
                    menu->load.rom_info.game_code,
                    menu->load.rom_info.settings.last_cpak_file,
                    &pak_list);

    // If no paks exist, default to "Create New"
    if (pak_list.count == 0) {
        pak_list.selected = CREATE_NEW_INDEX;
    }

    // Initialize scroll_offset so selected item is visible
    if (pak_list.selected >= 0) {
        int display_pos = pak_list.selected + 1;
        if (display_pos >= MAX_VISIBLE_ENTRIES) {
            scroll_offset = display_pos - MAX_VISIBLE_ENTRIES + 1;
        }
    }
}

void view_vcpak_select_display(menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}
