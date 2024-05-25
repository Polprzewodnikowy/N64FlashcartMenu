#include "../sfx.h"
#include "views.h"


static const char *format_switch (bool state) {
    switch (state) {
        case true: return "On";
        case false: return "Off";
    }
}


static void process (menu_t *menu) {
    if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
        wav64_play(&sfx_exit, SFX_CURSOR);
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
        "To change the settings, please adjust them\n"
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

    rdpq_detach_show();
}


void view_settings_init (menu_t *menu) {
    // Nothing to initialize (yet)
}

void view_settings_display (menu_t *menu, surface_t *display) {
    process(menu);
    
    draw(menu, display);
}
