#include <libdragon.h>

#include "views.h"


static void process (menu_t *menu) {
    menu->next_mode = MENU_MODE_BROWSER;
}

static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach_clear(d, NULL);
    rdpq_detach_show();
}


void view_startup_init (menu_t *menu) {
    // Nothing to initialize (yet)
}

void view_startup_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}
