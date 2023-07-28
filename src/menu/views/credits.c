#include <libdragon.h>

#include "../components/components.h"
#include "fragments/fragments.h"
#include "views.h"


#ifndef MENU_VERSION
#define MENU_VERSION "0.0.0.5.ALPHA"
#endif


static void process (menu_t *menu) {
    if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

static void draw (menu_t *menu, surface_t *d) {
    layout_t *layout = layout_get();

    const int text_x = layout->offset_x + layout->offset_text_x;
    int text_y = layout->offset_y + layout->offset_text_y;

    const color_t text_color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF);

    rdpq_attach(d, NULL);

    // Background
    component_background_draw(menu->components.background);

    // Layout
    fragment_borders(d);

    // Text start
    fragment_text_start(text_color);

	text_y += fragment_textf((d->width / 2) - 76, text_y, "MENU INFORMATION");

	text_y += fragment_textf(text_x, text_y, "\n");

	text_y += fragment_textf(text_x, text_y, "Menu Revision: V%s", MENU_VERSION);
	text_y += fragment_textf(text_x, text_y, "\n");
	text_y += fragment_textf(text_x, text_y, "Authors:");
	text_y += fragment_textf(text_x, text_y, "  JonesAlmighty / NetworkFusion");
	text_y += fragment_textf(text_x, text_y, "  korgeaux / Polprzewodnikowy");
	text_y += fragment_textf(text_x, text_y, "\n");
	text_y += fragment_textf(text_x, text_y, "Credits:");
	text_y += fragment_textf(text_x, text_y, "  N64Brew / libdragon contributors.");
	text_y += fragment_textf(text_x, text_y, "\n");
	text_y += fragment_textf(text_x, text_y, "Github:");
	text_y += fragment_textf(text_x, text_y, "  https://github.com/Polprzewodnikowy/N64FlashcartMenu");
	text_y += fragment_textf(text_x, text_y, "\n");
	text_y += fragment_textf(text_x, text_y, "OSS licenses used:");
	text_y += fragment_textf(text_x, text_y, "  UNLICENSE"); /* libdragon license */
	text_y += fragment_textf(text_x, text_y, "  MIT");
    text_y += fragment_textf(text_x, text_y, "  BSD 2-Clause"); /* libspng license */
    text_y += fragment_textf(text_x, text_y, "  CC0 1.0 Universal"); /* minimp3 license */
    // text_y += fragment_textf(text_x, text_y, "  Permissive, unspecific"); /* miniz license */


    // Actions bar
    text_y = layout->actions_y + layout->offset_text_y;
    text_y += fragment_textf(text_x, text_y, "B: Exit");

    rdpq_detach_show();
}


void view_credits_init (menu_t *menu) {
    // Nothing to initialize (yet)
}

void view_credits_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}
