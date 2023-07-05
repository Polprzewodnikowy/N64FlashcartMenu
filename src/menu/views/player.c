#include <libdragon.h>

#include "../mp3player.h"
#include "fragments/fragments.h"
#include "views.h"


static void process (menu_t *menu) {
    if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
        mp3player_unload();
    } else {
        if (mp3player_process()) {
            mp3player_unload();
            menu->next_mode = MENU_MODE_BROWSER;
        }
    }
}

static void draw (menu_t *menu, surface_t *d) {
    // layout_t *layout = get_layout();

    const color_t bg_color = RGBA32(0x00, 0x00, 0x00, 0xFF);

    rdpq_attach(d, NULL);
    rdpq_clear(bg_color);

    // Layout
    fragment_borders(d);

    rdpq_detach_show();
}


void view_player_init (menu_t *menu) {
    path_t *path = path_clone(menu->browser.directory);
    path_push(path, menu->browser.list[menu->browser.selected].name);
    if (mp3player_load(path_get(path))) {
        menu->next_mode = MENU_MODE_ERROR;
    } else {
        mp3player_start();
    }
    path_free(path);
}

void view_player_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}
