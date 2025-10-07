/**
 * @file datel_code_editor.c
 * @brief Datel code editor implementation
 * @author Robin Jones (networkfusion)
 * @ingroup views
 */

#include "../ui_components/constants.h"
#include "../datel_codes.h"
#include "../sound.h"
#include "views.h"


static cheat_file_code_t *cheat_codes;
static short item_selected = 0;
static bool is_editing_mode_address = false;
static bool is_editing_mode_value = false;
static uint8_t editing_field_selected = 7; // 0-7 for 8 nibbles of the address or value (set to last nibble by default).

static bool show_message_save_confirm = false;

/**
 * Render an 8-nibble hexadecimal editor for a 32-bit value and highlight one nibble.
 * 
 * @param value 32-bit value to render (most significant nibble shown first).
 * @param selected_field Index of the nibble to highlight: 0 = most significant nibble, 7 = least significant nibble.
 */
static void cheat_ui_component_edit_field_draw(uint32_t value, int selected_field) {
    char nibbles[8][2];

    // Format each nibble as a single hex digit
    for (int i = 0; i < 8; ++i) {
        snprintf(nibbles[i], sizeof(nibbles[i]), "%lX", (value >> (28 - i * 4)) & 0xF);
    }

    static const char *labels[8] = { "*", "*", "*", "*", "*", "*", "*", "*" };

    const char *nibble_ptrs[8];
    for (int i = 0; i < 8; ++i) {
        nibble_ptrs[i] = nibbles[i];
    }

    ui_component_value_editor(
        labels,
        nibble_ptrs,
        8,
        selected_field,
        20.0f
    );
}


/**
 * Toggle enabled state of the currently selected cheat entry.
 *
 * Persists the updated cheat list and plays the setting sound effect.
 */
static void toggle_enable_selected_cheat (menu_t *menu, void *arg) {
    debugf("Cheat Editor: Edit Selected Cheat toggle.\n");
    cheat_codes[item_selected].enabled = !cheat_codes[item_selected].enabled;
    set_cheat_codes(cheat_codes);
    sound_play_effect(SFX_SETTING);
}

/**
 * Enter address editing mode for the currently selected cheat entry.
 *
 * Prepares the UI to edit the cheat's 32-bit address by selecting the most-significant
 * nibble (first nibble) and enabling address-editing state.
 *
 * @param menu Current menu context (unused by this action).
 * @param arg Unused.
 */
static void edit_selected_cheat_address (menu_t *menu, void *arg) {
    debugf("Cheat Editor: Edit Selected Cheat address %08lX.\n", cheat_codes[item_selected].address);
    editing_field_selected = 0; // Reset to the first nibble of the address.
    is_editing_mode_address = true;
}

/**
 * Enter value-editing mode for the currently selected cheat.
 *
 * Resets the nibble selection to the first nibble of the cheat's value and enables
 * the value editing mode so subsequent input modifies the value field.
 *
 * @param arg Unused.
 */
static void edit_selected_cheat_value (menu_t *menu, void *arg) {
    debugf("Cheat Editor: Edit Selected Cheat value: %04X.\n", cheat_codes[item_selected].value);
    editing_field_selected = 4; // Reset to the first nibble of the value.
    is_editing_mode_value = true;
}

/**
 * Reset the currently selected cheat entry to an empty, disabled state and persist the change.
 *
 * Clears the cheat's address and value, empties its description, marks it disabled, and saves
 * the updated cheat list via set_cheat_codes().
 */
static void reset_selected_cheat (menu_t *menu, void *arg) {
    debugf("Cheat Editor: Reset Selected Cheat.\n");
    cheat_codes[item_selected].address = 0; // Reset the cheat address.
    cheat_codes[item_selected].value = 0; // Reset the cheat value.
    strncpy(cheat_codes[item_selected].description, "\0", sizeof(cheat_codes[item_selected].description));
    cheat_codes[item_selected].enabled = false;// Mark the cheat as disabled instead of deleting it.
    set_cheat_codes(cheat_codes);
}

