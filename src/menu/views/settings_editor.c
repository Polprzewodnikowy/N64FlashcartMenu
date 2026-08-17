/**
 * @file settings_editor.c
 * @brief The Menu pane of the Settings tab.
 * @ingroup view
 *
 * Edits the values in @ref settings_t. This is a settings pane rather than a
 * standalone view; the Settings tab in settings_tab.c arranges it.
 */

#include <stdbool.h>
#include <stddef.h>

#include "../sound.h"
#include "../settings.h"
#include "../ui_components/constants.h"
#include "../../utils/utils.h"
#include "views.h"

/** @brief Marks a setting that is not a simple boolean toggle. */
#define NO_BOOL_OFFSET  ((size_t) -1)

/** @brief Declare a boolean setting bound to a field of @ref settings_t. */
#define BOOL_SETTING(label, field, flags) \
    { (label), NULL, offsetof(settings_t, field), (flags) }

/** @brief Side effects a setting has beyond writing its own value. */
enum {
    SETTING_RELOAD_BROWSER  = (1 << 0),  /**< Browser contents depend on this setting. */
    SETTING_UPDATE_SFX      = (1 << 1),  /**< Apply to the sound effect mixer. */
    SETTING_UPDATE_BGM      = (1 << 2),  /**< Apply to the background music mixer. */
    SETTING_CONFIRM_PAL60   = (1 << 3),  /**< Needs the PAL60 warning before applying. */
    SETTING_CONFIRM_RESET   = (1 << 4),  /**< Needs the reset confirmation before applying. */
    SETTING_CLEAR_BACKGROUND = (1 << 5), /**< Acts on press rather than toggling. */
};

typedef const char *(*setting_value_fn)(menu_t *menu);

typedef struct {
    const char *label;
    /** @brief Formats the value column, or NULL for a boolean setting. */
    setting_value_fn value;
    /** @brief Offset of the bool inside @ref settings_t, or @ref NO_BOOL_OFFSET. */
    size_t bool_offset;
    unsigned flags;
} setting_descriptor_t;

static int selected_row;
static bool show_reset_confirm_message;
static bool show_reset_complete_message;
static bool show_pal60_confirm_message;
static bool pal60_target;


static const char *format_switch (bool state) {
    return state ? "On" : "Off";
}

static const char *format_clear_action (menu_t *menu) {
    return "A: Clear";
}

static const char *format_reset_action (menu_t *menu) {
    return "A: Reset";
}

/** @brief Read only: the default directory is configured in menu/config.ini. */
static const char *format_default_directory (menu_t *menu) {
    return menu->settings.default_directory;
}

#ifdef FEATURE_AUTOLOAD_ROM_ENABLED
static const char *format_autoload (menu_t *menu) {
    return format_switch(menu->settings.rom_autoload_enabled);
}
#endif

static const setting_descriptor_t settings[] = {
    { "Default Directory", format_default_directory, NO_BOOL_OFFSET, 0 },
    BOOL_SETTING("Show Hidden Files", show_protected_entries, SETTING_RELOAD_BROWSER),
    BOOL_SETTING("Sound Effects", soundfx_enabled, SETTING_UPDATE_SFX),
    BOOL_SETTING("Background Music", bgm_enabled, SETTING_UPDATE_BGM),
    BOOL_SETTING("Use Saves Folder", use_saves_folder, 0),
    BOOL_SETTING("Show Saves Folder", show_saves_folder, SETTING_RELOAD_BROWSER),
    BOOL_SETTING("Show Save Files", show_save_files, SETTING_RELOAD_BROWSER),
    BOOL_SETTING("Show Cheat Files", show_cheat_files, SETTING_RELOAD_BROWSER),
    BOOL_SETTING("PAL60 Mode", pal60_enabled, SETTING_CONFIRM_PAL60),
    BOOL_SETTING("Wrap File List", wrap_file_list_scrolling, 0),
#ifdef FEATURE_AUTOLOAD_ROM_ENABLED
    { "Autoload ROM", format_autoload, NO_BOOL_OFFSET, 0 },
    BOOL_SETTING("ROM Loading Bar", loading_progress_bar_enabled, 0),
#else
    BOOL_SETTING("Fast Reboot ROM", rom_fast_reboot_enabled, 0),
#endif
#ifdef BETA_SETTINGS
    BOOL_SETTING("Hide ROM Extensions", show_browser_file_extensions, SETTING_RELOAD_BROWSER),
    BOOL_SETTING("Hide ROM Tags", show_browser_rom_tags, 0),
    BOOL_SETTING("Rumble Feedback", rumble_enabled, 0),
#endif
    { "Remove Background", format_clear_action, NO_BOOL_OFFSET, SETTING_CLEAR_BACKGROUND },
    { "Reset to Defaults", format_reset_action, NO_BOOL_OFFSET, SETTING_CONFIRM_RESET },
};

#define SETTING_COUNT ((int) (sizeof(settings) / sizeof(settings[0])))

/**
 * @brief Resolve the boolean a setting is bound to.
 */
static bool *setting_value_ptr (menu_t *menu, const setting_descriptor_t *setting) {
    return (bool *) ((char *) &menu->settings + setting->bool_offset);
}

static const char *setting_value (menu_t *menu, const setting_descriptor_t *setting) {
    if (setting->value) {
        return setting->value(menu);
    }
    return format_switch(*setting_value_ptr(menu, setting));
}

/**
 * @brief Apply PAL60 to the video timing, then persist it.
 *
 * PAL60 only exists on PAL consoles, so the setting is forced off elsewhere.
 */
