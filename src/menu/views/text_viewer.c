#include <libdragon.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

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


    char *sd_path = calloc(4 + strlen(path_get(path)) + 1, sizeof(char));
    sprintf(sd_path, "sd:/%s", path_get(path));

    // Set the buffer size from the file size
    struct stat file_stat;
    stat(sd_path, &file_stat);
    uint32_t file_size = file_stat.st_size;

    if (file_size > 307200) { // FIXME: this is just 640x480 which definitely means it will be larger than the display can show.
        file_size = 307200; // For the moment, we just set it to that, since any more would be a waste.
    }

    file_content = calloc(file_size, 1);

    // read file content
    FILE *fp = fopen(sd_path, "r");

    debugf("loading path: %s\n", sd_path);
	if (!fp) {
        debugf("Error loading file content\n");
    }
    else {
        fread(file_content, file_size, 1, fp);
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