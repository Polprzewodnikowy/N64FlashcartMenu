/**
 * @file grid.c
 * @brief Grid view component implementation
 * @ingroup ui_components
 */

#include <stdlib.h>
#include <string.h>

#include "../ui_components.h"
#include "../fonts.h"
#include "../path.h"
#include "../png_decoder.h"
#include "constants.h"
#include "utils/fs.h"

#define BOXART_DIRECTORY    "menu/boxart"

/**
 * @brief Grid component structure
 */
typedef struct {
    component_boxart_t *thumbnails[GRID_ITEMS_PER_PAGE];
    int loaded_page;
    bool loading;
} component_grid_t;

static component_grid_t grid_state = {
    .thumbnails = {NULL},
    .loaded_page = -1,
    .loading = false
};

/**
 * @brief PNG decoder callback function for grid thumbnails
 */
static void grid_png_callback (png_err_t err, surface_t *decoded_image, void *callback_data) {
    component_boxart_t *b = (component_boxart_t *) (callback_data);
    b->loading = false;
    b->image = decoded_image;
}

/**
 * @brief Load a single thumbnail for a grid cell
 */
static component_boxart_t *load_grid_thumbnail (const char *storage_prefix, char *game_code) {
    component_boxart_t *b;
    char boxart_id_path[8];

    if ((b = calloc(1, sizeof(component_boxart_t))) == NULL) {
        return NULL;
    }

    b->loading = true;

    path_t *path = path_init(storage_prefix, BOXART_DIRECTORY);

    sprintf(boxart_id_path, "%c/%c/%c/%c", game_code[0], game_code[1], game_code[2], game_code[3]);
    path_push(path, boxart_id_path);

    if (!directory_exists(path_get(path))) {
        path_pop(path);
    }

    if (directory_exists(path_get(path))) {
        path_push(path, "boxart_front.png");

        if (file_exists(path_get(path))) {
            if (png_decoder_start(path_get(path), GRID_IMAGE_WIDTH, GRID_IMAGE_HEIGHT, grid_png_callback, b) == PNG_OK) {
                path_free(path);
                return b;
            }
        }
    } else {
        // Compatibility mode
        char file_name[9];
        path_free(path);
        path = path_init(storage_prefix, BOXART_DIRECTORY);

        snprintf(file_name, sizeof(file_name), "%c%c%c%c.png", game_code[0], game_code[1], game_code[2], game_code[3]);
        path_push(path, file_name);

        if (file_exists(path_get(path))) {
            if (png_decoder_start(path_get(path), GRID_IMAGE_WIDTH, GRID_IMAGE_HEIGHT, grid_png_callback, b) == PNG_OK) {
                path_free(path);
                return b;
            }
        }
    }

    path_free(path);
    b->loading = false;
    return b;
}

/**
 * @brief Free grid thumbnails
 */
void ui_components_grid_free (void) {
    for (int i = 0; i < GRID_ITEMS_PER_PAGE; i++) {
        if (grid_state.thumbnails[i]) {
            if (grid_state.thumbnails[i]->loading) {
                png_decoder_abort();
            }
            if (grid_state.thumbnails[i]->image) {
                surface_free(grid_state.thumbnails[i]->image);
            }
            free(grid_state.thumbnails[i]);
            grid_state.thumbnails[i] = NULL;
        }
    }
    grid_state.loaded_page = -1;
}

/**
 * @brief Load thumbnails for a specific page
 */
void ui_components_grid_load_page (const char *storage_prefix, entry_t *list, int entries, int page) {
    if (grid_state.loaded_page == page) {
        return;
    }

    ui_components_grid_free();

    int start_index = page * GRID_ITEMS_PER_PAGE;
    int end_index = start_index + GRID_ITEMS_PER_PAGE;
    if (end_index > entries) {
        end_index = entries;
    }

    for (int i = start_index; i < end_index; i++) {
        entry_t *entry = &list[i];
        int grid_index = i - start_index;

        // Only load thumbnails for ROM/Disk/Emulator types
        if (entry->type == ENTRY_TYPE_ROM || entry->type == ENTRY_TYPE_DISK || entry->type == ENTRY_TYPE_EMULATOR) {
            // Extract game code from filename (first 4 characters before extension)
            char game_code[5] = {0};
            strncpy(game_code, entry->name, 4);

            grid_state.thumbnails[grid_index] = load_grid_thumbnail(storage_prefix, game_code);
        }
    }

    grid_state.loaded_page = page;
}

