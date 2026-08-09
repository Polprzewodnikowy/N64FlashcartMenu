#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../sound.h"
#include "../settings.h"
#include "../ui_components/constants.h"
#include "views.h"

static bool show_message_reset_settings = false;
static bool show_pal60_confirm_message = false;
static bool pal60_confirm_waiting_for_input = false;
static int pal60_confirm_started_at = 0;

static const char *tabs[] = {
    "Menu",
    "Video",
    "Sound",
    NULL
};

typedef enum {
    SETTINGS_TAB_MENU,
    SETTINGS_TAB_VIDEO,
    SETTINGS_TAB_SOUND,
} settings_tab_context_t;

static settings_tab_context_t selected_tab = SETTINGS_TAB_MENU;
static int selected_menu_row = 0;
static int selected_video_row = 0;
static int selected_sound_row = 0;

static int *selected_row_ptrs[] = {
    &selected_menu_row,
    &selected_video_row,
    &selected_sound_row,
};

#ifdef FEATURE_AUTOLOAD_ROM_ENABLED
static void set_loading_progress_bar_enabled_type (menu_t *menu, void *arg) {
    menu->settings.loading_progress_bar_enabled = (bool)(uintptr_t)(arg);
    settings_save(&menu->settings);
}
#endif

static void set_protected_entries_type (menu_t *menu, void *arg) {
    menu->settings.show_protected_entries = (bool)(uintptr_t)(arg);
    settings_save(&menu->settings);

    menu->browser.reload = true;
}

static void set_use_saves_folder_type (menu_t *menu, void *arg) {
    menu->settings.use_saves_folder = (bool)(uintptr_t)(arg);
    settings_save(&menu->settings);
}

static void set_show_saves_folder_type (menu_t *menu, void *arg) {
    menu->settings.show_saves_folder = (bool)(uintptr_t)(arg);
    settings_save(&menu->settings);

    menu->browser.reload = true;
}

static void set_show_save_files_type (menu_t *menu, void *arg) {
    menu->settings.show_save_files = (bool)(uintptr_t)(arg);
    settings_save(&menu->settings);

    menu->browser.reload = true;
}

static void set_show_cheat_files_type (menu_t *menu, void *arg) {
    menu->settings.show_cheat_files = (bool)(uintptr_t)(arg);
    settings_save(&menu->settings);

    menu->browser.reload = true;
}
    
static void set_soundfx_enabled_type (menu_t *menu, void *arg) {
    menu->settings.soundfx_enabled = (bool)(uintptr_t)(arg);
    sound_use_sfx(menu->settings.soundfx_enabled);
    settings_save(&menu->settings);
}

static void set_bgm_enabled_type (menu_t *menu, void *arg) {
    menu->settings.bgm_enabled = (bool)(uintptr_t)(arg);
    sound_use_bgm(menu->settings.bgm_enabled);
    settings_save(&menu->settings);
}

static void set_pal60_type (menu_t *menu, void *arg) {
    bool pal60_try_enable = (bool)(uintptr_t)(arg);
    tv_type_t tv_type = get_tv_type();

    if (pal60_try_enable && (tv_type == TV_PAL)) {
        show_pal60_confirm_message = true;
        pal60_confirm_waiting_for_input = false;
        pal60_confirm_started_at = get_ticks();
        vi_set_timing_preset(&VI_TIMING_PAL60);
        menu->settings.pal60_enabled = true;
        return;
    }

    if (!pal60_try_enable && (tv_type == TV_PAL)) {
        vi_set_timing_preset(&VI_TIMING_PAL);
    } else {
        pal60_try_enable = false;
    }

    menu->settings.pal60_enabled = pal60_try_enable;
    settings_save(&menu->settings);
}

static void set_wrap_file_list_scrolling_type (menu_t *menu, void *arg) {
    menu->settings.wrap_file_list_scrolling = (bool)(uintptr_t)(arg);
    settings_save(&menu->settings);
}

static void set_force_progressive_scan_type (menu_t *menu, void *arg) {
    menu->settings.force_progressive_scan = (bool)(uintptr_t)(arg);
    settings_save(&menu->settings);
}

#ifndef FEATURE_AUTOLOAD_ROM_ENABLED
static void set_use_rom_fast_reboot_enabled_type (menu_t *menu, void *arg) {
    menu->settings.rom_fast_reboot_enabled = (bool)(uintptr_t)(arg);
    settings_save(&menu->settings);
}
#endif

