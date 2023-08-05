#include <libdragon.h>
#include <stdlib.h>
#include <stdio.h>

#include "views.h"

static char *file_content;

static void process (menu_t *menu) {
    if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

static void draw (menu_t *menu, surface_t *d) {
    
    rdpq_attach(d, NULL);

    //display file content.
    component_background_draw();

    component_main_text_draw(
        ALIGN_CENTER, VALIGN_TOP,
        "FILE CONTENT"
    );

    // FIXME: should have scrollbars if required.
    component_main_text_draw(
        ALIGN_LEFT, VALIGN_TOP,
        "\n\n%s",
        file_content
    );

    component_actions_bar_text_draw(
        ALIGN_LEFT, VALIGN_TOP,
        "B: Exit"
    );

    rdpq_detach_show();
}


static void deinit (menu_t *menu) {
    // free memory
    free(file_content);
}


void view_text_viewer_init (menu_t *menu) {

    path_t *path = path_clone(menu->browser.directory);
    path_push(path, menu->browser.list[menu->browser.selected].name);

    // read file content
    char *sd_path = calloc(4 + strlen(path_get(path)) + 1, sizeof(char));
    sprintf(sd_path, "sd:/%s", path_get(path));

    uint16_t file_size = 2048;  // FIXME: get proper file size and set a reasonable maximum.

    file_content = calloc(file_size, sizeof(char));

    FILE *fp = fopen(sd_path, "r");

    debugf("loading path: %s\n", sd_path);
	if (!fp) {
        debugf("Error loading file content\n");
    }
    else {
        fread(file_content, file_size, sizeof(char), fp);
        fclose(fp);
    }

    path_free(path);
}

void view_text_viewer_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);

    if (menu->next_mode != MENU_MODE_TEXT_VIEWER) {
        deinit(menu);
    }
}