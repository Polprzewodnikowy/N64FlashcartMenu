#include <stdio.h>
#include <sys/stat.h>

#include "utils/fs.h"
#include "utils/utils.h"
#include "views.h"


static char *text;


static void process (menu_t *menu) {
    if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    component_background_draw();

    component_layout_draw();

    if (text) {
        component_main_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "%s\n",
            text
        );
    } else {
        component_messagebox_draw("Text file is empty");
    }

    component_actions_bar_text_draw(
        ALIGN_LEFT, VALIGN_TOP,
        "\n"
        "B: Back"
    );

    rdpq_detach_show();
}

static void deinit (void) {
    if (text) {
        free(text);
        text = NULL;
    }
}


void view_text_viewer_init (menu_t *menu) {
    path_t *path = path_clone_push(menu->browser.directory, menu->browser.entry->name);

    FILE *f;

    if ((f = fopen(path_get(path), "r")) == NULL) {
        path_free(path);
        menu_show_error(menu, "Couldn't open text file");
        return;
    }

    path_free(path);

    struct stat st;
    if (fstat(fileno(f), &st)) {
        fclose(f);
        menu_show_error(menu, "Couldn't get text file size");
        return;
    }

    // TODO: Implement proper text file viewer with both vertical and horizontal scrolling
    size_t size = MIN(st.st_size, 1024);

    if (size) {
        if ((text = calloc(sizeof(char), size)) == NULL) {
            fclose(f);
            menu_show_error(menu, "Couldn't allocate memory for the text file contents");
            return;
        }

        if (fread(text, size, 1, f) != 1) {
            fclose(f);
            menu_show_error(menu, "Couldn't read text file contents");
            return;
        }
    }

    if (fclose(f)) {
        menu_show_error(menu, "Couldn't close text file");
    }
}

void view_text_viewer_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);

    if (menu->next_mode != MENU_MODE_TEXT_VIEWER) {
        deinit();
    }
}
