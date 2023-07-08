#include <libdragon.h>

#include "views.h"

#include "../menu_res_setup.h"

#ifndef MENU_VERSION
#define MENU_VERSION "0.0.0.4"
#endif


static void process (menu_t *menu) {
    if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

static void draw (menu_t *menu, surface_t *d) {
	graphics_fill_screen(d, 0x00);

	graphics_draw_text(d, (d->width / 2) - 64, vertical_start_position, "MENU INFORMATION"); // centre = numchars * font_horizontal_pixels / 2
	graphics_draw_line(d, 0, 30, d->width, 30, 0xff);

	int16_t vertical_position = 40;

	graphics_draw_text(d, horizontal_start_position, vertical_position, "Menu Revision: V");
	graphics_draw_text(d, horizontal_start_position + 16 * 8, vertical_position, MENU_VERSION);
	vertical_position += (font_vertical_pixels * 2);
	graphics_draw_text(d, horizontal_start_position, vertical_position, "Authors:");
	graphics_draw_text(d, horizontal_indent, vertical_position += font_vertical_pixels, "JonesAlmighty / NetworkFusion");
	graphics_draw_text(d, horizontal_indent, vertical_position += font_vertical_pixels, "korgeaux / Polprzewodnikowy");
	vertical_position += (font_vertical_pixels * 2);
	graphics_draw_text(d, horizontal_start_position, vertical_position += font_vertical_pixels, "Credits:");
	graphics_draw_text(d, horizontal_indent, vertical_position += font_vertical_pixels, "N64Brew / libdragon contributors.");
	vertical_position += (font_vertical_pixels * 2);
	graphics_draw_text(d, horizontal_start_position, vertical_position += font_vertical_pixels, "Github:");
	// graphics_draw_text(d, horizontal_indent, vertical_position += font_vertical_pixels, "https://github.com/Polprzewodnikowy/SummerCart64");
	graphics_draw_text(d, horizontal_indent, vertical_position += font_vertical_pixels, "https://github.com/Polprzewodnikowy/N64FlashcartMenu");
	graphics_draw_text(d, horizontal_indent, vertical_position += font_vertical_pixels, "https://github.com/NetworkFusion/N64FlashcartMenu");
	//graphics_draw_text(d, horizontal_indent, vertical_position += font_vertical_pixels, "https://github.com/dragonminded/libdragon");
	vertical_position += (font_vertical_pixels * 2);
	graphics_draw_text(d, horizontal_start_position, vertical_position, "OSS licenses used:");
	graphics_draw_text(d, horizontal_indent,vertical_position += font_vertical_pixels, "UNLICENSE");
	graphics_draw_text(d, horizontal_indent,vertical_position += font_vertical_pixels, "MIT");

	graphics_draw_line(d, 0, d->height - overscan_vertical_pixels - font_vertical_pixels, d->width,d->height - overscan_vertical_pixels - font_vertical_pixels, 0xff);
	graphics_draw_text(d, (d->width / 2) - 80,d->height - overscan_vertical_pixels, "Press (B) to return!"); // centre = numchars * font_horizontal_pixels / 2

    display_show(d);
}


void view_credits_init (menu_t *menu) {
    // Nothing to initialize (yet)
}

void view_credits_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}
