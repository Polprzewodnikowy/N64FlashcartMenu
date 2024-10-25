/**
 * @file components.h
 * @brief Menu Components
 * @ingroup menu
 */

#ifndef COMPONENTS_H__
#define COMPONENTS_H__


#include <libdragon.h>
#include "menu_state.h"

/** @brief File image Enumeration. */
typedef enum {

    /** @brief Boxart image from the front */
    IMAGE_BOXART_FRONT,

    /** @brief Boxart image from the back */
    IMAGE_BOXART_BACK,

    /** @brief Boxart image from the top */
    IMAGE_BOXART_TOP,

    /** @brief Boxart image from the bottom */
    IMAGE_BOXART_BOTTOM,

    /** @brief Boxart image from the left side */
    IMAGE_BOXART_LEFT,

    /** @brief Boxart image from the right side */
    IMAGE_BOXART_RIGHT,

    /** @brief GamePak image from the front */
    IMAGE_GAMEPAK_FRONT,

    /** @brief GamePak image from the back */
    IMAGE_GAMEPAK_BACK,

    /** @brief File image thumbnail */
    IMAGE_THUMBNAIL,

    /** @brief List end marker */
    IMAGE_TYPE_END

} file_image_type_t;


/**
 * @addtogroup 
 * @{ menu_components
 */

void component_box_draw (int x0, int y0, int x1, int y1, color_t color);
void component_border_draw (int x0, int y0, int x1, int y1);
void component_layout_draw (void);
void component_progressbar_draw (int x0, int y0, int x1, int y1, float progress);
void component_seekbar_draw (float progress);
void component_loader_draw (float position);
void component_scrollbar_draw (int x, int y, int width, int height, int position, int items, int visible_items);
void component_list_scrollbar_draw (int position, int items, int visible_items);
void component_dialog_draw (int width, int height);
void component_messagebox_draw (char *fmt, ...);
void component_main_text_draw (rdpq_align_t align, rdpq_valign_t valign, char *fmt, ...);
void component_actions_bar_text_draw (rdpq_align_t align, rdpq_valign_t valign, char *fmt, ...);

void component_background_init (char *cache_location);
void component_background_free (void);
void component_background_replace_image (surface_t *image);
void component_background_draw (void);

void component_file_list_draw (entry_t *list, int entries, int selected);

typedef struct component_context_menu {
    int row_count;
    int row_selected;
    bool hide_pending;
    struct component_context_menu *parent;
    struct component_context_menu *submenu;
    struct {
        const char *text;
        void (*action) (menu_t *menu, void *arg);
        void *arg;
        struct component_context_menu *submenu;
    } list[];
} component_context_menu_t;

#define COMPONENT_CONTEXT_MENU_LIST_END     { .text = NULL }

void component_context_menu_init (component_context_menu_t *cm);
void component_context_menu_show (component_context_menu_t *cm);
bool component_context_menu_process (menu_t *menu, component_context_menu_t *cm);
void component_context_menu_draw (component_context_menu_t *cm);

/** @brief Box Art Structure. */
typedef struct {
    bool loading;
    surface_t *image;
} component_boxart_t;

component_boxart_t *component_boxart_init (const char *storage_prefix, char *game_code, file_image_type_t current_image_view);
void component_boxart_free (component_boxart_t *b);
void component_boxart_draw (component_boxart_t *b);

/** @} */ /* menu_components */


#endif
