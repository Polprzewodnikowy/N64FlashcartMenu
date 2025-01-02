/**
 * @file ui_components.h
 * @brief Menu Graphical User Interface Components
 * @ingroup menu
 */

#ifndef UI_COMPONENTS_H__
#define UI_COMPONENTS_H__

#include <libdragon.h>
#include "menu_state.h"


/** 
 * @brief File image Enumeration.
 * 
 * Enumeration for different types of file images used in the user interface.
 */
typedef enum {
    IMAGE_BOXART_FRONT,    /**< Boxart image from the front */
    IMAGE_BOXART_BACK,     /**< Boxart image from the back */
    IMAGE_BOXART_TOP,      /**< Boxart image from the top */
    IMAGE_BOXART_BOTTOM,   /**< Boxart image from the bottom */
    IMAGE_BOXART_LEFT,     /**< Boxart image from the left side */
    IMAGE_BOXART_RIGHT,    /**< Boxart image from the right side */
    IMAGE_GAMEPAK_FRONT,   /**< GamePak image from the front */
    IMAGE_GAMEPAK_BACK,    /**< GamePak image from the back */
    IMAGE_THUMBNAIL,       /**< File image thumbnail */
    IMAGE_TYPE_END         /**< List end marker */
} file_image_type_t;

/**
 * @brief Draw a box component.
 * 
 * @param x0 Starting x-coordinate.
 * @param y0 Starting y-coordinate.
 * @param x1 Ending x-coordinate.
 * @param y1 Ending y-coordinate.
 * @param color Color of the box.
 */
void ui_components_box_draw(int x0, int y0, int x1, int y1, color_t color);

/**
 * @brief Draw a border component.
 * 
 * @param x0 Starting x-coordinate.
 * @param y0 Starting y-coordinate.
 * @param x1 Ending x-coordinate.
 * @param y1 Ending y-coordinate.
 */
void ui_components_border_draw(int x0, int y0, int x1, int y1);

/**
 * @brief Draw the layout component.
 */
void ui_components_layout_draw(void);

/**
 * @brief Draw a progress bar component.
 * 
 * @param x0 Starting x-coordinate.
 * @param y0 Starting y-coordinate.
 * @param x1 Ending x-coordinate.
 * @param y1 Ending y-coordinate.
 * @param progress Progress value (0.0 to 1.0).
 */
void ui_components_progressbar_draw(int x0, int y0, int x1, int y1, float progress);

/**
 * @brief Draw a seek bar component.
 * 
 * @param progress Progress value (0.0 to 1.0).
 */
void ui_components_seekbar_draw(float progress);

/**
 * @brief Draw a loader component.
 * 
 * @param position Position value (0.0 to 1.0).
 */
void ui_components_loader_draw(float position);

/**
 * @brief Draw a scrollbar component.
 * 
 * @param x Starting x-coordinate.
 * @param y Starting y-coordinate.
 * @param width Width of the scrollbar.
 * @param height Height of the scrollbar.
 * @param position Current position.
 * @param items Total number of items.
 * @param visible_items Number of visible items.
 */
void ui_components_scrollbar_draw(int x, int y, int width, int height, int position, int items, int visible_items);

/**
 * @brief Draw a list scrollbar component.
 * 
 * @param position Current position.
 * @param items Total number of items.
 * @param visible_items Number of visible items.
 */
void ui_components_list_scrollbar_draw(int position, int items, int visible_items);

/**
 * @brief Draw a dialog component.
 * 
 * @param width Width of the dialog.
 * @param height Height of the dialog.
 */
void ui_components_dialog_draw(int width, int height);

/**
 * @brief Draw a message box component.
 * 
 * @param fmt Format string for the message.
 * @param ... Additional arguments for the format string.
 */
void ui_components_messagebox_draw(char *fmt, ...);

