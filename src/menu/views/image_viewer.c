#include <libdragon.h>
#include <stdlib.h>

#include "../png_decoder.h"
#include "fragments/fragments.h"
#include "views.h"


static bool image_loading;
static surface_t *image;


static void process (menu_t *menu) {
    if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach_clear(d, NULL);

    if (!image) {
        fragment_loader(d);
    } else {
        uint16_t x = (d->width / 2) - (image->width / 2);
        uint16_t y = (d->height / 2) - (image->height / 2);

        rdpq_set_mode_copy(false);
        rdpq_tex_blit(image, x, y, NULL);
    }

    rdpq_detach_show();
}

static void image_callback (png_err_t err, surface_t *decoded_image, void *callback_data) {
    image_loading = false;
    image = decoded_image;

    if (err != PNG_OK) {
        menu_t *menu = (menu_t *) (callback_data);
        menu->next_mode = MENU_MODE_ERROR;
    }
}

static void deinit (menu_t *menu) {
    if (image_loading) {
        png_decode_abort();
    }

    if (image) {
        surface_free(image);
        free(image);
    }
}


void view_image_viewer_init (menu_t *menu) {
    image_loading = false;
    image = NULL;

    path_t *path = path_clone(menu->browser.directory);
    path_push(path, menu->browser.list[menu->browser.selected].name);

    if (png_decode_start(path_get(path), 640, 480, image_callback, menu) == PNG_OK) {
        image_loading = true;
    } else {
        menu->next_mode = MENU_MODE_ERROR;
    }

    path_free(path);
}

void view_image_viewer_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);

    if (menu->next_mode != MENU_MODE_IMAGE_VIEWER) {
        deinit(menu);
    }
}
