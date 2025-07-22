#include "../datel_codes.h"
#include "views.h"
#include "../sound.h"
#include "../ui_components/constants.h" // FIXME: remove when ui_component_value_editor can handle widths by itself.


static cheat_file_code_t *cheat_codes;
static short item_selected = 0;
static bool is_editing_mode_address = false;
static bool is_editing_mode_value = false;
static uint8_t editing_field_selected = 7; // 0-7 for 8 nibbles of the address or value (set to last nibble by default).

typedef struct {
    char slot_zero[2];
    char slot_one[2];
    char slot_two[2];
    char slot_three[2];
    char slot_four[2];
    char slot_five[2];
    char slot_six[2];
    char slot_seven[2];
} cheat_field_nibbles_t;

/**
 * Draws the cheat editor UI component.
 * 
 * @param address The address to display.
 * @param selected_field The field currently selected for editing.
 */
static void cheat_ui_component_edit_field_draw ( uint32_t address, int selected_field ) {
    cheat_field_nibbles_t nibbles;

    /* Format each nibble in address as hex */
    snprintf(nibbles.slot_zero, sizeof(nibbles.slot_zero), "%lX", (address >> 28) & 0xF);
    snprintf(nibbles.slot_one, sizeof(nibbles.slot_one), "%lX", (address >> 24) & 0xF);
    snprintf(nibbles.slot_two, sizeof(nibbles.slot_two), "%lX", (address >> 20) & 0xF);
    snprintf(nibbles.slot_three, sizeof(nibbles.slot_three), "%lX", (address >> 16) & 0xF);
    snprintf(nibbles.slot_four, sizeof(nibbles.slot_four), "%lX", (address >> 12) & 0xF);
    snprintf(nibbles.slot_five, sizeof(nibbles.slot_five), "%lX", (address >> 8) & 0xF);
    snprintf(nibbles.slot_six, sizeof(nibbles.slot_six), "%lX", (address >> 4) & 0xF);
    snprintf(nibbles.slot_seven, sizeof(nibbles.slot_seven), "%lX", address & 0xF);

    ui_component_value_editor(
        (const char *[]){
            "*", "*", "*", "*", "*", "*", "*", "*"
        },
        (const char *[]){
            nibbles.slot_zero, nibbles.slot_one, nibbles.slot_two, nibbles.slot_three, nibbles.slot_four, nibbles.slot_five, nibbles.slot_six, nibbles.slot_seven
        },
        8,
        selected_field,
        (VISIBLE_AREA_WIDTH - (TEXT_MARGIN_HORIZONTAL * 2)) / 20.0f
    );
}


static void toggle_enable_selected_cheat (menu_t *menu, void *arg) {
    debugf("Cheat Editor: Edit Selected Cheat toggle.\n");
    cheat_codes[item_selected].enabled = !cheat_codes[item_selected].enabled;
    set_cheat_codes(cheat_codes);
    sound_play_effect(SFX_SETTING);
}

static void edit_selected_cheat_address (menu_t *menu, void *arg) {
    debugf("Cheat Editor: Edit Selected Cheat address %08lX, edit not implemented.\n", cheat_codes[item_selected].address);
    editing_field_selected = 0; // Reset to the first nibble of the address.
    is_editing_mode_address = true;
}

static void edit_selected_cheat_value (menu_t *menu, void *arg) {
    debugf("Cheat Editor: Edit Selected Cheat value: %04X, edit not implemented.\n", cheat_codes[item_selected].value);
    editing_field_selected = 4; // Reset to the first nibble of the value.
    is_editing_mode_value = true;
}

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
            // adjust_selected_nibble( &cheat_field_nibble, +1 );
        }
        else if (menu->actions.go_down) {
            // adjust_selected_nibble( &cheat_field_nibble, -1 );
        }
        else if (menu->actions.enter) { // apply
            is_editing_mode_address = false;
            is_editing_mode_value = false;
        }
        else if (menu->actions.back) { // cancel
            is_editing_mode_address = false;
            is_editing_mode_value = false;
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
            set_cheat_codes(cheat_codes);
            menu->next_mode = MENU_MODE_LOAD_ROM;
            debugf("Cheat Editor: Applying cheats.\n");
            sound_play_effect(SFX_ENTER);
        } else if (menu->actions.back) {
            sound_play_effect(SFX_EXIT);
            menu->next_mode = MENU_MODE_BROWSER;
        } else if (menu->actions.options) {
            ui_components_context_menu_show(&options_context_menu);
            sound_play_effect(SFX_SETTING);
        }
    }
}

/**
 * @brief Draw the cheat list.
 * 
 * @param list Pointer to the list of entries.
 * @param entries Number of entries in the list.
 * @param selected Index of the currently selected entry.
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
            sprintf(str_buffer, "%02d: %08lX %04X - %s", 
                entry_index + 1, entry->address, entry->value, entry->description
            );

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
        "A: Apply ROM with these cheats\n"
        "B: Back"
    );

    ui_components_actions_bar_text_draw(
        STL_DEFAULT,
        ALIGN_RIGHT, VALIGN_TOP,
        "\n"
        "R: Item Options\n"
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

    rdpq_detach_show();
}

void view_datel_code_editor_init (menu_t *menu) {
    is_editing_mode_address = false;
    is_editing_mode_value = false;

    ui_components_context_menu_init(&options_context_menu);
    ui_components_context_menu_init(&cm_edit_selected_cheat);

    cheat_codes = get_cheat_codes();

    path_t *path = path_clone(menu->load.rom_path);
    path_ext_replace(path, "datel.txt");

    load_cheats_from_file(path_get(path));

    path_free(path);

}

void view_datel_code_editor_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);
}
