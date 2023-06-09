#include <libdragon.h>
#include "menu_info.h"
#include "menu_res_setup.h"

void menu_info_draw_header(surface_t *disp) {
	
	graphics_draw_text(disp, (disp->width / 2) - 64, vertical_start_position, "MENU INFORMATION"); // centre = numchars * font_horizontal_pixels / 2
	graphics_draw_line(disp, 0, 30, disp->width, 30, 0xff);
	
}

void menu_info_draw_footer(surface_t *disp) {
	
	graphics_draw_line(disp, 0, disp->height - overscan_vertical_pixels - font_vertical_pixels, disp->width,disp->height - overscan_vertical_pixels - font_vertical_pixels, 0xff);
	graphics_draw_text(disp, (disp->width / 2) - 80,disp->height - overscan_vertical_pixels, "Press (B) to return!"); // centre = numchars * font_horizontal_pixels / 2
	
}

void menu_info(void) {
	
	surface_t *disp = display_try_get();
	graphics_fill_screen(disp, 0x00);
	menu_info_draw_header(disp);

	int16_t vertical_position = 40;

	graphics_draw_text(disp, horizontal_start_position, vertical_position, "Menu Version:");
	graphics_draw_text(disp, horizontal_indent,vertical_position += font_vertical_pixels, MENU_VERSION);
	vertical_position += (font_vertical_pixels * 2);
	graphics_draw_text(disp, horizontal_start_position, vertical_position, "Authors:");
	graphics_draw_text(disp, horizontal_indent, vertical_position += font_vertical_pixels, "JonesAlmighty / NetworkFusion");
	graphics_draw_text(disp, horizontal_indent, vertical_position += font_vertical_pixels, "korgeaux / Polprzewodnikowy");
	vertical_position += (font_vertical_pixels * 2);
	graphics_draw_text(disp, horizontal_start_position, vertical_position += font_vertical_pixels, "Github:");
	// graphics_draw_text(disp, horizontal_indent, vertical_position += font_vertical_pixels, "https://github.com/Polprzewodnikowy/SummerCart64");
	graphics_draw_text(disp, horizontal_indent, vertical_position += font_vertical_pixels, "https://github.com/Polprzewodnikowy/N64FlashcartMenu");
	graphics_draw_text(disp, horizontal_indent, vertical_position += font_vertical_pixels, "https://github.com/NetworkFusion/N64FlashcartMenu");
	vertical_position += (font_vertical_pixels * 2);
	graphics_draw_text(disp, horizontal_start_position, vertical_position, "OSS licenses used:");
	graphics_draw_text(disp, horizontal_indent,vertical_position += font_vertical_pixels, "UNLICENSE");
	graphics_draw_text(disp, horizontal_indent,vertical_position += font_vertical_pixels, "MIT");

	menu_info_draw_footer(disp);

	display_show(disp);

	for (;;) {
		controller_scan();
		struct controller_data ckeys = get_keys_down();

		if (ckeys.c[0].B) {
			break;
		}
	}
}