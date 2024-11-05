#include <stdbool.h>
#include "../sound.h"
#include "../settings.h"
#include "views.h"


static const char *format_switch (bool state) {
    switch (state) {
        case true: return "On";
        case false: return "Off";
    }
}

static void set_pal60_type (menu_t *menu, void *arg) {
    menu->settings.pal60_enabled = (bool)(uintptr_t)(arg);
    settings_save(&menu->settings);
}

static void set_protected_entries_type (menu_t *menu, void *arg) {
    menu->settings.show_protected_entries = (bool)(uintptr_t)(arg);
    settings_save(&menu->settings);

    menu->browser.reload = true;
}

static void set_use_saves_folder_type (menu_t *menu, void *arg) {
    menu->settings.use_saves_folder = (bool)(uintptr_t)(arg);
    settings_save(&menu->settings);
}

static void set_sound_enabled_type (menu_t *menu, void *arg) {
    menu->settings.sound_enabled = (bool)(uintptr_t)(arg);
    sound_use_sfx(menu->settings.sound_enabled);
    settings_save(&menu->settings);
}

#ifdef BETA_SETTINGS
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


static component_context_menu_t set_pal60_type_context_menu = { .list = {
    {.text = "On", .action = set_pal60_type, .arg = (void *)(uintptr_t)(true) },
    {.text = "Off", .action = set_pal60_type, .arg = (void *) (false) },
    COMPONENT_CONTEXT_MENU_LIST_END,
}};

static component_context_menu_t set_protected_entries_type_context_menu = { .list = {
    {.text = "On", .action = set_protected_entries_type, .arg = (void *)(uintptr_t)(true) },
    {.text = "Off", .action = set_protected_entries_type, .arg = (void *)(uintptr_t)(false) },
    COMPONENT_CONTEXT_MENU_LIST_END,
}};

static component_context_menu_t set_sound_enabled_type_context_menu = { .list = {
    {.text = "On", .action = set_sound_enabled_type, .arg = (void *)(uintptr_t)(true) },
    {.text = "Off", .action = set_sound_enabled_type, .arg = (void *)(uintptr_t)(false) },
    COMPONENT_CONTEXT_MENU_LIST_END,
}};

static component_context_menu_t set_use_saves_folder_type_context_menu = { .list = {
    {.text = "On", .action = set_use_saves_folder_type, .arg = (void *)(uintptr_t)(true) },
    {.text = "Off", .action = set_use_saves_folder_type, .arg = (void *)(uintptr_t)(false) },
    COMPONENT_CONTEXT_MENU_LIST_END,
}};

#ifdef BETA_SETTINGS
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
    { .text = "PAL60 Mode", .submenu = &set_pal60_type_context_menu },
    { .text = "Show Hidden Files", .submenu = &set_protected_entries_type_context_menu },
    { .text = "Sound Effects", .submenu = &set_sound_enabled_type_context_menu },
    { .text = "Use Saves Folder", .submenu = &set_use_saves_folder_type_context_menu },
#ifdef BETA_SETTINGS
    { .text = "Background Music", .submenu = &set_bgm_enabled_type_context_menu },
    { .text = "Rumble Feedback", .submenu = &set_rumble_enabled_type_context_menu },
    // { .text = "Restore Defaults", .action = set_use_default_settings },
#endif

    COMPONENT_CONTEXT_MENU_LIST_END,
}};


static void process (menu_t *menu) {
    if (component_context_menu_process(menu, &options_context_menu)) {
        return;
    }

    if (menu->actions.enter) {
        component_context_menu_show(&options_context_menu);
        sound_play_effect(SFX_SETTING);
    } else if (menu->actions.back) {
        sound_play_effect(SFX_EXIT);
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    component_background_draw();

    component_layout_draw();

	component_main_text_draw(
        ALIGN_CENTER, VALIGN_TOP,
        "MENU SETTINGS EDITOR\n"
        "\n"
    );

    component_main_text_draw(
        ALIGN_LEFT, VALIGN_TOP,
        "\n\n"
        "  Default Directory : %s\n\n"
        "  Autoload ROM      : %s\n"
        "To change the following menu settings, press 'A':\n"
        "*    PAL60 Mode        : %s\n"
        "     Show Hidden Files : %s\n"
        "     Use Saves folder  : %s\n"
        "     Sound Effects     : %s\n"
#ifdef BETA_SETTINGS
        "     Background Music  : %s\n"
        "     Rumble Feedback   : %s\n"
#endif
        "\n\n"
        "Note: Certain settings have the following caveats:\n"
        "*    Requires rebooting the N64 Console.\n",
        menu->settings.default_directory,
        format_switch(menu->settings.rom_autoload_enabled),
        format_switch(menu->settings.pal60_enabled),
        format_switch(menu->settings.show_protected_entries),
        format_switch(menu->settings.use_saves_folder),
        format_switch(menu->settings.sound_enabled)
#ifdef BETA_SETTINGS
        ,
        format_switch(menu->settings.bgm_enabled),
        format_switch(menu->settings.rumble_enabled)
#endif
    );


    component_actions_bar_text_draw(
        ALIGN_LEFT, VALIGN_TOP,
        "A: Change\n"
        "B: Back"
    );

    component_context_menu_draw(&options_context_menu);

    rdpq_detach_show();
}


void view_settings_init (menu_t *menu) {
    
    component_context_menu_init(&options_context_menu);

}

void view_settings_display (menu_t *menu, surface_t *display) {
    process(menu);
    
    draw(menu, display);
}
