#include <stdlib.h>

#include "../components.h"
#include "../path.h"
#include "../png_decoder.h"
#include "constants.h"
#include "utils/fs.h"


#define BOXART_DIRECTORY    "menu/boxart"


static void png_decoder_callback (png_err_t err, surface_t *decoded_image, void *callback_data) {
    component_boxart_t *b = (component_boxart_t *) (callback_data);
    b->loading = false;
    b->image = decoded_image;
}


component_boxart_t *component_boxart_init (const char *storage_prefix, char *game_code) {
    component_boxart_t *b;
    char file_name[8];

    if ((b = calloc(1, sizeof(component_boxart_t))) == NULL) {
        return NULL;
    }

    b->loading = true;

    path_t *path = path_init(storage_prefix, BOXART_DIRECTORY);

    sprintf(file_name, "%.3s.png", game_code);
    path_push(path, file_name);
    if (png_decoder_start(path_get(path), BOXART_WIDTH_MAX, BOXART_HEIGHT_MAX, png_decoder_callback, b) == PNG_OK) {
        path_free(path);
        return b;
    }
    path_pop(path);

    // TODO: This is bad, we should only check for 3 letter codes
    sprintf(file_name, "%.2s.png", game_code + 1);
    path_push(path, file_name);
    if (png_decoder_start(path_get(path), BOXART_WIDTH_MAX, BOXART_HEIGHT_MAX, png_decoder_callback, b) == PNG_OK) {
        path_free(path);
        return b;
    }

    path_free(path);
    free(b);

    return NULL;
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
    if (b && b->image  && b->image->width <= BOXART_WIDTH_MAX && b->image->height <= BOXART_HEIGHT_MAX) {
        rdpq_mode_push();
            rdpq_set_mode_copy(false);
            if(b->image->height == BOXART_HEIGHT_MAX)
            {
                rdpq_tex_blit(
                    b->image,
                    BOXART_X_JP,
                    BOXART_Y_JP,
                    NULL
                );
            }
            else if(b->image->width == BOXART_WIDTH_DD && b->image->height == BOXART_HEIGHT_DD )
            {
                rdpq_tex_blit(
                    b->image,
                    BOXART_X_DD,
                    BOXART_Y_DD,
                    NULL
                );
            }
            else
            {
                rdpq_tex_blit(
                    b->image,
                    BOXART_X,
                    BOXART_Y,
                    NULL
                );
            }
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