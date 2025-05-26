#include <stdbool.h>
#include "../sound.h"
#include "../settings.h"
#include "views.h"

static bool show_message;

static const char *format_switch (bool state) {
    switch (state) {
        case true: return "On";
        case false: return "Off";
    }
}

#ifdef FEATURE_AUTOLOAD_ROM
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

static void set_soundfx_enabled_type (menu_t *menu, void *arg) {
    menu->settings.soundfx_enabled = (bool)(uintptr_t)(arg);
    sound_use_sfx(menu->settings.soundfx_enabled);
    settings_save(&menu->settings);
}

static void set_use_rom_fast_reboot_enabled_type (menu_t *menu, void *arg) {
    menu->settings.rom_fast_reboot_enabled = (bool)(uintptr_t)(arg);
    settings_save(&menu->settings);
}

#ifdef BETA_SETTINGS
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

#ifdef FEATURE_AUTOLOAD_ROM
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

static component_context_menu_t set_use_rom_fast_reboot_context_menu = { .list = {
    {.text = "On", .action = set_use_rom_fast_reboot_enabled_type, .arg = (void *)(uintptr_t)(true) },
    {.text = "Off", .action = set_use_rom_fast_reboot_enabled_type, .arg = (void *)(uintptr_t)(false) },
    COMPONENT_CONTEXT_MENU_LIST_END,
}};

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
#ifdef FEATURE_AUTOLOAD_ROM
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


static void process (menu_t *menu) {
    if (ui_components_context_menu_process(menu, &options_context_menu)) {
        return;
    }

    if (menu->actions.enter) {
        if (show_message) {
            settings_reset_to_defaults();
            menu_show_error(menu, "Reboot N64 to take effect!");
            show_message = false;
        } else {
            ui_components_context_menu_show(&options_context_menu);
        }
        sound_play_effect(SFX_SETTING);
    } else if (menu->actions.back) {
        if (show_message) {
            show_message = false;
        } else {
            menu->next_mode = MENU_MODE_BROWSER;
        }
        sound_play_effect(SFX_EXIT);
    } else if (menu->actions.options){
        show_message = true;
    }
}

static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    ui_components_background_draw();

    ui_components_layout_draw();

	ui_components_main_text_draw(
        ALIGN_CENTER, VALIGN_TOP,
        "MENU SETTINGS EDITOR\n"
        "\n"
    );

    ui_components_main_text_draw(
        ALIGN_LEFT, VALIGN_TOP,
        "\n\n"
        "  Default Directory : %s\n"
#ifdef FEATURE_AUTOLOAD_ROM
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
#ifdef FEATURE_AUTOLOAD_ROM
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
        ALIGN_LEFT, VALIGN_TOP,
        "A: Change\n"
        "B: Back"
    );

    ui_components_actions_bar_text_draw(
        ALIGN_RIGHT, VALIGN_TOP,
        "R: Reset settings\n"
        "\n"
    );

    ui_components_context_menu_draw(&options_context_menu);

    if (show_message) {
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