#ifdef BETA_SETTINGS
static void set_show_browser_file_extensions_type(menu_t *menu, void *arg) {
    menu->settings.show_browser_file_extensions = (bool)(uintptr_t)(arg);
    settings_save(&menu->settings);
    menu->browser.reload = true;
}

static void set_show_browser_rom_tags_type (menu_t *menu, void *arg) {
    menu->settings.show_browser_rom_tags = (bool)(uintptr_t)(arg);
    settings_save(&menu->settings);
}

static void set_rumble_enabled_type (menu_t *menu, void *arg) {
    menu->settings.rumble_enabled = (bool)(uintptr_t)(arg);
    settings_save(&menu->settings);
}

#endif

static void remove_background_image (menu_t *menu, void *arg) {
    (void)arg;
    (void)menu;
    ui_components_background_clear();
}

/* ------------------------------------------------------------------ */
/* Table-driven settings: each entry describes how to display its     */
/* current value and how to advance/cycle to the next value.          */
/* This supports boolean toggles (On/Off) as well as multi-value      */
/* options (e.g. language selection) by providing custom handlers.    */
/* ------------------------------------------------------------------ */

typedef void (*setting_toggle_fn)(menu_t *menu, void *arg);

typedef struct setting_entry setting_entry_t;

/* Returns the current value as a string, or NULL for action rows. */
typedef const char *(*setting_get_value_fn)(menu_t *menu, const setting_entry_t *entry);
/* Advances/cycles the setting to its next value. */
typedef void (*setting_cycle_fn)(menu_t *menu, const setting_entry_t *entry);

struct setting_entry {
    settings_tab_context_t tab;
    const char *label;
    bool is_action;     /* action row: no value shown, cycle runs a one-shot action */
    bool is_video;      /* video row with dynamic PAL60/Progressive label */
    setting_get_value_fn get_value;
    setting_cycle_fn cycle;
    size_t offset;      /* offsetof into settings_t for a bool field (bool handlers)
                           or an int field (multi-value handlers) */
    setting_toggle_fn setter; /* setter called with the new bool value (bool handlers) */
    const char *const *values; /* NULL-terminated display names for multi-value options */
};

/* --- Generic boolean handlers (On/Off) --- */
static const char *bool_get_value (menu_t *menu, const setting_entry_t *entry) {
    bool *field = (bool *)((char *)&menu->settings + entry->offset);
    return *field ? "On" : "Off";
}

static void bool_cycle (menu_t *menu, const setting_entry_t *entry) {
    bool *field = (bool *)((char *)&menu->settings + entry->offset);
    entry->setter(menu, (void *)(uintptr_t)(!(*field)));
}

/* --- Video row (PAL60 / Progressive Scan depending on tv_type) --- */
static const char *video_get_value (menu_t *menu, const setting_entry_t *entry) {
    (void)entry;
    if (get_tv_type() == TV_PAL) {
        return menu->settings.pal60_enabled ? "On" : "Off";
    }
    return menu->settings.force_progressive_scan ? "On" : "Off";
}

static void video_cycle (menu_t *menu, const setting_entry_t *entry) {
    (void)entry;
    if (get_tv_type() == TV_PAL) {
        set_pal60_type(menu, (void *)(uintptr_t)(!menu->settings.pal60_enabled));
    } else {
        set_force_progressive_scan_type(menu, (void *)(uintptr_t)(!menu->settings.force_progressive_scan));
    }
}

/* --- Action row (one-shot, e.g. Remove Background Image) --- */
static const char *action_get_value (menu_t *menu, const setting_entry_t *entry) {
    (void)menu;
    (void)entry;
    return NULL;
}

static void action_cycle (menu_t *menu, const setting_entry_t *entry) {
    (void)entry;
    remove_background_image(menu, NULL);
}

