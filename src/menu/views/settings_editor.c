#include <stdbool.h>
#include "../sound.h"
#include "../settings.h"
#include "views.h"

static bool show_message_reset_settings = false;

/**
 * Convert a boolean switch state to its user-facing label.
 * @param state The switch state to format.
 * @returns `"On"` if `state` is `true`, `"Off"` otherwise.
 */
static const char *format_switch (bool state) {
    switch (state) {
        case true: return "On";
        case false: return "Off";
    }
}

#ifdef FEATURE_AUTOLOAD_ROM_ENABLED
/**
 * Update the menu setting that enables or disables the ROM loading progress bar.
 *
 * @param menu Menu instance whose settings will be updated and persisted.
 * @param arg Boolean value cast through a pointer: `true` to enable the loading progress bar, `false` to disable it.
 */
static void set_loading_progress_bar_enabled_type (menu_t *menu, void *arg) {
    menu->settings.loading_progress_bar_enabled = (bool)(uintptr_t)(arg);
    settings_save(&menu->settings);
}
#endif

/**
 * Update the menu setting that controls visibility of protected entries and persist the change.
 *
 * @param menu Pointer to the menu whose settings will be updated.
 * @param arg Pointer-sized boolean where non-zero enables showing protected entries and zero hides them.
 */
static void set_protected_entries_type (menu_t *menu, void *arg) {
    menu->settings.show_protected_entries = (bool)(uintptr_t)(arg);
    settings_save(&menu->settings);

    menu->browser.reload = true;
}

/**
 * Update the menu settings to enable or disable using the saves folder.
 *
 * Sets menu->settings.use_saves_folder to the boolean value provided by `arg`
 * and persists the updated settings.
 *
 * @param menu Menu context whose settings will be modified.
 * @param arg Boolean value: `true` to enable using the saves folder, `false` to disable.
 */
static void set_use_saves_folder_type (menu_t *menu, void *arg) {
    menu->settings.use_saves_folder = (bool)(uintptr_t)(arg);
    settings_save(&menu->settings);
}

/**
 * Update the "show saves folder" setting, persist it, and mark the browser for reload.
 *
 * Updates menu->settings.show_saves_folder to the boolean value provided by `arg`,
 * saves the settings, and sets menu->browser.reload to true so the browser will refresh.
 *
 * @param menu Menu context whose settings will be modified.
 * @param arg Boolean value (passed as a void pointer) indicating whether to show the saves folder.
 */
static void set_show_saves_folder_type (menu_t *menu, void *arg) {
    menu->settings.show_saves_folder = (bool)(uintptr_t)(arg);
    settings_save(&menu->settings);

    menu->browser.reload = true;
}

/**
 * Update the sound effects enabled setting, apply the change, and persist it.
 *
 * @param menu Menu context whose settings will be modified.
 * @param arg Boolean value (passed as a void pointer) indicating whether sound effects should be enabled: `true` to enable, `false` to disable.
 */
static void set_soundfx_enabled_type (menu_t *menu, void *arg) {
    menu->settings.soundfx_enabled = (bool)(uintptr_t)(arg);
    sound_use_sfx(menu->settings.soundfx_enabled);
    settings_save(&menu->settings);
}

#ifndef FEATURE_AUTOLOAD_ROM_ENABLED
/**
 * Set the ROM fast-reboot setting and persist it.
 *
 * Updates the menu's rom_fast_reboot_enabled setting according to the provided argument
 * (interpreted as a boolean) and saves the settings to storage.
 *
 * @param menu Menu context whose settings will be modified.
 * @param arg Pointer-sized value interpreted as a boolean: nonzero enables fast reboot, zero disables it.
 */
static void set_use_rom_fast_reboot_enabled_type (menu_t *menu, void *arg) {
    menu->settings.rom_fast_reboot_enabled = (bool)(uintptr_t)(arg);
    settings_save(&menu->settings);
}
#endif

#ifdef BETA_SETTINGS
/**
 * Set the PAL60 (60Hz PAL) setting for the given menu and persist the change.
 *
 * Updates menu->settings.pal60_enabled to enabled when `arg` is non-zero and disabled when `arg` is zero, then saves the settings via settings_save.
 *
 * @param menu Menu context whose settings will be modified.
 * @param arg Boolean flag passed as a pointer-sized value: non-zero to enable PAL60, zero to disable.
 */
static void set_pal60_type (menu_t *menu, void *arg) {
    menu->settings.pal60_enabled = (bool)(uintptr_t)(arg);
    settings_save(&menu->settings);
}

static void set_mod_pal60_compatibility_type (menu_t *menu, void *arg) {
    menu->settings.pal60_compatibility_mode = (bool)(uintptr_t)(arg);
    settings_save(&menu->settings);
}

static void set_bgm_enabled_type (menu_t *menu, void *arg) {
    menu->settings.bgm_enabled = (bool)(uintptr_t)(arg);
    settings_save(&menu->settings);
}

