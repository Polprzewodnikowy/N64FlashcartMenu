#include <stdio.h>
#include <stdlib.h>

#include "../ui_components.h"
#include "constants.h"
#include "utils/fs.h"


#define CACHE_METADATA_MAGIC    (0x424B4731)


typedef struct {
    char *cache_location;
    surface_t *image;
    rspq_block_t *image_display_list;
} component_background_t;

typedef struct {
    uint32_t magic;
    uint32_t width;
    uint32_t height;
    uint32_t size;
} cache_metadata_t;


static component_background_t *background = NULL;


static void load_from_cache (component_background_t *c) {
    if (!c->cache_location) {
        return;
    }

    FILE *f;

    if ((f = fopen(c->cache_location, "rb")) == NULL) {
        return;
    }

    cache_metadata_t cache_metadata;

    if (fread(&cache_metadata, sizeof(cache_metadata), 1, f) != 1) {
        fclose(f);
        return;
    }

    if (cache_metadata.magic != CACHE_METADATA_MAGIC || cache_metadata.width > DISPLAY_WIDTH || cache_metadata.height > DISPLAY_HEIGHT) {
        fclose(f);
        return;
    }

    c->image = calloc(1, sizeof(surface_t));
    *c->image = surface_alloc(FMT_RGBA16, cache_metadata.width, cache_metadata.height);

    if (cache_metadata.size != (c->image->height * c->image->stride)) {
        surface_free(c->image);
        free(c->image);
        c->image = NULL;
        fclose(f);
        return;
    }

    if (fread(c->image->buffer, cache_metadata.size, 1, f) != 1) {
        surface_free(c->image);
        free(c->image);
        c->image = NULL;
    }

    fclose(f);
}

static void save_to_cache (component_background_t *c) {
    if (!c->cache_location || !c->image) {
        return;
    }

    FILE *f;

    if ((f = fopen(c->cache_location, "wb")) == NULL) {
        return;
    }

    cache_metadata_t cache_metadata = {
        .magic = CACHE_METADATA_MAGIC,
        .width = c->image->width,
        .height = c->image->height,
        .size = (c->image->height * c->image->stride),
    };

    fwrite(&cache_metadata, sizeof(cache_metadata), 1, f);
    fwrite(c->image->buffer, cache_metadata.size, 1, f);

    fclose(f);
}

static void prepare_background (component_background_t *c) {
    if (!c->image || c->image->width == 0 || c->image->height == 0) {
        return;
    }

    // Darken the image
    rdpq_attach(c->image, NULL);
    rdpq_mode_push();
        rdpq_set_mode_standard();
        rdpq_set_prim_color(BACKGROUND_OVERLAY_COLOR);
        rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
        rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
        rdpq_fill_rectangle(0, 0, c->image->width, c->image->height);
    rdpq_mode_pop();
    rdpq_detach();

    uint16_t image_center_x = (c->image->width / 2);
    uint16_t image_center_y = (c->image->height / 2);

    // Prepare display list
    rspq_block_begin();
    rdpq_mode_push();
        if ((c->image->width != DISPLAY_WIDTH) || (c->image->height != DISPLAY_HEIGHT)) {
            rdpq_set_mode_fill(BACKGROUND_EMPTY_COLOR);
        }
        if (c->image->width != DISPLAY_WIDTH) {
            rdpq_fill_rectangle(
                0,
                DISPLAY_CENTER_Y - image_center_y,
                DISPLAY_CENTER_X - image_center_x,
                DISPLAY_CENTER_Y + image_center_y
            );
            rdpq_fill_rectangle(
                DISPLAY_CENTER_X + image_center_x - (c->image->width % 2),
                DISPLAY_CENTER_Y - image_center_y,
                DISPLAY_WIDTH,
                DISPLAY_CENTER_Y + image_center_y
            );
        }
        if (c->image->height != DISPLAY_HEIGHT) {
            rdpq_fill_rectangle(
                0,
                0,
                DISPLAY_WIDTH,
                DISPLAY_CENTER_Y - image_center_y
            );
            rdpq_fill_rectangle(
                0,
                DISPLAY_CENTER_Y + image_center_y - (c->image->height % 2),
                DISPLAY_WIDTH,
                DISPLAY_HEIGHT
            );
        }
        rdpq_set_mode_copy(false);
        rdpq_tex_blit(c->image, DISPLAY_CENTER_X - image_center_x, DISPLAY_CENTER_Y - image_center_y, NULL);
    rdpq_mode_pop();
    c->image_display_list = rspq_block_end();
}

static void display_list_free (void *arg) {
    rspq_block_free((rspq_block_t *) (arg));
}


void ui_components_background_init (char *cache_location) {
    if (!background) {
        background = calloc(1, sizeof(component_background_t));
        background->cache_location = strdup(cache_location);
        load_from_cache(background);
        prepare_background(background);
    }
}

void ui_components_background_free (void) {
    if (background) {
        if (background->image) {
            surface_free(background->image);
            free(background->image);
            background->image = NULL;
        }
        if (background->image_display_list) {
            rdpq_call_deferred(display_list_free, background->image_display_list);
            background->image_display_list = NULL;
        }
        if (background->cache_location) {
            free(background->cache_location);
        }
        free(background);
        background = NULL;
    }
}

void ui_components_background_replace_image (surface_t *image) {
    if (!background) {
        return;
    }

    if (background->image) {
        surface_free(background->image);
        free(background->image);
        background->image = NULL;
    }

    if (background->image_display_list) {
        rdpq_call_deferred(display_list_free, background->image_display_list);
        background->image_display_list = NULL;
    }

    background->image = image;
    save_to_cache(background);
    prepare_background(background);
}

void ui_components_background_draw (void) {
    if (background && background->image_display_list) {
        rspq_block_run(background->image_display_list);
    } else {
        rdpq_clear(BACKGROUND_EMPTY_COLOR);
    }
}