/**
 * @brief Draw the grid view component
 */
void ui_components_grid_draw (entry_t *list, int entries, int selected, int current_page, int grid_row, int grid_col) {
    int start_index = current_page * GRID_ITEMS_PER_PAGE;
    int total_pages = (entries + GRID_ITEMS_PER_PAGE - 1) / GRID_ITEMS_PER_PAGE;

    // Draw each grid cell
    for (int row = 0; row < GRID_ROWS; row++) {
        for (int col = 0; col < GRID_COLS; col++) {
            int grid_index = row * GRID_COLS + col;
            int entry_index = start_index + grid_index;

            if (entry_index >= entries) {
                continue;
            }

            entry_t *entry = &list[entry_index];

            int cell_x = GRID_START_X + col * (GRID_CELL_WIDTH + GRID_SPACING_X);
            int cell_y = GRID_START_Y + row * (GRID_CELL_HEIGHT + GRID_SPACING_Y);

            // Draw cell background
            rdpq_set_mode_fill(GRID_CELL_BG_COLOR);
            rdpq_fill_rectangle(cell_x, cell_y, cell_x + GRID_CELL_WIDTH, cell_y + GRID_CELL_HEIGHT);

            // Calculate centered position for thumbnail
            int thumb_x = cell_x + (GRID_CELL_WIDTH - GRID_IMAGE_WIDTH) / 2;
            int thumb_y = cell_y + 4;

            // Draw thumbnail if available
            component_boxart_t *thumbnail = grid_state.thumbnails[grid_index];
            if (thumbnail) {
                if (thumbnail->loading) {
                    // Draw loading indicator
                    rdpq_set_mode_fill(BOXART_LOADING_COLOR);
                    rdpq_fill_rectangle(thumb_x, thumb_y, thumb_x + GRID_IMAGE_WIDTH, thumb_y + GRID_IMAGE_HEIGHT);
                } else if (thumbnail->image) {
                    // Draw the thumbnail image
                    rdpq_set_mode_standard();
                    rdpq_tex_blit(thumbnail->image, thumb_x, thumb_y, NULL);
                } else {
                    // Draw placeholder for missing boxart
                    rdpq_set_mode_fill(RGBA32(0x40, 0x40, 0x40, 0xFF));
                    rdpq_fill_rectangle(thumb_x, thumb_y, thumb_x + GRID_IMAGE_WIDTH, thumb_y + GRID_IMAGE_HEIGHT);
                }
            } else {
                // No thumbnail object, draw placeholder
                rdpq_set_mode_fill(RGBA32(0x40, 0x40, 0x40, 0xFF));
                rdpq_fill_rectangle(thumb_x, thumb_y, thumb_x + GRID_IMAGE_WIDTH, thumb_y + GRID_IMAGE_HEIGHT);
            }

            // Draw game title below thumbnail (truncated if needed)
            rdpq_textparms_t textparms = {
                .width = GRID_CELL_WIDTH - 8,
                .align = ALIGN_CENTER,
            };

            // Remove file extension from display name
            char display_name[256];
            strncpy(display_name, entry->name, sizeof(display_name) - 1);
            display_name[sizeof(display_name) - 1] = '\0';
            char *dot = strrchr(display_name, '.');
            if (dot) {
                *dot = '\0';
            }

            rdpq_text_printn(&textparms, FONT_BUILTIN_DEBUG, cell_x + 4, cell_y + GRID_CELL_HEIGHT - 16, display_name, 24);

            // Draw selection highlight
            if (row == grid_row && col == grid_col) {
                ui_components_border_draw(cell_x - 2, cell_y - 2, cell_x + GRID_CELL_WIDTH + 2, cell_y + GRID_CELL_HEIGHT + 2);
            }
        }
    }

    // Draw page indicator
    if (total_pages > 1) {
        char page_text[32];
        snprintf(page_text, sizeof(page_text), "Page %d/%d", current_page + 1, total_pages);

        rdpq_textparms_t textparms = {
            .align = ALIGN_CENTER,
        };
        rdpq_text_print(&textparms, FONT_BUILTIN_DEBUG, DISPLAY_CENTER_X, LAYOUT_ACTIONS_SEPARATOR_Y - 20, page_text);
    }
}
