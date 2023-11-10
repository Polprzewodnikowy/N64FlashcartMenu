#include "views.h"


#ifndef MENU_VERSION
#define MENU_VERSION "0.0.0.6.ALPHA"
#endif


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
        "MENU INFORMATION"
    );

    component_main_text_draw(
        ALIGN_LEFT, VALIGN_TOP,
        "\n"
        "\n"
        "Menu Revision: V%s\n"
        "\n"
        "Github:\n"
        " https://github.com/Polprzewodnikowy/N64FlashcartMenu\n"
        "Authors:\n"
        " JonesAlmighty / NetworkFusion\n"
        " Mateusz Faderewski / Polprzewodnikowy\n"
        "Credits:\n"
        " N64Brew / libdragon contributors\n"
        "\n"
        "OSS software used:\n"
        " libdragon (UNLICENSE License)\n"
        " libspng (BSD 2-Clause License)\n"
        " mini.c (BSD 2-Clause License)\n"
        " minimp3 (CC0 1.0 Universal)\n"
        " miniz (MIT License)",
        MENU_VERSION
    );

    component_actions_bar_text_draw(
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
