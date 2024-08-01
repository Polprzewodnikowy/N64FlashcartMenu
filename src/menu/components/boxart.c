#include <stdlib.h>

#include "../components.h"
#include "../path.h"
#include "../png_decoder.h"
#include "constants.h"
#include "utils/fs.h"


#define BOXART_DIRECTORY    "menu/boxart"

/** @brief ROM Boxart Enumeration. */
typedef enum {
    // Image from the front
    BOXART_IMAGE_FRONT,

    // Image from the back
    BOXART_IMAGE_BACK,

    // Image from the top
    BOXART_IMAGE_TOP,

    // Image from the bottom
    BOXART_IMAGE_BOTTOM,

    // Image from the left side
    BOXART_IMAGE_LEFT,

    // Image from the right side
    BOXART_IMAGE_RIGHT,

    // List end marker
    BOXART_TYPE_END
} boxart_type_t;


static void png_decoder_callback (png_err_t err, surface_t *decoded_image, void *callback_data) {
    component_boxart_t *b = (component_boxart_t *) (callback_data);
    b->loading = false;
    b->image = decoded_image;
}


component_boxart_t *component_boxart_init (const char *storage_prefix, char *game_code) {
    component_boxart_t *b;
    char boxart_id_path[8];

    if ((b = calloc(1, sizeof(component_boxart_t))) == NULL) {
        return NULL;
    }

    b->loading = true;

    path_t *path = path_init(storage_prefix, BOXART_DIRECTORY);

    sprintf(boxart_id_path, "%c/%c/%c/%c", game_code[0], game_code[1], game_code[2], game_code[3]);
    path_push(path, boxart_id_path);
    if (directory_exists(path_get(path))) {
        // if (back_art) {
        //   path_push(path, "back.png");
        // }
        // else if (left_side_art) {
        //   path_push(path, "left.png");
        // }
        // else if (right_side_art) {
        //   path_push(path, "right.png");
        // }
        // else if (bottom_side_art {
        //   path_push(path, "bottom.png");
        // }
        // else if (top_side_art {
        //   path_push(path, "top.png");
        // }
        // else {
        path_push(path, "front.png");
        // }
        if (file_exists(path_get(path))) { 
            if (png_decoder_start(path_get(path), BOXART_WIDTH, BOXART_HEIGHT, png_decoder_callback, b) == PNG_OK) {
                path_free(path);
                return b;
            }
        }
    }
    else { // compatibility mode

        //TODO: this could be path_pop one by one to find the generic image?!
        char file_name[8];

        // undo the dir path used for the boxart previously.
        path_pop(path);
        path_pop(path);
        path_pop(path);
        path_pop(path);

        sprintf(file_name, "%c%c%c.png", game_code[0], game_code[1], game_code[2]);
        path_push(path, file_name);
        if (file_exists(path_get(path))) {      
            if (png_decoder_start(path_get(path), BOXART_WIDTH, BOXART_HEIGHT, png_decoder_callback, b) == PNG_OK) {
                path_free(path);
                return b;
            }
        }
        else {
            path_pop(path);

            sprintf(file_name, "%c%c.png", game_code[1], game_code[2]);
            path_push(path, file_name);
            if (file_exists(path_get(path))) {
                if (png_decoder_start(path_get(path), BOXART_WIDTH, BOXART_HEIGHT, png_decoder_callback, b) == PNG_OK) {
                    path_free(path);
                    return b;
                }
            }
        }
    }
    // TODO: return default image.

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
