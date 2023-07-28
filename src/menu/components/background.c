#include <fatfs/ff.h>
#include <stdlib.h>

#include "components.h"
#include "utils/fs.h"


#define CACHE_METADATA_MAGIC    (0x424B4731)


typedef struct {
    uint32_t magic;
    uint32_t width;
    uint32_t height;
    uint32_t size;
} cache_metadata_t;


static void load_from_cache (component_background_t *c) {
    if (!c->cache_location) {
        return;
    }

    FIL fil;
    UINT bytes_read;

    if (f_open(&fil, strip_sd_prefix(c->cache_location), FA_READ) != FR_OK) {
        return;
    }

    cache_metadata_t cache_metadata;

    if ((f_read(&fil, &cache_metadata, sizeof(cache_metadata), &bytes_read) != FR_OK) || (bytes_read != sizeof(cache_metadata))) {
        f_close(&fil);
        return;
    }

    uint32_t display_width = display_get_width();
    uint32_t display_height = display_get_height();

    if (cache_metadata.magic != CACHE_METADATA_MAGIC || cache_metadata.width > display_width || cache_metadata.height > display_height) {
        f_close(&fil);
        return;
    }

    c->image = calloc(1, sizeof(surface_t));
    *c->image = surface_alloc(FMT_RGBA16, cache_metadata.width, cache_metadata.height);

    if (cache_metadata.size != (c->image->height * c->image->stride)) {
        surface_free(c->image);
        free(c->image);
        c->image = NULL;
        f_close(&fil);
        return;
    }

    if ((f_read(&fil, c->image->buffer, cache_metadata.size, &bytes_read) != FR_OK) || (bytes_read != cache_metadata.size)) {
        surface_free(c->image);
        free(c->image);
        c->image = NULL;
    }

    f_close(&fil);
}

static void save_to_cache (component_background_t *c) {
    if (!c->cache_location || !c->image) {
        return;
    }

    FIL fil;
    UINT bytes_written;
    if (f_open(&fil, strip_sd_prefix(c->cache_location), FA_WRITE | FA_CREATE_ALWAYS) != FR_OK) {
        return;
    }

    cache_metadata_t cache_metadata = {
        .magic = CACHE_METADATA_MAGIC,
        .width = c->image->width,
        .height = c->image->height,
        .size = (c->image->height * c->image->stride),
    };

    f_write(&fil, &cache_metadata, sizeof(cache_metadata), &bytes_written);
    f_write(&fil, c->image->buffer, cache_metadata.size, &bytes_written);

    f_close(&fil);
}

static void prepare_background (component_background_t *c) {
    if (!c->image || c->image->width == 0 || c->image->height == 0) {
        return;
    }

    uint32_t display_width = display_get_width();
    uint32_t display_height = display_get_height();

    int16_t display_center_x = (display_width / 2);
    int16_t display_center_y = (display_height / 2);

    int16_t image_center_x = (c->image->width / 2);
    int16_t image_center_y = (c->image->height / 2);

    // Darken the image
    rdpq_attach(c->image, NULL);
    rdpq_mode_push();
        rdpq_set_mode_standard();
        rdpq_set_prim_color(RGBA32(0x00, 0x00, 0x00, 0xA0));
        rdpq_mode_combiner(RDPQ_COMBINER_FLAT);
        rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
        rdpq_fill_rectangle(0, 0, c->image->width, c->image->height);
    rdpq_mode_pop();
    rdpq_detach();

    // Prepare display list
    rspq_block_begin();
    rdpq_mode_push();
        if ((c->image->width != display_width) || (c->image->height != display_height)) {
            rdpq_set_mode_fill(RGBA32(0x00, 0x00, 0x00, 0xFF));
        }
        if (c->image->width != display_width) {
            rdpq_fill_rectangle(
                0,
                display_center_y - image_center_y,
                display_center_x - image_center_x,
                display_center_y + image_center_y
            );
            rdpq_fill_rectangle(
                display_center_x + image_center_x - (c->image->width % 2),
                display_center_y - image_center_y,
                display_width,
                display_center_y + image_center_y
            );
        }
        if (c->image->height != display_height) {
            rdpq_fill_rectangle(
                0,
                0,
                display_width,
                display_center_y - image_center_y
            );
            rdpq_fill_rectangle(
                0,
                display_center_y + image_center_y - (c->image->height % 2),
                display_width,
                display_height
            );
        }
        rdpq_set_mode_copy(false);
        rdpq_tex_blit(c->image, display_center_x - image_center_x, display_center_y - image_center_y, NULL);
    rdpq_mode_pop();
    c->image_display_list = rspq_block_end();
}

static void display_list_free (void *arg) {
    rspq_block_free((rspq_block_t *) (arg));
}


component_background_t *component_background_create (char *cache_location) {
    component_background_t *c = calloc(1, sizeof(component_background_t));

    c->cache_location = cache_location;

    load_from_cache(c);

    prepare_background(c);

    return c;
}

void component_background_replace_image (component_background_t *c, surface_t *image) {
    if (!c) {
        return;
    }

    if (c->image) {
        surface_free(c->image);
        free(c->image);
        c->image = NULL;
    }

    if (c->image_display_list) {
        rdpq_call_deferred(display_list_free, c->image_display_list);
        c->image_display_list = NULL;
    }

    c->image = image;

    save_to_cache(c);

    prepare_background(c);
}

void component_background_draw (component_background_t *c) {
    if (!c || !c->image_display_list) {
        rdpq_clear(RGBA32(0x00, 0x00, 0x00, 0xFF));
        return;
    }

    rspq_block_run(c->image_display_list);
}