static const setting_entry_t settings_table[] = {
    /* Menu tab */
    { SETTINGS_TAB_MENU, "Show Hidden Files", false, false, bool_get_value, bool_cycle, offsetof(settings_t, show_protected_entries), set_protected_entries_type, NULL },
    { SETTINGS_TAB_MENU, "Use Saves Folder", false, false, bool_get_value, bool_cycle, offsetof(settings_t, use_saves_folder), set_use_saves_folder_type, NULL },
    { SETTINGS_TAB_MENU, "Show Saves Folder", false, false, bool_get_value, bool_cycle, offsetof(settings_t, show_saves_folder), set_show_saves_folder_type, NULL },
    { SETTINGS_TAB_MENU, "Show Save Files", false, false, bool_get_value, bool_cycle, offsetof(settings_t, show_save_files), set_show_save_files_type, NULL },
    { SETTINGS_TAB_MENU, "Show Cheat Files", false, false, bool_get_value, bool_cycle, offsetof(settings_t, show_cheat_files), set_show_cheat_files_type, NULL },
    { SETTINGS_TAB_MENU, "Wrap File List", false, false, bool_get_value, bool_cycle, offsetof(settings_t, wrap_file_list_scrolling), set_wrap_file_list_scrolling_type, NULL },
#ifdef FEATURE_AUTOLOAD_ROM_ENABLED
    { SETTINGS_TAB_MENU, "ROM Loading Bar", false, false, bool_get_value, bool_cycle, offsetof(settings_t, loading_progress_bar_enabled), set_loading_progress_bar_enabled_type, NULL },
#else
    { SETTINGS_TAB_MENU, "Fast Reboot ROM", false, false, bool_get_value, bool_cycle, offsetof(settings_t, rom_fast_reboot_enabled), set_use_rom_fast_reboot_enabled_type, NULL },
#endif
#ifdef BETA_SETTINGS
    { SETTINGS_TAB_MENU, "Hide ROM Extensions", false, false, bool_get_value, bool_cycle, offsetof(settings_t, show_browser_file_extensions), set_show_browser_file_extensions_type, NULL },
    { SETTINGS_TAB_MENU, "Hide ROM Tags", false, false, bool_get_value, bool_cycle, offsetof(settings_t, show_browser_rom_tags), set_show_browser_rom_tags_type, NULL },
    { SETTINGS_TAB_MENU, "Rumble Feedback", false, false, bool_get_value, bool_cycle, offsetof(settings_t, rumble_enabled), set_rumble_enabled_type, NULL },
#endif
    { SETTINGS_TAB_MENU, "Remove Background Image", true, false, action_get_value, action_cycle, 0, NULL, NULL },

    /* Video tab */
    { SETTINGS_TAB_VIDEO, NULL, false, true, video_get_value, video_cycle, 0, NULL, NULL },

    /* Sound tab */
    { SETTINGS_TAB_SOUND, "Sound Effects", false, false, bool_get_value, bool_cycle, offsetof(settings_t, soundfx_enabled), set_soundfx_enabled_type, NULL },
    { SETTINGS_TAB_SOUND, "Background Music", false, false, bool_get_value, bool_cycle, offsetof(settings_t, bgm_enabled), set_bgm_enabled_type, NULL },
};

static int tab_row_count (settings_tab_context_t tab) {
    int count = 0;
    for (size_t i = 0; i < (sizeof(settings_table) / sizeof(settings_table[0])); i++) {
        if (settings_table[i].tab == tab) {
            count++;
        }
    }
    return count;
}

static const setting_entry_t *find_entry (settings_tab_context_t tab, int row) {
    int current_row = 0;
    for (size_t i = 0; i < (sizeof(settings_table) / sizeof(settings_table[0])); i++) {
        if (settings_table[i].tab != tab) {
            continue;
        }
        if (current_row == row) {
            return &settings_table[i];
        }
        current_row++;
    }
    return NULL;
}

static void set_selected_row_for_current_tab (int row) {
    int row_count = tab_row_count(selected_tab);
    if (row_count > 0) {
        row = (row % row_count + row_count) % row_count;
    }
    *selected_row_ptrs[selected_tab] = row;
}

static void change_selected_row (int selected) {
    set_selected_row_for_current_tab(*selected_row_ptrs[selected_tab] + selected);
}

static void toggle_selected_setting (menu_t *menu) {
    int row = *selected_row_ptrs[selected_tab];
    const setting_entry_t *entry = find_entry(selected_tab, row);
    if (entry != NULL) {
        entry->cycle(menu, entry);
    }
}

static bool get_setting_row_text (menu_t *menu, int row, char *buffer, size_t buffer_size) {
    const setting_entry_t *entry = find_entry(selected_tab, row);
    if (entry == NULL) {
        return false;
    }

    const char *label = entry->label;
    if (entry->is_video) {
        label = (get_tv_type() == TV_PAL) ? "PAL60 Mode" : "Progressive Scan";
    }

    const char *value = entry->get_value(menu, entry);
    if (value != NULL) {
        snprintf(buffer, buffer_size, "%s: %s", label, value);
    } else {
        snprintf(buffer, buffer_size, "%s", label);
    }

    return true;
}