static void set_rumble_enabled_type (menu_t *menu, void *arg) {
    menu->settings.rumble_enabled = (bool)(uintptr_t)(arg);
    settings_save(&menu->settings);
}

// static void set_use_default_settings (menu_t *menu, void *arg) {
//     // FIXME: add implementation
//     menu->browser.reload = true;
// }
#endif

#ifdef FEATURE_AUTOLOAD_ROM_ENABLED
static component_context_menu_t set_loading_progress_bar_enabled_context_menu = { .list = {
    {.text = "On", .action = set_loading_progress_bar_enabled_type, .arg = (void *)(uintptr_t)(true) },
    {.text = "Off", .action = set_loading_progress_bar_enabled_type, .arg = (void *)(uintptr_t)(false) },
    COMPONENT_CONTEXT_MENU_LIST_END,
}};
#endif

static component_context_menu_t set_protected_entries_type_context_menu = { .list = {
    {.text = "On", .action = set_protected_entries_type, .arg = (void *)(uintptr_t)(true) },
    {.text = "Off", .action = set_protected_entries_type, .arg = (void *)(uintptr_t)(false) },
    COMPONENT_CONTEXT_MENU_LIST_END,
}};

static component_context_menu_t set_soundfx_enabled_type_context_menu = { .list = {
    {.text = "On", .action = set_soundfx_enabled_type, .arg = (void *)(uintptr_t)(true) },
    {.text = "Off", .action = set_soundfx_enabled_type, .arg = (void *)(uintptr_t)(false) },
    COMPONENT_CONTEXT_MENU_LIST_END,
}};

static component_context_menu_t set_use_saves_folder_type_context_menu = { .list = {
    {.text = "On", .action = set_use_saves_folder_type, .arg = (void *)(uintptr_t)(true) },
    {.text = "Off", .action = set_use_saves_folder_type, .arg = (void *)(uintptr_t)(false) },
    COMPONENT_CONTEXT_MENU_LIST_END,
}};

static component_context_menu_t set_show_saves_folder_type_context_menu = { .list = {
    {.text = "On", .action = set_show_saves_folder_type, .arg = (void *)(uintptr_t)(true) },
    {.text = "Off", .action = set_show_saves_folder_type, .arg = (void *)(uintptr_t)(false) },
    COMPONENT_CONTEXT_MENU_LIST_END,
}};

#ifndef FEATURE_AUTOLOAD_ROM_ENABLED
static component_context_menu_t set_use_rom_fast_reboot_context_menu = { .list = {
    {.text = "On", .action = set_use_rom_fast_reboot_enabled_type, .arg = (void *)(uintptr_t)(true) },
    {.text = "Off", .action = set_use_rom_fast_reboot_enabled_type, .arg = (void *)(uintptr_t)(false) },
    COMPONENT_CONTEXT_MENU_LIST_END,
}};
#endif

#ifdef BETA_SETTINGS
static component_context_menu_t set_pal60_type_context_menu = { .list = {
    {.text = "On", .action = set_pal60_type, .arg = (void *)(uintptr_t)(true) },
    {.text = "Off", .action = set_pal60_type, .arg = (void *)(uintptr_t)(false) },
    COMPONENT_CONTEXT_MENU_LIST_END,
}};

static component_context_menu_t set_pal60_mod_compatibility_type_context_menu = { .list = {
    {.text = "On", .action = set_mod_pal60_compatibility_type, .arg = (void *)(uintptr_t)(true) },
    {.text = "Off", .action = set_mod_pal60_compatibility_type, .arg = (void *)(uintptr_t)(false) },
    COMPONENT_CONTEXT_MENU_LIST_END,
}};

static component_context_menu_t set_bgm_enabled_type_context_menu = { .list = {
    {.text = "On", .action = set_bgm_enabled_type, .arg = (void *)(uintptr_t)(true) },
    {.text = "Off", .action = set_bgm_enabled_type, .arg = (void *)(uintptr_t)(false) },
    COMPONENT_CONTEXT_MENU_LIST_END,
}};

static component_context_menu_t set_rumble_enabled_type_context_menu = { .list = {
    {.text = "On", .action = set_rumble_enabled_type, .arg = (void *)(uintptr_t)(true) },
    {.text = "Off", .action = set_rumble_enabled_type, .arg = (void *)(uintptr_t)(false) },
    COMPONENT_CONTEXT_MENU_LIST_END,
}};
#endif

static component_context_menu_t options_context_menu = { .list = {
    { .text = "Show Hidden Files", .submenu = &set_protected_entries_type_context_menu },
    { .text = "Sound Effects", .submenu = &set_soundfx_enabled_type_context_menu },
    { .text = "Use Saves Folder", .submenu = &set_use_saves_folder_type_context_menu },
    { .text = "Show Saves Folder", .submenu = &set_show_saves_folder_type_context_menu },
#ifdef FEATURE_AUTOLOAD_ROM_ENABLED
    { .text = "ROM Loading Bar", .submenu = &set_loading_progress_bar_enabled_context_menu },
#else
    { .text = "Fast Reboot ROM", .submenu = &set_use_rom_fast_reboot_context_menu },
#endif
#ifdef BETA_SETTINGS
    { .text = "PAL60 Mode", .submenu = &set_pal60_type_context_menu },
    { .text = "PAL60 Compatibility", .submenu = &set_pal60_mod_compatibility_type_context_menu },
    { .text = "Background Music", .submenu = &set_bgm_enabled_type_context_menu },
    { .text = "Rumble Feedback", .submenu = &set_rumble_enabled_type_context_menu },
    // { .text = "Restore Defaults", .action = set_use_default_settings },
#endif

    COMPONENT_CONTEXT_MENU_LIST_END,
}};


