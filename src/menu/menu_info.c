#include <libdragon.h>

static void draw_header(display_context_t disp) {
	graphics_draw_text(disp, 200-70, 10, "Menu Information");
}


void menu_info(void) {
	display_init(RESOLUTION_512x240, DEPTH_16_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE);
	display_context_t disp = display_get();
	graphics_fill_screen(disp, 0);
	draw_header(disp);

	graphics_draw_text(disp, 30, 50, "Authors:");
	graphics_draw_text(disp, 30, 58, "JonesAlmighty / NetworkFusion");
	graphics_draw_text(disp, 30, 66, "korgeaux / Polprzewodnikowy");

	graphics_draw_text(disp, 30, 80, "Github:");
	graphics_draw_text(disp, 30, 88, "https://github.com/Polprzewodnikowy/SummerCart64");
	graphics_draw_text(disp, 30, 96, "https://github.com/Polprzewodnikowy/N64FlashcartMenu");

	graphics_draw_text(disp, 30,112, "OSS licenses used:");
	graphics_draw_text(disp, 30,120, "GPL");
	graphics_draw_text(disp, 30,128, "MIT");

	graphics_draw_text(disp, 30,144, "Press B to return!");

	display_show(disp);

	for (;;) {
		controller_scan();
		struct controller_data ckeys = get_keys_down();

		if (ckeys.c[0].B) {
			break;
		}
	}
}