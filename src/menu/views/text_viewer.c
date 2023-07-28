#include <libdragon.h>
#include <stdlib.h>
#include <stdio.h>

#include "fragments/fragments.h"
#include "views.h"

static char *file_line_content[20];
static uint16_t file_line_count = 0;

static void process (menu_t *menu) {
    if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

static void draw (menu_t *menu, surface_t *d) {
    
    const color_t text_color = RGBA32(0x00, 0x00, 0x00, 0xFF);
    const color_t bg_color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF);

    rdpq_attach(d, NULL);
    rdpq_clear(bg_color);

    fragment_text_start(text_color);

    //display file content.

    const int offset_x = 40;
    int offset_y = 40;

    rdpq_font_begin(text_color);
    
    for (int i = 0; i < file_line_count - 1; i++)
    {
        if (file_line_content[i] != NULL) {
            offset_y += fragment_textf(offset_x, offset_y, file_line_content[i]);
        }
    }

    fragment_textf(offset_x, d->height - 40, "B: Exit");
    rdpq_detach_show();
}


static void deinit (menu_t *menu) {
    // free memory
    for (int i = 0; i < file_line_count; i++)
    {
        free(file_line_content[i]);
    }
}


void view_text_viewer_init (menu_t *menu) {

    file_line_count = 0;

    path_t *path = path_clone(menu->browser.directory);
    path_push(path, menu->browser.list[menu->browser.selected].name);

    // read file content
    char *sd_path = calloc(4 + strlen(path_get(path)) + 1, sizeof(char));
    sprintf(sd_path, "sd:/%s", path_get(path));

    FILE *fp = fopen(sd_path, "r");

    debugf("loading path: %s\n", sd_path);
	if (!fp) {
        debugf("Error loading file content\n");
    }
    else {
        // read the first 20 lines of the file with a max line length of 256 chars
        for (int i = 0; i < 20; i++)
        {
            if( feof(fp) ) {
                break;
            }

            file_line_content[i] = calloc(256, sizeof(char));
            fgets(file_line_content[i], 256, fp);

            file_line_count += 1;

        }

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