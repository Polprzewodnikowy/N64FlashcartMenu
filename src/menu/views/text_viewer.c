#include <libdragon.h>
#include <stdlib.h>
#include <stdio.h>

#include "fragments/fragments.h"
#include "views.h"

//static *char file_content;

static void process (menu_t *menu) {
    if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach_clear(d, NULL);

    // display file content.
    fragment_loader(d);


    rdpq_detach_show();
}


static void deinit (menu_t *menu) {
    // free memory
}


void view_text_viewer_init (menu_t *menu) {

    path_t *path = path_clone(menu->browser.directory);
    path_push(path, menu->browser.list[menu->browser.selected].name);

    // read file content

    path_free(path);
}

void view_text_viewer_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);

    if (menu->next_mode != MENU_MODE_TEXT_VIEWER) {
        deinit(menu);
    }
}