static component_context_menu_t cm_edit_selected_cheat = { .list = {
    { .text = "Toggle Enabled", .action = toggle_enable_selected_cheat },
    { .text = "Edit Address", .action = edit_selected_cheat_address },
    { .text = "Edit Value", .action = edit_selected_cheat_value },
    COMPONENT_CONTEXT_MENU_LIST_END
}};

static component_context_menu_t options_context_menu = { .list = {
    { .text = "Edit Selected Item", .submenu = &cm_edit_selected_cheat },
    { .text = "Reset Selected Item", .action = reset_selected_cheat },
    COMPONENT_CONTEXT_MENU_LIST_END
}};




/**
 * Process input and update Datel code editor state.
 *
 * Handles the active context menu or, when not handled, processes either nibble-based
 * address/value editing or the normal list navigation and command actions. In editing
 * mode, left/right move the selected nibble, up/down increment or decrement the nibble,
 * enter finalizes editing (enables the cheat), and back cancels editing. When not editing,
 * up/down change the selected cheat, enter either saves to file (when confirmation is shown)
 * or applies cheats to the ROM, back cancels or exits to the previous view, options opens
 * the item options menu, and the L/Z action shows the save-confirmation prompt.
 *
 * @param menu Current menu context containing input action flags, load path, and next-mode field.
 */
static void process(menu_t *menu) {

    if (ui_components_context_menu_process(menu, &options_context_menu)) {
        return;
    }

    if (is_editing_mode_address || is_editing_mode_value) {
        if (menu->actions.go_left) {
            if ( editing_field_selected <= 0 ) { editing_field_selected = 7; }
            else { editing_field_selected = editing_field_selected - 1; }
        }
        else if (menu->actions.go_right) {
            if ( editing_field_selected >= 7 ) { editing_field_selected = 0; }
            else { editing_field_selected = editing_field_selected + 1; }
       
        }
        else if (menu->actions.go_up) {
            // Increment the selected nibble
            if (is_editing_mode_address) {
                // Edit the address field, one nibble at a time.
                uint32_t *addr = &cheat_codes[item_selected].address;
                // Calculate the bit position for the selected nibble (0 = leftmost/highest nibble)
                int shift = (7 - editing_field_selected) * 4;
                // Extract the current nibble value
                uint32_t nibble = (*addr >> shift) & 0xF;
                // Increment the nibble, wrapping around from 0xF to 0x0
                nibble = (nibble + 1) & 0xF;
                // Clear the old nibble and set the new value
                *addr = (*addr & ~(0xFu << shift)) | (nibble << shift);
            } else if (is_editing_mode_value) {
                // Edit the value field, one nibble at a time (only lower 4 nibbles for 16-bit value)
                uint16_t *val = &cheat_codes[item_selected].value;
                int shift = (7 - editing_field_selected) * 4;
                if (shift < 16) { // Only modify valid nibbles in the 16-bit value
                    // Extract the current nibble value
                    uint16_t nibble = (*val >> shift) & 0xF;
                    // Increment the nibble, wrapping around from 0xF to 0x0
                    nibble = (nibble + 1) & 0xF;
                    // Clear the old nibble and set the new value
                    *val = (*val & ~(0xFu << shift)) | (nibble << shift);
                }
            }
            sound_play_effect(SFX_CURSOR);
        }
        else if (menu->actions.go_down) {
            // Decrement the selected nibble
            if (is_editing_mode_address) {
                // Edit the address field, one nibble at a time.
                uint32_t *addr = &cheat_codes[item_selected].address;
                // Calculate the bit position for the selected nibble (0 = leftmost/highest nibble)
                int shift = (7 - editing_field_selected) * 4;
                // Extract the current nibble value
                uint32_t nibble = (*addr >> shift) & 0xF;
                // Decrement the nibble, wrapping around from 0x0 to 0xF
                nibble = (nibble - 1) & 0xF;
                // Clear the old nibble and set the new value
                *addr = (*addr & ~(0xFu << shift)) | (nibble << shift);
            } else if (is_editing_mode_value) {
                // Edit the value field, one nibble at a time (only lower 4 nibbles for 16-bit value)
                uint16_t *val = &cheat_codes[item_selected].value;
                int shift = (7 - editing_field_selected) * 4;
                if (shift < 16) { // Only modify valid nibbles in the 16-bit value
                    // Extract the current nibble value
                    uint16_t nibble = (*val >> shift) & 0xF;
                    // Decrement the nibble, wrapping around from 0x0 to 0xF
                    nibble = (nibble - 1) & 0xF;
                    // Clear the old nibble and set the new value
                    *val = (*val & ~(0xFu << shift)) | (nibble << shift);
                }
            }
            sound_play_effect(SFX_CURSOR);
        }
        else if (menu->actions.enter) { // finish editing and ensure enabled
            is_editing_mode_address = false;
            is_editing_mode_value = false;
            cheat_codes[item_selected].enabled = true;
            debugf("Cheat Editor: Edited and enabled selected Cheat %08lX %04X.\n", cheat_codes[item_selected].address, cheat_codes[item_selected].value);
        }
        else if (menu->actions.back) { // finish editing
            is_editing_mode_address = false;
            is_editing_mode_value = false;
            debugf("Cheat Editor: Edited selected Cheat %08lX %04X.\n", cheat_codes[item_selected].address, cheat_codes[item_selected].value);
        }
    } 
    else {
        if(menu->actions.go_down) {
            item_selected++;
            if (item_selected >= MAX_CHEAT_CODES) {
                item_selected = 0;
            }
            sound_play_effect(SFX_CURSOR);
        } else if(menu->actions.go_up) {
            item_selected--;
            if (item_selected < 0) {
                item_selected = MAX_CHEAT_CODES - 1;
            }
            sound_play_effect(SFX_CURSOR);
        } else if(menu->actions.enter) {
            if (show_message_save_confirm) {

                path_t *rom_datel_filepath = path_clone(menu->load.rom_path);
                path_ext_replace(rom_datel_filepath, "datel.txt");
                save_cheats_to_file(path_get(rom_datel_filepath));
                path_free(rom_datel_filepath);
                sound_play_effect(SFX_SETTING);
                show_message_save_confirm = false;
            }
            else {
                set_cheat_codes(cheat_codes);
                menu->next_mode = MENU_MODE_LOAD_ROM;
                debugf("Cheat Editor: Applying cheats.\n");
                sound_play_effect(SFX_ENTER);
            }
        } else if (menu->actions.back) {
            if (show_message_save_confirm) {
                show_message_save_confirm = false;
            }
            else {
                debugf("Cheat Editor: Cheats not saved or applied.\n");
                menu->next_mode = MENU_MODE_LOAD_ROM;
            }
            sound_play_effect(SFX_EXIT);
        } else if (menu->actions.options) {
            ui_components_context_menu_show(&options_context_menu);
            sound_play_effect(SFX_SETTING);
        } else if (menu->actions.lz_context) {
            debugf("Cheat Editor: Saving cheats to file.\n");
            show_message_save_confirm = true;
            sound_play_effect(SFX_SETTING);
        }
    }
}

