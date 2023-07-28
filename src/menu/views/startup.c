#include <libdragon.h>

#include "../components/components.h"
#include "utils/fs.h"
#include "views.h"


#define CACHE_DIRECTORY     "sd:/menu/cache"

#define BACKGROUND_CACHE    "sd:/menu/cache/background.data"


static void process (menu_t *menu) {
    menu->next_mode = MENU_MODE_BROWSER;
}

static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach_clear(d, NULL);
    rdpq_detach_show();
}

static void load (menu_t *menu) {
    menu->components.background = component_background_create(BACKGROUND_CACHE);
}


void view_startup_init (menu_t *menu) {
    directory_create(CACHE_DIRECTORY);
}

void view_startup_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);

    load(menu);
}
