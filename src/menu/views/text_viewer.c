#include <fatfs/ff.h>

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
    path_t *path = path_clone_push(menu->browser.directory, menu->browser.entry->name);

    uint32_t file_size = file_get_size(path_get(path));

    if (file_size > 307200) { // FIXME: this is just 640x480 which definitely means it will be larger than the display can show.
        file_size = 307200; // For the moment, we just set it to that, since any more would be a waste.
    }

    file_content = calloc(file_size, 1);

    // read file content
    FIL fil;
    UINT br;

    if (f_open(&fil, strip_sd_prefix(path_get(path)), FA_READ) != FR_OK) {
        debugf("Error loading file\n");
    }
    if (f_read(&fil, file_content, file_size, &br) != FR_OK) {
        f_close(&fil);
        debugf("Error loading file content\n");
    }
    if (f_close(&fil) != FR_OK) {
        debugf("Error closing file\n");
    }

    path_free(path);
}

void view_text_viewer_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}
