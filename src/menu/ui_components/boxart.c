/**
 * @file boxart.c
 * @brief Boxart component implementation
 * @ingroup ui_components
 */

#include <stdlib.h>

#include "../ui_components.h"
#include "../path.h"
#include "../png_decoder.h"
#include "constants.h"
#include "utils/fs.h"

#define BOXART_DIRECTORY    "menu/boxart"

/**
 * @brief PNG decoder callback function.
 * 
 * @param err PNG decoder error code.
 * @param decoded_image Pointer to the decoded image surface.
 * @param callback_data Pointer to the callback data.
 */
static void png_decoder_callback (png_err_t err, surface_t *decoded_image, void *callback_data) {
    component_boxart_t *b = (component_boxart_t *) (callback_data);
    b->loading = false;
    b->image = decoded_image;
}

/**
 * @brief Initialize the boxart component.
 * 
 * @param storage_prefix The storage prefix.
 * @param game_code The game code.
 * @param current_image_view The current image view type.
 * @return component_boxart_t* Pointer to the initialized boxart component.
 */
component_boxart_t *ui_components_boxart_init (const char *storage_prefix, char *game_code, file_image_type_t current_image_view) {
    component_boxart_t *b;
    char boxart_id_path[8];

    if ((b = calloc(1, sizeof(component_boxart_t))) == NULL) {
        return NULL;
    }

    b->loading = true;

    path_t *path = path_init(storage_prefix, BOXART_DIRECTORY);

    sprintf(boxart_id_path, "%c/%c/%c/%c", game_code[0], game_code[1], game_code[2], game_code[3]);
    path_push(path, boxart_id_path);

    if (!directory_exists(path_get(path))) { // Allow boxart to not specify the region code.
        path_pop(path);
    }

    if (directory_exists(path_get(path))) {
        switch (current_image_view) {
            case IMAGE_GAMEPAK_FRONT:
                path_push(path, "gamepak_front.png");
                break;
            case IMAGE_GAMEPAK_BACK:
                path_push(path, "gamepak_back.png");
                break;
            case IMAGE_BOXART_BACK:
                path_push(path, "boxart_back.png");
                break;
            case IMAGE_BOXART_LEFT:
                path_push(path, "boxart_left.png");
                break;
            case IMAGE_BOXART_RIGHT:
                path_push(path, "boxart_right.png");
                break;
            case IMAGE_BOXART_BOTTOM:
                path_push(path, "boxart_bottom.png");
                break;
            case IMAGE_BOXART_TOP:
                path_push(path, "boxart_top.png");
                break;
            default:
                path_push(path, "boxart_front.png");
                break;
        }

        if (file_exists(path_get(path))) { 
            if (png_decoder_start(path_get(path), BOXART_WIDTH_MAX, BOXART_HEIGHT_MAX, png_decoder_callback, b) == PNG_OK) {
                path_free(path);
                return b;
            }
        }
    } else { // compatibility mode

        char file_name[9];

        // reset the directory path used for boxart.
        path_free(path);
        path = path_init(storage_prefix, BOXART_DIRECTORY);

        snprintf(file_name, sizeof(file_name), "%c%c%c%c.png", game_code[0], game_code[1], game_code[2], game_code[3]);
        path_push(path, file_name);

        if (file_exists(path_get(path))) {
            if (png_decoder_start(path_get(path), BOXART_WIDTH_MAX, BOXART_HEIGHT_MAX, png_decoder_callback, b) == PNG_OK) {
                path_free(path);
                return b;
            }
        }

        path_pop(path);
        snprintf(file_name, sizeof(file_name), "%c%c%c.png", game_code[0], game_code[1], game_code[2]);
        path_push(path, file_name);

        if (file_exists(path_get(path))) {
            if (png_decoder_start(path_get(path), BOXART_WIDTH_MAX, BOXART_HEIGHT_MAX, png_decoder_callback, b) == PNG_OK) {
                path_free(path);
                return b;
            }
        } else {
            path_pop(path);

            snprintf(file_name, sizeof(file_name), "%c%c.png", game_code[1], game_code[2]);
            path_push(path, file_name);
            if (file_exists(path_get(path))) {
                if (png_decoder_start(path_get(path), BOXART_WIDTH_MAX, BOXART_HEIGHT_MAX, png_decoder_callback, b) == PNG_OK) {
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

/**
 * @brief Free the boxart component.
 * 
 * @param b Pointer to the boxart component.
 */
void ui_components_boxart_free (component_boxart_t *b) {
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

/**
 * @brief Draw the boxart component.
 * 
 * @param b Pointer to the boxart component.
 */
void ui_components_boxart_draw (component_boxart_t *b) {
    int box_x = BOXART_X;
    int box_y = BOXART_Y;

    if (b && b->image && b->image->width <= BOXART_WIDTH_MAX && b->image->height <= BOXART_HEIGHT_MAX) {
        rdpq_mode_push();
            rdpq_set_mode_copy(false);
            if (b->image->height == BOXART_HEIGHT_MAX) {
                box_x = BOXART_X_JP;
                box_y = BOXART_Y_JP;
            } else if (b->image->width == BOXART_WIDTH_DD && b->image->height == BOXART_HEIGHT_DD) {
                box_x = BOXART_X_DD;
                box_y = BOXART_Y_DD;
            }
            rdpq_tex_blit(b->image, box_x, box_y, NULL);
        rdpq_mode_pop();
    } else {
        ui_components_box_draw(
            BOXART_X,
            BOXART_Y,
            BOXART_X + BOXART_WIDTH,
            BOXART_Y + BOXART_HEIGHT,
            BOXART_LOADING_COLOR
        );
    }
}