static void process (menu_t *menu) {
    if (show_pal60_confirm_message) {
        if (!pal60_confirm_waiting_for_input) {
            pal60_confirm_waiting_for_input = true;
            return;
        }

        if ((get_ticks() - pal60_confirm_started_at) >= (5 * TICKS_PER_SECOND)) {
            vi_set_timing_preset(&VI_TIMING_PAL);
            menu->settings.pal60_enabled = false;
            settings_save(&menu->settings);
            show_pal60_confirm_message = false;
            pal60_confirm_waiting_for_input = false;
            sound_play_effect(SFX_EXIT);
            return;
        }

        if (menu->actions.enter) {
            menu->settings.pal60_enabled = true;
            settings_save(&menu->settings);
            show_pal60_confirm_message = false;
            pal60_confirm_waiting_for_input = false;
            sound_play_effect(SFX_SETTING);
        } else if (menu->actions.back) {
            vi_set_timing_preset(&VI_TIMING_PAL);
            menu->settings.pal60_enabled = false;
            settings_save(&menu->settings);
            show_pal60_confirm_message = false;
            pal60_confirm_waiting_for_input = false;
            sound_play_effect(SFX_EXIT);
        }
        return;
    }

    if (menu->actions.go_up) {
        change_selected_row(-1);
        sound_play_effect(SFX_CURSOR);
    } else if (menu->actions.go_down) {
        change_selected_row(1);
        sound_play_effect(SFX_CURSOR);
    } else if (menu->actions.go_left) {
        if (selected_tab == SETTINGS_TAB_MENU) {
            selected_tab = SETTINGS_TAB_SOUND;
        } else {
            selected_tab--;
        }
        sound_play_effect(SFX_CURSOR);
    } else if (menu->actions.go_right) {
        if (selected_tab == SETTINGS_TAB_SOUND) {
            selected_tab = SETTINGS_TAB_MENU;
        } else {
            selected_tab++;
        }
        sound_play_effect(SFX_CURSOR);
    } else if (menu->actions.enter) {
        if (show_message_reset_settings) {
            settings_reset_to_defaults();
            menu_show_error(menu, "Reboot N64 to take effect!");
            show_message_reset_settings = false;
        } else {
            toggle_selected_setting(menu);
        }
        sound_play_effect(SFX_SETTING);
    } else if (menu->actions.back) {
        if (show_message_reset_settings) {
            show_message_reset_settings = false;
        } else {
            menu->next_mode = MENU_MODE_BROWSER;
        }
        sound_play_effect(SFX_EXIT);
    } else if (menu->actions.options){
        show_message_reset_settings = true;
    }
}

