#include <stdbool.h>
#include "../sound.h"
#include "../settings.h"
#include "views.h"

static bool settings_changed = false;

static const char *format_switch (bool state) {
    switch (state) {
        case true: return "On";
        case false: return "Off";
    }
}

static void set_pal60_type (menu_t *menu, void *arg) {
    bool initial_setting_val = menu->settings.pal60_enabled;
    menu->settings.pal60_enabled = (bool) (arg);
    if (initial_setting_val != menu->settings.pal60_enabled) {
        settings_changed = true;
    }
    
    menu->browser.reload = true;
}

static void set_protected_entries_type (menu_t *menu, void *arg) {
    bool initial_setting_val = menu->settings.show_protected_entries;
    menu->settings.show_protected_entries = (bool) (arg);
    if (initial_setting_val != menu->settings.show_protected_entries) {
        settings_changed = true;
    }

    menu->browser.reload = true;
}

static void set_sound_enabled_type (menu_t *menu, void *arg) {
    bool initial_setting_val = menu->settings.sound_enabled;
    menu->settings.sound_enabled = (bool) (arg);
    if (initial_setting_val != menu->settings.sound_enabled) {
        settings_changed = true;
    }

    menu->browser.reload = true;
}

static void set_use_saves_folder_type (menu_t *menu, void *arg) {
    bool initial_setting_val = menu->settings.use_saves_folder;
    menu->settings.use_saves_folder = (bool) (arg);
    if (initial_setting_val != menu->settings.use_saves_folder) {
        settings_changed = true;
    }

    menu->browser.reload = true;
}

// static void set_use_default_settings (menu_t *menu, void *arg) {
//     // FIXME: add implementation
//     menu->browser.reload = true;
// }

static component_context_menu_t set_pal60_type_context_menu = { .list = {
    {.text = "On", .action = set_pal60_type, .arg = (void *) (true) },
    {.text = "Off", .action = set_pal60_type, .arg = (void *) (false) },
    COMPONENT_CONTEXT_MENU_LIST_END,
}};

static component_context_menu_t set_protected_entries_type_context_menu = { .list = {
    {.text = "On", .action = set_protected_entries_type, .arg = (void *) (true) },
    {.text = "Off", .action = set_protected_entries_type, .arg = (void *) (false) },
    COMPONENT_CONTEXT_MENU_LIST_END,
}};

static component_context_menu_t set_sound_enabled_type_context_menu = { .list = {
    {.text = "On", .action = set_sound_enabled_type, .arg = (void *) (true) },
    {.text = "Off", .action = set_sound_enabled_type, .arg = (void *) (false) },
    COMPONENT_CONTEXT_MENU_LIST_END,
}};

static component_context_menu_t set_use_saves_folder_type_context_menu = { .list = {
    {.text = "On", .action = set_use_saves_folder_type, .arg = (void *) (true) },
    {.text = "Off", .action = set_use_saves_folder_type, .arg = (void *) (false) },
    COMPONENT_CONTEXT_MENU_LIST_END,
}};

static component_context_menu_t options_context_menu = { .list = {
    { .text = "Set PAL60 Mode", .submenu = &set_pal60_type_context_menu },
    { .text = "Show Hidden Files", .submenu = &set_protected_entries_type_context_menu },
    { .text = "Set Menu Sound", .submenu = &set_sound_enabled_type_context_menu },
    { .text = "Use saves folder", .submenu = &set_use_saves_folder_type_context_menu },
    // { .text = "Restore defaults", .action = set_use_default_settings },

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
        // FIXME: should we allow preview in the menu, or forget them?
        menu->next_mode = MENU_MODE_BROWSER;
        sound_play_effect(SFX_EXIT);
    } else if (menu->actions.options) {
        if (settings_changed) {
            settings_save(&menu->settings);
            settings_changed = false;
            sound_play_effect(SFX_ENTER);
        }
    }
}

static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    component_background_draw();

    component_layout_draw();

	component_main_text_draw(
        ALIGN_CENTER, VALIGN_TOP,
        "SETTINGS EDITOR\n"
        "\n"
    );

    component_main_text_draw(
        ALIGN_LEFT, VALIGN_TOP,
        "\n\n"
        "To change the settings, press 'A'.\n\n"
        "pal60_enabled:          %s\n"
        "show_protected_entries: %s\n"
        "default_directory*:     %s\n"
        "use_saves_folder:       %s\n"
        "bgm_enabled:            %s\n"
        "sound_enabled**:        %s\n"
        "rumble_enabled:         %s\n\n"
        "Note: Certain settings are only adjustable\n"
        "directly in the 'menu/config.ini' file.\n\n"
        "*  Set using other menu.\n"
        "** Currently requires a flashcart reboot.\n\n"
        "To write the settings permanently, press 'R`.\n",
        format_switch(menu->settings.pal60_enabled),
        format_switch(menu->settings.show_protected_entries),
        menu->settings.default_directory,
        format_switch(menu->settings.use_saves_folder),
        format_switch(menu->settings.bgm_enabled),
        format_switch(menu->settings.sound_enabled),
        format_switch(menu->settings.rumble_enabled)
    );


    component_actions_bar_text_draw(
        ALIGN_LEFT, VALIGN_TOP,
        "A: Change\n"
        "B: Back"
    );

    if (settings_changed) {
        component_actions_bar_text_draw(
                ALIGN_RIGHT, VALIGN_TOP,
                "\n"
                "R: Save"
            );
    }

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
