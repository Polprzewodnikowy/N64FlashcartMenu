#include <libdragon.h>
#include <stdlib.h>

#include "../png_decoder.h"
#include "fragments/fragments.h"
#include "views.h"


static surface_t *image;
static rspq_block_t *cached_image_dl;


static void process (menu_t *menu) {
    if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach_clear(d, NULL);

    if (image == NULL) {
        fragment_loader(d);
    } else {
        rspq_block_run(cached_image_dl);
    }

    rdpq_detach_show();
}

static void deffered_image_load (menu_t *menu, surface_t *d) {
    image = calloc(1, sizeof(surface_t));

    if (image == NULL) {
        menu->next_mode = MENU_MODE_ERROR;
        return;
    }

    path_t *path = path_clone(menu->browser.directory);
    path_push(path, menu->browser.list[menu->browser.selected].name);

    if (png_decode(path_get(path), image, 640, 480) == PNG_OK) {
        uint16_t x = (d->width / 2) - (image->width / 2);
        uint16_t y = (d->height / 2) - (image->height / 2);

        rspq_block_begin();

        rdpq_set_mode_copy(false);
        rdpq_tex_blit(image, x, y, NULL);

        cached_image_dl = rspq_block_end();
    } else {
        menu->next_mode = MENU_MODE_ERROR;
    }

    path_free(path);
}

static void dl_free (void *arg) {
    rspq_block_free((rspq_block_t *) (arg));
}

static void deinit (menu_t *menu) {
    if (image != NULL) {
        rdpq_call_deferred(dl_free, cached_image_dl);
        surface_free(image);
        free(image);
    }
}


void view_image_viewer_init (menu_t *menu) {
    image = NULL;
    cached_image_dl = NULL;
}

void view_image_viewer_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);

    if (image == NULL) {
        deffered_image_load(menu, display);
    }

    if (menu->next_mode != MENU_MODE_IMAGE_VIEWER) {
        deinit(menu);
    }
}