/**
 * Render the cheat code list UI including an entry column, a right-hand ON/OFF column, a scrollbar, and a highlighted selection.
 *
 * Draws a paged view of up to LIST_ENTRIES entries from `list`, renders a vertical scrollbar, highlights the `selected` row, and shows each entry's index, address, value, optional description, and enabled status.
 *
 * @param list Pointer to an array of cheat_file_code_t entries to display.
 * @param entries Number of valid entries in `list`.
 * @param selected Index of the currently selected entry within `list`.
 */
void cheat_code_list_draw (cheat_file_code_t *list, int entries, int selected) {
    int starting_position = 0;

    if (entries > LIST_ENTRIES && selected >= (LIST_ENTRIES / 2)) {
        starting_position = selected - (LIST_ENTRIES / 2);
        if (starting_position >= entries - LIST_ENTRIES) {
            starting_position = entries - LIST_ENTRIES;
        }
    }

    ui_components_list_scrollbar_draw(selected, entries, LIST_ENTRIES);

    if (entries == 0) {
        ui_components_main_text_draw(
            STL_RED,
            ALIGN_LEFT, VALIGN_TOP,
            "\n"
            "^%02X** no cheats found **"
        );
    } else {
        rdpq_paragraph_t *cheat_list_layout;
        rdpq_paragraph_t *layout;

        size_t cheat_string_lengths[LIST_ENTRIES];
        size_t total_length = 1;

        for (int i = 0; i < LIST_ENTRIES; i++) {
            int entry_index = starting_position + i;

            if (entry_index >= entries) {
                cheat_string_lengths[i] = 0;
            } else {
                cheat_string_lengths[i] = 64; //length;
                total_length += cheat_string_lengths[i];
            }
        }

        cheat_list_layout = malloc(sizeof(rdpq_paragraph_t) + (sizeof(rdpq_paragraph_char_t) * total_length));
        memset(cheat_list_layout, 0, sizeof(rdpq_paragraph_t));
        cheat_list_layout->capacity = total_length;

        rdpq_paragraph_builder_begin(
            &(rdpq_textparms_t) {
                .width = FILE_LIST_MAX_WIDTH + 40 - (TEXT_MARGIN_HORIZONTAL * 2),
                .height = LAYOUT_ACTIONS_SEPARATOR_Y - VISIBLE_AREA_Y0  - (TEXT_MARGIN_VERTICAL * 2),
                .wrap = WRAP_ELLIPSES,
                .line_spacing = TEXT_LINE_SPACING_ADJUST,
            },
            FNT_DEFAULT,
            cheat_list_layout
        );

        for (int i = 0; i < LIST_ENTRIES; i++) {
            int entry_index = starting_position + i;

            cheat_file_code_t *entry = &list[entry_index];

            menu_font_style_t style;

            style = entry->enabled ? STL_GREEN : STL_RED;

            rdpq_paragraph_builder_style(style);

            char str_buffer[64];
            if (entry->description[0] != '\0') {
                snprintf(str_buffer, sizeof(str_buffer), "%02d: %08lX %04X - %s",
                    entry_index + 1, entry->address, entry->value, entry->description
                );
            } else {
                snprintf(str_buffer, sizeof(str_buffer), "%02d: %08lX %04X",
                    entry_index + 1, entry->address, entry->value
                );
            }

            rdpq_paragraph_builder_span(str_buffer, strlen(str_buffer));

            if ((entry_index + 1) >= entries) {
                break;
            }

            rdpq_paragraph_builder_newline();
        }

        layout = rdpq_paragraph_builder_end();

        int highlight_height = (layout->bbox.y1 - layout->bbox.y0) / layout->nlines;
        int highlight_y = VISIBLE_AREA_Y0 + TEXT_MARGIN_VERTICAL + TAB_HEIGHT + TEXT_OFFSET_VERTICAL + ((selected - starting_position) * highlight_height);

        ui_components_box_draw(
            FILE_LIST_HIGHLIGHT_X,
            highlight_y,
            FILE_LIST_HIGHLIGHT_X + FILE_LIST_HIGHLIGHT_WIDTH,
            highlight_y + highlight_height,
            FILE_LIST_HIGHLIGHT_COLOR
        );

        rdpq_paragraph_render(
            layout,
            VISIBLE_AREA_X0 + TEXT_MARGIN_HORIZONTAL,
            VISIBLE_AREA_Y0 + TEXT_MARGIN_VERTICAL + TAB_HEIGHT + TEXT_OFFSET_VERTICAL
        );

        rdpq_paragraph_free(layout);

        rdpq_paragraph_builder_begin(
            &(rdpq_textparms_t) {
                .width = VISIBLE_AREA_WIDTH - LIST_SCROLLBAR_WIDTH - (TEXT_MARGIN_HORIZONTAL * 2),
                .height = LAYOUT_ACTIONS_SEPARATOR_Y - VISIBLE_AREA_Y0  - (TEXT_MARGIN_VERTICAL * 2),
                .align = ALIGN_RIGHT,
                .wrap = WRAP_NONE,
                .line_spacing = TEXT_LINE_SPACING_ADJUST,
            },
            FNT_DEFAULT,
            NULL
        );

        for (int i = starting_position; i < entries; i++) {
            cheat_file_code_t *entry = &list[i];

            menu_font_style_t style;

            style = entry->enabled ? STL_GREEN : STL_RED;

            rdpq_paragraph_builder_style(style);

            char str_enabled_buffer[4];
            sprintf(str_enabled_buffer, "%s",
                entry->enabled ? "ON" : "OFF"
            );

            rdpq_paragraph_builder_span(str_enabled_buffer, strlen(str_enabled_buffer));

            if ((i + 1) == (starting_position + LIST_ENTRIES)) {
                break;
            }

            rdpq_paragraph_builder_newline();
        }

        layout = rdpq_paragraph_builder_end();

        rdpq_paragraph_render(
            layout,
            VISIBLE_AREA_X0 + TEXT_MARGIN_HORIZONTAL,
            VISIBLE_AREA_Y0 + TEXT_MARGIN_VERTICAL + TAB_HEIGHT + TEXT_OFFSET_VERTICAL
        );

        rdpq_paragraph_free(layout);
    }
}

