#include "views.h"


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
        "pal60_enabled: %d\n"
        "hidden_files_enabled: %d\n"
        "default_directory: %s\n"
        "use_saves_folder: %d\n"
        "autodetect_rom_region: %d\n"
        "bgm_enabled: %d\n"
        "sound_enabled: %d\n"
        "rumble_enabled: %d\n",
        menu->settings.pal60_enabled,
        menu->settings.hidden_files_enabled,
        menu->settings.default_directory,
        menu->settings.autodetect_rom_region,
        menu->settings.bgm_enabled,
        menu->settings.sound_enabled,
        menu->settings.rumble_enabled
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