/**
 * @brief Draw the main text component.
 * 
 * @param align Horizontal alignment.
 * @param valign Vertical alignment.
 * @param fmt Format string for the text.
 * @param ... Additional arguments for the format string.
 */
void ui_components_main_text_draw(rdpq_align_t align, rdpq_valign_t valign, char *fmt, ...);

/**
 * @brief Draw the actions bar text component.
 * 
 * @param align Horizontal alignment.
 * @param valign Vertical alignment.
 * @param fmt Format string for the text.
 * @param ... Additional arguments for the format string.
 */
void ui_components_actions_bar_text_draw(rdpq_align_t align, rdpq_valign_t valign, char *fmt, ...);

/**
 * @brief Initialize the background component.
 * 
 * @param cache_location Location of the cache.
 */
void ui_components_background_init(char *cache_location);

/**
 * @brief Free the background component resources.
 */
void ui_components_background_free(void);

/**
 * @brief Replace the background image.
 * 
 * @param image New background image.
 */
void ui_components_background_replace_image(surface_t *image);

/**
 * @brief Draw the background component.
 */
void ui_components_background_draw(void);

/**
 * @brief Draw the file list component.
 * 
 * @param list List of entries.
 * @param entries Number of entries.
 * @param selected Index of the selected entry.
 */
void ui_components_file_list_draw(entry_t *list, int entries, int selected);

/**
 * @brief Context menu structure.
 */
typedef struct component_context_menu {
    int row_count; /**< Number of rows in the context menu */
    int row_selected; /**< Index of the selected row */
    bool hide_pending; /**< Flag to indicate if hiding is pending */
    struct component_context_menu *parent; /**< Pointer to the parent context menu */
    struct component_context_menu *submenu; /**< Pointer to the submenu */
    struct {
        const char *text; /**< Text of the menu item */
        void (*action)(menu_t *menu, void *arg); /**< Action function for the menu item */
        void *arg; /**< Argument for the action function */
        struct component_context_menu *submenu; /**< Pointer to the submenu */
    } list[]; /**< List of menu items */
} component_context_menu_t;

#define COMPONENT_CONTEXT_MENU_LIST_END { .text = NULL } /**< End marker for the context menu list */

/**
 * @brief Initialize the context menu component.
 * 
 * @param cm Pointer to the context menu structure.
 */
void ui_components_context_menu_init(component_context_menu_t *cm);

/**
 * @brief Show the context menu component.
 * 
 * @param cm Pointer to the context menu structure.
 */
void ui_components_context_menu_show(component_context_menu_t *cm);

/**
 * @brief Process the context menu component.
 * 
 * @param menu Pointer to the menu structure.
 * @param cm Pointer to the context menu structure.
 * @return True if the context menu was processed, false otherwise.
 */
bool ui_components_context_menu_process(menu_t *menu, component_context_menu_t *cm);

/**
 * @brief Draw the context menu component.
 * 
 * @param cm Pointer to the context menu structure.
 */
void ui_components_context_menu_draw(component_context_menu_t *cm);

/**
 * @brief Box Art Structure.
 */
typedef struct {
    bool loading; /**< Flag to indicate if the box art is loading */
    surface_t *image; /**< Pointer to the box art image */
} component_boxart_t;

/**
 * @brief Initialize the box art component.
 * 
 * @param storage_prefix Prefix for the storage location.
 * @param game_code Game code for the box art.
 * @param current_image_view Current image view type.
 * @return Pointer to the initialized box art component.
 */
component_boxart_t *ui_components_boxart_init(const char *storage_prefix, char *game_code, file_image_type_t current_image_view);

/**
 * @brief Free the box art component resources.
 * 
 * @param b Pointer to the box art component.
 */
void ui_components_boxart_free(component_boxart_t *b);

/**
 * @brief Draw the box art component.
 * 
 * @param b Pointer to the box art component.
 */
void ui_components_boxart_draw(component_boxart_t *b);

#endif /* UI_COMPONENTS_H__ */