/**
 * Render the Datel Code Editor view onto the given display.
 *
 * Draws the view background and chrome, the centered title, the cheat list with its scrollbar
 * and selection highlight, left/right action bars, and the options context menu. If address
 * or value editing modes are active, renders the corresponding nibble editor for the
 * currently selected cheat. If a save-confirmation is active, renders the confirmation
 * message box.
 *
 * @param menu Pointer to the menu state containing selection and editing flags.
 * @param display Rendering target surface where the view is drawn.
 */
static void draw (menu_t *menu, surface_t *display) {
    rdpq_attach(display, NULL);

    ui_components_background_draw();

    ui_components_layout_draw();

    ui_components_main_text_draw(
        STL_DEFAULT,
        ALIGN_CENTER, VALIGN_TOP,
        "DATEL CODE EDITOR\n"
    );

    // ui_components_main_text_draw(
    //     STL_DEFAULT,
    //     ALIGN_LEFT, VALIGN_TOP,
    //     "\n"
    //     "ID  Address  Value  Description\n"
    // );

    cheat_code_list_draw(cheat_codes, MAX_CHEAT_CODES, item_selected);

    ui_components_actions_bar_text_draw(
        STL_DEFAULT,
        ALIGN_LEFT, VALIGN_TOP,
        "A: Apply to ROM\n"
        "B: Back"
    );

    ui_components_actions_bar_text_draw(
        STL_DEFAULT,
        ALIGN_RIGHT, VALIGN_TOP,
        "L|Z: Save changes\n"
        "R: Item options\n"
    );

    ui_components_context_menu_draw(&options_context_menu);

    if (is_editing_mode_address) {
        cheat_ui_component_edit_field_draw(
            cheat_codes[item_selected].address,
            editing_field_selected
        );
    }

    if (is_editing_mode_value) {
        cheat_ui_component_edit_field_draw(
            cheat_codes[item_selected].value,
            editing_field_selected
        );
    }

    if (show_message_save_confirm) {
        ui_components_messagebox_draw(
            "Overwrite file?\n\n"
            "A: Yes, B: No"
        );
    }

    rdpq_detach_show();
}

/**
 * Initialize the Datel code editor view and load cheats for the current ROM.
 *
 * Initializes editing state and context menus, obtains the global cheat list,
 * constructs a Datel cheats file path from menu->load.rom_path, and loads cheats
 * from that file into the editor state.
 *
 * @param menu Pointer to the menu instance containing the current ROM path used
 *             to locate the Datel cheats file.
 */
void view_datel_code_editor_init (menu_t *menu) {
    is_editing_mode_address = false;
    is_editing_mode_value = false;

    ui_components_context_menu_init(&options_context_menu);
    ui_components_context_menu_init(&cm_edit_selected_cheat);

    cheat_codes = get_cheat_codes();

    path_t *rom_datel_filepath = path_clone(menu->load.rom_path);
    path_ext_replace(rom_datel_filepath, "datel.txt");

    load_cheats_from_file(path_get(rom_datel_filepath));

    path_free(rom_datel_filepath);

}

/**
 * Process input/state changes and render the Datel code editor view.
 *
 * Executes the view's input processing (state transitions, editing, menus) and then draws the updated UI onto the given surface.
 * @param menu Pointer to the menu state for the Datel code editor.
 * @param display Surface to render the view onto.
 */
void view_datel_code_editor_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);
}