static void apply_pal60 (menu_t *menu, bool enabled) {
    if (get_tv_type() == TV_PAL) {
        vi_set_timing_preset(enabled ? &VI_TIMING_PAL60 : &VI_TIMING_PAL);
        menu->settings.pal60_enabled = enabled;
    } else {
        menu->settings.pal60_enabled = false;
    }
    settings_save(&menu->settings);
}

/**
 * @brief Act on the selected setting.
 *
 * Settings that need confirmation only raise their dialog here; they are
 * applied once the dialog is accepted.
 */
static void setting_activate (menu_t *menu) {
    const setting_descriptor_t *setting = &settings[selected_row];
    bool *value;

    if (setting->flags & SETTING_CONFIRM_RESET) {
        show_reset_confirm_message = true;
        return;
    }

    if (setting->flags & SETTING_CLEAR_BACKGROUND) {
        ui_components_background_clear();
        return;
    }

    /* Read only rows have nothing to toggle. */
    if (setting->bool_offset == NO_BOOL_OFFSET) {
        return;
    }

    value = setting_value_ptr(menu, setting);

    if (setting->flags & SETTING_CONFIRM_PAL60) {
        pal60_target = !*value;
        show_pal60_confirm_message = true;
        return;
    }

    *value = !*value;

    if (setting->flags & SETTING_RELOAD_BROWSER) {
        menu->browser.reload = true;
    }
    if (setting->flags & SETTING_UPDATE_SFX) {
        sound_use_sfx(*value);
    }
    if (setting->flags & SETTING_UPDATE_BGM) {
        sound_use_bgm(*value);
    }

    settings_save(&menu->settings);
}

/**
 * @brief Whether the selected setting responds to Left / Right as well as A.
 *
 * Toggles are adjustable in place; actions such as "Reset to Defaults" are not,
 * so that scrolling past them cannot trigger them by accident.
 */
static bool setting_is_toggle (void) {
    return settings[selected_row].bool_offset != NO_BOOL_OFFSET;
}

static void pane_enter (menu_t *menu) {
    selected_row = 0;
    show_reset_confirm_message = false;
    show_reset_complete_message = false;
    show_pal60_confirm_message = false;
}

static bool pane_process (menu_t *menu) {
    if (show_pal60_confirm_message) {
        if (menu->actions.enter) {
            apply_pal60(menu, pal60_target);
            show_pal60_confirm_message = false;
            sound_play_effect(SFX_SETTING);
        } else if (menu->actions.back) {
            show_pal60_confirm_message = false;
            sound_play_effect(SFX_EXIT);
        }
        return true;
    }

    if (show_reset_confirm_message) {
        if (menu->actions.enter) {
            settings_reset_to_defaults();
            show_reset_confirm_message = false;
            show_reset_complete_message = true;
            sound_play_effect(SFX_ENTER);
        } else if (menu->actions.back) {
            show_reset_confirm_message = false;
            sound_play_effect(SFX_EXIT);
        }
        return true;
    }

    if (show_reset_complete_message) {
        if (menu->actions.enter || menu->actions.back) {
            show_reset_complete_message = false;
            sound_play_effect(SFX_EXIT);
        }
        return true;
    }

    if (menu->actions.back) {
        return false;
    } else if (menu->actions.go_up && selected_row > 0) {
        selected_row--;
        sound_play_effect(SFX_CURSOR);
    } else if (menu->actions.go_down && selected_row < SETTING_COUNT - 1) {
        selected_row++;
        sound_play_effect(SFX_CURSOR);
    } else if (menu->actions.enter || ((menu->actions.go_left || menu->actions.go_right) && setting_is_toggle())) {
        setting_activate(menu);
        sound_play_effect(SFX_SETTING);
    }

    return true;
}

static void pane_draw (menu_t *menu, bool focused) {
    int first = MIN(MAX(selected_row - (SETTINGS_ROWS / 2), 0), MAX(SETTING_COUNT - SETTINGS_ROWS, 0));
    int y = SETTINGS_PANE_Y0 + 4;

    for (int i = first; (i < SETTING_COUNT) && (i < first + SETTINGS_ROWS); i++) {
        ui_components_settings_row_draw(
            y, settings[i].label, setting_value(menu, &settings[i]), focused && (selected_row == i)
        );
        y += SETTINGS_ROW_HEIGHT;
    }
}

static void pane_overlay (menu_t *menu) {
    if (show_pal60_confirm_message) {
        ui_components_messagebox_draw(
            "PAL60 MODE WARNING\n\n"
            "* NOTE: This setting may cause the display to go dark. If you get it wrong, "
            "you must manually edit the menu/config.ini on the SD card to re-disable it.\n\n"
            "A: Apply        B: Cancel"
        );
    } else if (show_reset_confirm_message) {
        ui_components_messagebox_draw("Reset all menu settings to defaults?\n\nA: Reset        B: Cancel");
    } else if (show_reset_complete_message) {
        ui_components_messagebox_draw(
            "Defaults restored.\n\nReboot the N64 to apply every setting.\n\nA or B: Close"
        );
    }
}

static const char *pane_hint (menu_t *menu, settings_hint_t slot) {
    switch (slot) {
        case SETTINGS_HINT_LEFT: return "A: Change\nB: Categories";
        case SETTINGS_HINT_CENTER: return "D-Pad: Adjust\nL / R: Tabs";
        default: return NULL;
    }
}

const settings_pane_t settings_pane_menu = {
    .label = "Menu",
    .enter = pane_enter,
    .process = pane_process,
    .draw = pane_draw,
    .overlay = pane_overlay,
    .hint = pane_hint,
};
