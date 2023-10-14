#include "utils/fs.h"
#include "views.h"


static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach_clear(d, NULL);
    rdpq_detach_show();
}


void view_startup_init (menu_t *menu) {
    menu->next_mode = MENU_MODE_BROWSER;
}

void view_startup_display (menu_t *menu, surface_t *display) {
    draw(menu, display);
}
