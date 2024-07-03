#include "../sound.h"
#include "views.h"


static const char *format_switch (bool state) {
    switch (state) {
        case true: return "On";
        case false: return "Off";
    }
}

static void set_pal60_type (menu_t *menu, void *arg) {
    menu->settings.pal60_enabled = (bool) (arg);
    
    menu->browser.reload = true;
}

static void set_protected_entries_type (menu_t *menu, void *arg) {
    menu->settings.show_protected_entries = (bool) (arg);
    
    menu->browser.reload = true;
}

static void set_sound_enabled_type (menu_t *menu, void *arg) {
    menu->settings.sound_enabled = (bool) (arg);
    
    menu->browser.reload = true;
}

static void set_use_saves_folder_type (menu_t *menu, void *arg) {
    menu->settings.use_saves_folder = (bool) (arg);
    
    menu->browser.reload = true;
}

static component_context_menu_t set_pal60_type_context_menu = { .list = {
    {.text = "Enable", .action = set_pal60_type, .arg = (void *) (true) },
    {.text = "Disable", .action = set_pal60_type, .arg = (void *) (false) },
    COMPONENT_CONTEXT_MENU_LIST_END,
}};

static component_context_menu_t set_protected_entries_type_context_menu = { .list = {
    {.text = "Enable", .action = set_protected_entries_type, .arg = (void *) (true) },
    {.text = "Disable", .action = set_protected_entries_type, .arg = (void *) (false) },
    COMPONENT_CONTEXT_MENU_LIST_END,
}};

static component_context_menu_t set_sound_enabled_type_context_menu = { .list = {
    {.text = "Enable", .action = set_sound_enabled_type, .arg = (void *) (true) },
    {.text = "Disable", .action = set_sound_enabled_type, .arg = (void *) (false) },
    COMPONENT_CONTEXT_MENU_LIST_END,
}};

static component_context_menu_t set_use_saves_folder_type_context_menu = { .list = {
    {.text = "Enable", .action = set_use_saves_folder_type, .arg = (void *) (true) },
    {.text = "Disable", .action = set_use_saves_folder_type, .arg = (void *) (false) },
    COMPONENT_CONTEXT_MENU_LIST_END,
}};

static component_context_menu_t options_context_menu = { .list = {
    { .text = "Set PAL60 Mode", .submenu = &set_pal60_type_context_menu },
    { .text = "Show Hidden Files", .submenu = &set_protected_entries_type_context_menu },
    { .text = "Set Menu Sound", .submenu = &set_sound_enabled_type_context_menu },
    { .text = "Use saves folder", .submenu = &set_use_saves_folder_type_context_menu },

    COMPONENT_CONTEXT_MENU_LIST_END,
}};


static void process (menu_t *menu) {
    if (component_context_menu_process(menu, &options_context_menu)) {
        return;
    }

    if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
        sound_play_effect(SFX_EXIT);
    } else if (menu->actions.options) {
        component_context_menu_show(&options_context_menu);
        sound_play_effect(SFX_SETTING);
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
        "\n"
        "\n"
        "To change the settings, press `R`.\n"
        "Certain options are only adjustable\n"
        "directly in the 'menu/config.ini' file.\n\n"
        "pal60_enabled:          %s\n"
        "show_protected_entries: %s\n"
        "default_directory:      %s\n"
        "use_saves_folder:       %s\n"
        "bgm_enabled:            %s\n"
        "sound_enabled:          %s\n"
        "rumble_enabled:         %s\n",
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
        "\n"
        "B: Back"
    );

    component_actions_bar_text_draw(
            ALIGN_RIGHT, VALIGN_TOP,
            "\n"
            "R: Options"
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
