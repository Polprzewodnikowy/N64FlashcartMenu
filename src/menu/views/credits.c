#include "views.h"
#include "../sound.h"

#ifndef MENU_VERSION
#define MENU_VERSION "Unknown"
#endif

#ifndef BUILD_TIMESTAMP
#define BUILD_TIMESTAMP "Unknown"
#endif


static void process (menu_t *menu) {
    if (menu->actions.back) {
        sound_play_effect(SFX_EXIT);
        menu->next_mode = MENU_MODE_BROWSER; 
    }
}

static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    ui_components_background_draw();

    ui_components_layout_draw();

    ui_components_main_text_draw(
        ALIGN_CENTER, VALIGN_TOP,
        "MENU INFORMATION"
    );

    ui_components_main_text_draw(
        ALIGN_LEFT, VALIGN_TOP,
        "\n"
        "\n"
        "Menu version: %s\n"
        "Build timestamp: %s\n"
        "\n"
        "Github - Website:\n"
        " https://github.com/Polprzewodnikowy/N64FlashcartMenu\n"
        "Authors:\n"
        " Robin Jones / NetworkFusion\n"
        " Mateusz Faderewski / Polprzewodnikowy\n"
        "Contributors:\n"
        " Thank you to ALL project contributors,\n"
        " no matter how small the commit.\n"
        "OSS software used:\n"
        " libdragon (UNLICENSE License)\n"
        " libspng (BSD 2-Clause License)\n"
        " mini.c (BSD 2-Clause License)\n"
        " minimp3 (CC0 1.0 Universal)\n"
        " miniz (MIT License)",
        MENU_VERSION,
        BUILD_TIMESTAMP
    );

    ui_components_actions_bar_text_draw(
        ALIGN_LEFT, VALIGN_TOP,
        "\n"
        "B: Exit"
    );

    rdpq_detach_show();
}


void view_credits_init (menu_t *menu) {
    // Nothing to initialize (yet)
}

void view_credits_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);
}
