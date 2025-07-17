#include "../datel_codes.h"
#include "views.h"
#include "../sound.h"
#include "../ui_components/constants.h"


static cheat_file_code_t *cheat_codes;
static short item_selected = 0;


static void toggle_enable_selected_cheat (menu_t *menu, void *arg) {
    debugf("Cheat Editor: Edit Selected Cheat toggle.\n");
    cheat_codes[item_selected].enabled = !cheat_codes[item_selected].enabled;
    sound_play_effect(SFX_SETTING);
}

static void edit_selected_cheat_address (menu_t *menu, void *arg) {
    debugf("Cheat Editor: Edit Selected Cheat address not implemented yet.\n");
}

static void edit_selected_cheat_value (menu_t *menu, void *arg) {
    debugf("Cheat Editor: Edit Selected Cheat value not implemented yet.\n");
}

// static void add_new_cheat (menu_t *menu, void *arg) {
//     debugf("Cheat Editor: Add New Cheat not implemented yet.\n");
// }

static void reset_selected_cheat (menu_t *menu, void *arg) {
    debugf("Cheat Editor: Reset Selected Cheat.\n");
    cheat_codes[item_selected].address = 0; // Reset the cheat address.
    cheat_codes[item_selected].value = 0; // Reset the cheat value.
    //cheat_codes[item_selected].description[0] = '\0'; // Clear the cheat description.
    cheat_codes[item_selected].enabled = !cheat_codes[item_selected].enabled;// Mark the cheat as disabled instead of deleting it.
}

static component_context_menu_t cm_edit_selected_cheat = { .list = {
    { .text = "Toggle Enabled", .action = toggle_enable_selected_cheat },
    { .text = "Edit Address", .action = edit_selected_cheat_address },
    { .text = "Edit Value", .action = edit_selected_cheat_value },
    COMPONENT_CONTEXT_MENU_LIST_END
}};

static component_context_menu_t options_context_menu = { .list = {
    { .text = "Edit Selected Item", .submenu = &cm_edit_selected_cheat },
    //{ .text = "Add New Cheat Item", .action = add_new_cheat },
    { .text = "Reset Selected Item", .action = reset_selected_cheat },
    COMPONENT_CONTEXT_MENU_LIST_END
}};




static void process(menu_t *menu) {

    if (ui_components_context_menu_process(menu, &options_context_menu)) {
        return;
    }

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
        //menu->next_mode = MENU_MODE_LOAD_ROM;
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
                .width = FILE_LIST_MAX_WIDTH - (TEXT_MARGIN_HORIZONTAL * 2),
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
            sprintf(str_buffer, "%02d: 0x%08lx 0x%04x - %s", 
                entry_index, entry->address, entry->value, entry->description
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

    ui_components_main_text_draw( // TODO: add header "Idx | Address | Value | Enabled\n"
        STL_DEFAULT,
        ALIGN_CENTER, VALIGN_TOP,
        "DATEL CODE EDITOR\n"
    );

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

    rdpq_detach_show();
}

void view_datel_code_editor_init (menu_t *menu) {
    ui_components_context_menu_init(&options_context_menu);
    ui_components_context_menu_init(&cm_edit_selected_cheat);

    cheat_codes = get_cheat_codes();

    // But we should be loading the cheat codes from a file here.
    // Currently we are just going to pre populate them for test purposes.
    debugf("Cheat Editor: Init debug codes MM USA.\n");
    // Enable code
    cheat_codes[0].address = 4043925536; // Hex 0xF1096820
    cheat_codes[0].value = 9216; // Hex 0x2400.
    strncpy(cheat_codes[0].description, "Enable code 1", sizeof(cheat_codes[0].description) - 1);
    cheat_codes[0].description[sizeof(cheat_codes[0].description) - 1] = '\0'; // Ensure null-termination
    cheat_codes[0].enabled = true;

    cheat_codes[1].address = 4278190624; // Hex 0xFF000220.
    cheat_codes[1].value = 0; // Hex 0x0000.
    strncpy(cheat_codes[1].description, "Enable code 2", sizeof(cheat_codes[1].description) - 1);
    cheat_codes[1].description[sizeof(cheat_codes[1].description) - 1] = '\0'; // Ensure null-termination
    cheat_codes[1].enabled = true;

    // Inventory Editor (assigned to L)
    cheat_codes[2].address = 3491732369; // Hex 0xD01F9B91.
    cheat_codes[2].value = 32; // Hex 0x0020.
    strncpy(cheat_codes[2].description, "Inventory Editor 1", sizeof(cheat_codes[2].description) - 1);
    cheat_codes[2].description[sizeof(cheat_codes[2].description) - 1] = '\0'; // Ensure null-termination
    cheat_codes[2].enabled = true;

    cheat_codes[3].address = 2151668287; // Hex 0x803FDA3F.
    cheat_codes[3].value = 2; // Hex 0x0002.
    strncpy(cheat_codes[3].description, "Inventory Editor 2", sizeof(cheat_codes[3].description) - 1);
    cheat_codes[3].description[sizeof(cheat_codes[3].description) - 1] = '\0'; // Ensure null-termination
    cheat_codes[3].enabled = true;

    // Complete Bomber's Notebook
    cheat_codes[4].address = 0x811F05AA; // Hex 0x811F05AA.
    cheat_codes[4].value = 0xffff; // Hex 0xffff.
    strncpy(cheat_codes[4].description, "Complete Bomber's Notebook 1", sizeof(cheat_codes[4].description) - 1);
    cheat_codes[4].description[sizeof(cheat_codes[4].description) - 1] = '\0'; // Ensure null-termination
    cheat_codes[4].enabled = true;

    cheat_codes[5].address = 0x811F05AC; // Hex 0x811F05AC.
    cheat_codes[5].value = 0xffff; // Hex 0xffff.
    strncpy(cheat_codes[5].description, "Complete Bomber's Notebook 2", sizeof(cheat_codes[5].description) - 1);
    cheat_codes[5].description[sizeof(cheat_codes[5].description) - 1] = '\0'; // Ensure null-termination
    cheat_codes[5].enabled = true;

    cheat_codes[6].address = 0x811F05AE; // Hex 0x811F05AE.
    cheat_codes[6].value = 0xffff; // Hex 0xffff.
    strncpy(cheat_codes[6].description, "Complete Bomber's Notebook 3", sizeof(cheat_codes[6].description) - 1);
    cheat_codes[6].description[sizeof(cheat_codes[6].description) - 1] = '\0'; // Ensure null-termination
    cheat_codes[6].enabled = true;

    cheat_codes[7].address = 0x811F05B0; // Hex 0x811F05B0.
    cheat_codes[7].value = 0xffff; // Hex 0xffff.
    strncpy(cheat_codes[7].description, "Complete Bomber's Notebook 4", sizeof(cheat_codes[7].description) - 1);
    cheat_codes[7].description[sizeof(cheat_codes[7].description) - 1] = '\0'; // Ensure null-termination
    cheat_codes[7].enabled = true;

    set_cheat_codes(cheat_codes);
    uint32_t cheats[MAX_CHEAT_CODE_ARRAYLIST_SIZE];
    size_t cheat_item_count = generate_enabled_cheats_array(get_cheat_codes(), cheats);
    // cheats[] now contains address/value pairs for enabled cheats, ending with two zeros.
    debugf("Cheat Editor: Generated %u cheat items.\n", cheat_item_count);

}

void view_datel_code_editor_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);
}
