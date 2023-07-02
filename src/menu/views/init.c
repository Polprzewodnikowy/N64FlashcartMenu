#include <libdragon.h>
#include "../menu.h"


static void draw (menu_t *menu, surface_t *d) {
    graphics_fill_screen(d, graphics_make_color(0, 0, 0, 255));

    display_show(d);
}

void view_init_display (menu_t *menu, surface_t *display) {
    draw(menu, display);
}
