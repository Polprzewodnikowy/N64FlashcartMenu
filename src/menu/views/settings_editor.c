#include "views.h"

static char *convert_boolean (int state) {
    switch (state) {
        case 0: return "Off";
        case 1: return "On";
        default: return "Unknown";
    }
}

static void process (menu_t *menu) {
    if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
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
        "pal60_enabled:         %s\n"
        "hidden_files_enabled:  %s\n"
        "default_directory:     %s\n"
        "use_saves_folder:      %s\n"
        "autodetect_rom_region: %s\n"
        "bgm_enabled:           %s\n"
        "sound_enabled:         %s\n"
        "rumble_enabled:        %s\n",
        convert_boolean(menu->settings.pal60_enabled),
        convert_boolean(menu->settings.hidden_files_enabled),
        menu->settings.default_directory,
        convert_boolean(menu->settings.use_saves_folder),
        convert_boolean(menu->settings.autodetect_rom_region),
        convert_boolean(menu->settings.bgm_enabled),
        convert_boolean(menu->settings.sound_enabled),
        convert_boolean(menu->settings.rumble_enabled)
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
