#include <libdragon.h>
#include "../menu.h"


static void process (menu_t *menu) {
    if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

static void draw (menu_t *menu, surface_t *d) {
	graphics_fill_screen(d, graphics_make_color(0, 0, 0, 255));

    graphics_draw_text(d, 24, 36, "ERROR!");

    display_show(d);
}


void view_error_init (menu_t *menu) {
    // Nothing to initialize (yet)
}

void view_error_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}
