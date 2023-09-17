#include <stdlib.h>

#include "../components.h"
#include "../path.h"
#include "../png_decoder.h"
#include "constants.h"
#include "utils/fs.h"


#define BOXART_DIRECTORY    "sd:/menu/boxart"


static void png_decoder_callback (png_err_t err, surface_t *decoded_image, void *callback_data) {
    component_boxart_t *b = (component_boxart_t *) (callback_data);
    b->loading = false;
    b->image = decoded_image;
}


component_boxart_t *component_boxart_init (uint8_t media_type, uint16_t id) {
    component_boxart_t *b = calloc(1, sizeof(component_boxart_t));

    if (b) {
        b->loading = true;
        char *path = alloca(strlen(BOXART_DIRECTORY) + 1 + 7 + 1); // allocate for the largest path.
        sprintf(path, "%s/%c%c%c.png", BOXART_DIRECTORY, (media_type & 0xFF), ((id >> 8) & 0xFF), (id & 0xFF));

        // if the file does not exist, also check for just the id.
        if (!file_exists(path)) {
            sprintf(path, "%s/%c%c.png", BOXART_DIRECTORY, ((id >> 8) & 0xFF), (id & 0xFF));
        }

        if (png_decoder_start(path, BOXART_WIDTH, BOXART_HEIGHT, png_decoder_callback, b) != PNG_OK) {
            free(b);
            b = NULL;
        }
    }

    return b;
}

void component_boxart_free (component_boxart_t *b) {
    if (b) {
        if (b->loading) {
            png_decoder_abort();
        }
        if (b->image) {
            surface_free(b->image);
            free(b->image);
        }
        free(b);
    }
}

void component_boxart_draw (component_boxart_t *b) {
    if (b && b->image && b->image->width == BOXART_WIDTH && b->image->height == BOXART_HEIGHT) {
        rdpq_mode_push();
            rdpq_set_mode_copy(false);
            rdpq_tex_blit(
                b->image,
                BOXART_X,
                BOXART_Y,
                NULL
            );
        rdpq_mode_pop();
    } else {
        component_box_draw(
            BOXART_X,
            BOXART_Y,
            BOXART_X + BOXART_WIDTH,
            BOXART_Y + BOXART_HEIGHT,
            BOXART_LOADING_COLOR
        );
    }
}