static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    ui_components_background_draw();

    ui_components_tabs_common_draw(tabs, selected_tab);

    ui_components_layout_draw_tabbed();

    ui_components_main_text_draw(
        STL_DEFAULT,
        ALIGN_CENTER, VALIGN_TOP,
        "\n"
        "MENU SETTINGS EDITOR\n"
        "\n",
        menu->settings.default_directory
    );

    if (selected_tab == SETTINGS_TAB_MENU) {
        ui_components_main_text_draw(
            STL_DEFAULT,
            ALIGN_LEFT, VALIGN_TOP,
            "\n"
            "\n"
            "\n"
            "Default Directory : %s\n"
            "\n"
            "To change the following menu settings, press 'A':\n"
            "\n",
            menu->settings.default_directory
        );
    } else {
        ui_components_main_text_draw(
                STL_DEFAULT,
                ALIGN_LEFT, VALIGN_TOP,
                "\n"
                "\n"
                "\n"
                "\n"
                "To change the following menu settings, press 'A':\n"
                "\n",
                menu->settings.default_directory
            );
    }

    int selected_row = *selected_row_ptrs[selected_tab];

    size_t total_length = 1;
    char row_text[96];
    char *row_texts[16] = {0};
    int rendered_row_count = 0;

    int row_count = tab_row_count(selected_tab);
    for (int i = 0; i < row_count; i++) {
        if (!get_setting_row_text(menu, i, row_text, sizeof(row_text))) {
            continue;
        }
        row_texts[rendered_row_count] = strdup(row_text);
        if (row_texts[rendered_row_count] != NULL) {
            total_length += strlen(row_texts[rendered_row_count]);
        }
        rendered_row_count++;
    }

    if (rendered_row_count > 0) {
        rdpq_paragraph_t *layout;
        rdpq_paragraph_t *settings_list_layout = malloc(sizeof(rdpq_paragraph_t) + (sizeof(rdpq_paragraph_char_t) * total_length));
        if (settings_list_layout != NULL) {
            memset(settings_list_layout, 0, sizeof(rdpq_paragraph_t));
            settings_list_layout->capacity = total_length;
        }

        rdpq_paragraph_builder_begin(
            &(rdpq_textparms_t) {
                .width = VISIBLE_AREA_WIDTH - (TEXT_MARGIN_HORIZONTAL * 2),
                .height = LAYOUT_ACTIONS_SEPARATOR_Y - VISIBLE_AREA_Y0 - (TEXT_MARGIN_VERTICAL * 2),
                .wrap = WRAP_ELLIPSES,
                .line_spacing = TEXT_LINE_SPACING_ADJUST,
            },
            FNT_DEFAULT,
            settings_list_layout
        );

        for (int i = 0; i < rendered_row_count; i++) {
            if (!row_texts[i]) {
                continue;
            }

            rdpq_paragraph_builder_style(i == selected_row ? STL_DEFAULT : STL_GRAY);
            rdpq_paragraph_builder_span(row_texts[i], strlen(row_texts[i]));

            if ((i + 1) < rendered_row_count) {
                rdpq_paragraph_builder_newline();
            }
        }

        layout = rdpq_paragraph_builder_end();

        int list_y = VISIBLE_AREA_Y0 + TAB_HEIGHT + TEXT_MARGIN_VERTICAL + 96;
        int highlight_height = (layout->bbox.y1 - layout->bbox.y0) / layout->nlines;
        int highlight_y = list_y + (selected_row * highlight_height);

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
            list_y
        );

        rdpq_paragraph_free(layout);
    }

    for (int i = 0; i < rendered_row_count; i++) {
        free(row_texts[i]);
    }

    ui_components_actions_bar_text_draw(
        STL_DEFAULT,
        ALIGN_LEFT, VALIGN_TOP,
        "A: Toggle\n"
        "B: Back"
    );

    ui_components_actions_bar_text_draw(
        STL_DEFAULT,
        ALIGN_CENTER, VALIGN_TOP,
        "◀ Change Tab ▶\n"
        "\n"
    );

    ui_components_actions_bar_text_draw(
        STL_DEFAULT,
        ALIGN_RIGHT, VALIGN_TOP,
        "R: Reset settings\n"
        "\n"
    );

    if (selected_tab == SETTINGS_TAB_VIDEO && get_tv_type() == TV_PAL) {
        ui_components_main_text_draw(
            STL_RED,
            ALIGN_LEFT, VALIGN_BOTTOM,
            "* NOTE: This setting may cause the display to go dark. If that happens, wait 10 seconds or press B or manually edit the menu/config.ini on the SD card.\n"
        );
    } else if (selected_tab == SETTINGS_TAB_VIDEO && get_tv_type() != TV_PAL) {
        ui_components_main_text_draw(
            STL_RED,
            ALIGN_LEFT, VALIGN_BOTTOM,
            "* NOTE: Reboot console to apply changes.\n"
        );
    }

    if (show_message_reset_settings) {
        ui_components_messagebox_draw(
            "Reset settings?\n\n"
            "A: Yes, B: Back"
        );
    } else if (show_pal60_confirm_message) {
        ui_components_messagebox_draw(
            "PAL60 Mode\n\n"
            "PAL60 will be reverted after 5 seconds.\n"
            "A: Use PAL60, B: Return to PAL50"
        );
    }

    rdpq_detach_show();
}


void view_settings_init (menu_t *menu) {
    selected_tab = SETTINGS_TAB_MENU;
    selected_menu_row = 0;
    selected_video_row = 0;
    selected_sound_row = 0;
    show_pal60_confirm_message = false;
    pal60_confirm_waiting_for_input = false;
    pal60_confirm_started_at = 0;
}

void view_settings_display (menu_t *menu, surface_t *display) {
    process(menu);
    
    draw(menu, display);
}
