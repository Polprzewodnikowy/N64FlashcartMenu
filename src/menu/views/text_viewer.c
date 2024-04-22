#include <stdio.h>

#include "utils/fs.h"
#include "views.h"


static char *file_content;


static void process (menu_t *menu) {
    if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    component_background_draw();

    component_layout_draw();

	component_main_text_draw(
        ALIGN_CENTER, VALIGN_TOP,
        "TEXT VIEWER\n"
        "\n"
    );

    component_main_text_draw(
        ALIGN_LEFT, VALIGN_TOP,
        "\n"
        "\n"
        "%s\n",
        file_content
    );

    component_actions_bar_text_draw(
        ALIGN_LEFT, VALIGN_TOP,
        "\n"
        "B: Back"
    );

    rdpq_detach_show();
}


void view_text_viewer_init (menu_t *menu) {
    FILE *f;
    path_t *path = path_clone_push(menu->browser.directory, menu->browser.entry->name);

    int64_t file_size = file_get_size(path_get(path));

    if (file_size > 1024) { // FIXME: this is just a placeholder until scrolling is implemented.
        file_size = 1024; // For the moment, we just set it to that, since any more would be a waste.
    }

    file_content = calloc(file_size, 1);

    if ((f = fopen(path_get(path), "r")) == NULL) {
        path_free(path);
        menu_show_error(menu, "Couldn't open text file");
        return;
    }
    path_free(path);
    fread(file_content, 1, file_size, f);
    if (fclose(f)) {
        menu_show_error(menu, "Couldn't close text file");
    }
}

void view_text_viewer_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);
}
