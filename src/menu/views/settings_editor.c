#include <libdragon.h>
#include <mini.c/src/mini.h>

#include "../settings.h"

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

    // TODO: add editor here!


    component_actions_bar_text_draw(
        ALIGN_LEFT, VALIGN_TOP,
        "A: Save\n"
        "B: Cancel"
    );

    rdpq_detach_show();
}


void view_settings_init (menu_t *menu) {
    // TODO: load the current settings.
}

void view_settings_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}