/**
 * Handle input actions for the Settings Editor view and perform corresponding state changes.
 *
 * When the options context menu is active, input is consumed and no further processing occurs.
 * - Enter: if the reset-confirmation flag is set, reset settings to defaults and show a reboot notice; otherwise open the options context menu. Plays the setting-change sound effect.
 * - Back: if the reset-confirmation flag is set, clear the flag; otherwise request switching to the browser menu. Plays the exit sound effect.
 * - Options: set the reset-confirmation flag so the next Enter triggers a reset.
 *
 * @param menu Current menu context used for input state and mode transitions.
 */
static void process (menu_t *menu) {
    if (ui_components_context_menu_process(menu, &options_context_menu)) {
        return;
    }

    if (menu->actions.enter) {
        if (show_message_reset_settings) {
            settings_reset_to_defaults();
            menu_show_error(menu, "Reboot N64 to take effect!");
            show_message_reset_settings = false;
        } else {
            ui_components_context_menu_show(&options_context_menu);
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

/**
 * Render the Settings Editor view onto the specified drawing surface.
 *
 * Renders the background and layout, the main header and current settings
 * values from the provided menu context, the action bars, the options
 * context menu, and — when active — the reset confirmation message box.
 *
 * @param menu Menu context whose settings and state are used for display.
 * @param d    Drawing surface to render the view onto.
 */
static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    ui_components_background_draw();

    ui_components_layout_draw();

	ui_components_main_text_draw(
        STL_DEFAULT,
        ALIGN_CENTER, VALIGN_TOP,
        "MENU SETTINGS EDITOR\n"
        "\n"
    );

    ui_components_main_text_draw(
        STL_DEFAULT,
        ALIGN_LEFT, VALIGN_TOP,
        "\n\n"
        "  Default Directory : %s\n"
#ifdef FEATURE_AUTOLOAD_ROM_ENABLED
        "  Autoload ROM      : %s\n\n"
        "To change the following menu settings, press 'A':\n"
        "    ROM Loading Bar   : %s\n"
#else
        "\n"
        "To change the following menu settings, press 'A':\n"
        "     Fast Reboot ROM   : %s\n"
#endif
        "     Show Hidden Files : %s\n"
        "     Use Saves folder  : %s\n"
        "     Show Saves folder : %s\n"
        "     Sound Effects     : %s\n"
#ifdef BETA_SETTINGS
        "*    PAL60 Mode        : %s\n"
        "*    PAL60 Mod Compat  : %s\n"
        "     Background Music  : %s\n"
        "     Rumble Feedback   : %s\n"
        "\n\n"
        "Note: Certain settings have the following caveats:\n"
        "*    Requires rebooting the N64 Console.\n"
#endif
        ,
        menu->settings.default_directory,
#ifdef FEATURE_AUTOLOAD_ROM_ENABLED
        format_switch(menu->settings.rom_autoload_enabled),
        format_switch(menu->settings.loading_progress_bar_enabled),
#else
        format_switch(menu->settings.rom_fast_reboot_enabled),
#endif
        format_switch(menu->settings.show_protected_entries),
        format_switch(menu->settings.use_saves_folder),
        format_switch(menu->settings.show_saves_folder),
        format_switch(menu->settings.soundfx_enabled)
#ifdef BETA_SETTINGS
        ,
        format_switch(menu->settings.pal60_enabled),
        format_switch(menu->settings.pal60_compatibility_mode),
        format_switch(menu->settings.bgm_enabled),
        format_switch(menu->settings.rumble_enabled)
#endif
    );

    ui_components_actions_bar_text_draw(
        STL_DEFAULT,
        ALIGN_LEFT, VALIGN_TOP,
        "A: Change\n"
        "B: Back"
    );

    ui_components_actions_bar_text_draw(
        STL_DEFAULT,
        ALIGN_RIGHT, VALIGN_TOP,
        "R: Reset settings\n"
        "\n"
    );

    ui_components_context_menu_draw(&options_context_menu);

    if (show_message_reset_settings) {
        ui_components_messagebox_draw(
            "Reset settings?\n\n"
            "A: Yes, B: Back"
        );
    }

    rdpq_detach_show();
}


void view_settings_init (menu_t *menu) {
    ui_components_context_menu_init(&options_context_menu);

}

void view_settings_display (menu_t *menu, surface_t *display) {
    process(menu);
    
    draw(menu, display);
}