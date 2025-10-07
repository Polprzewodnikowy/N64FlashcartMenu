#include <stdarg.h>
#include "../bookkeeping.h"
#include "../fonts.h"
#include "../ui_components/constants.h"
#include "../sound.h"
#include "views.h"


typedef enum {
    BOOKKEEPING_TAB_CONTEXT_HISTORY,
    BOOKKEEPING_TAB_CONTEXT_FAVORITE,
    BOOKKEEPING_TAB_CONTEXT_NONE
} bookkeeping_tab_context_t;


static bookkeeping_tab_context_t tab_context = BOOKKEEPING_TAB_CONTEXT_NONE;
static int selected_item = -1;
static bookkeeping_item_t *item_list;
static uint16_t item_max = 0;


/**
 * Reset the current selection to the first non-empty bookkeeping item.
 *
 * Scans item_list up to item_max and sets selected_item to the index of the
 * first entry whose bookkeeping_type is not BOOKKEEPING_TYPE_EMPTY. If no such
 * entry exists, selected_item is set to -1.
 *
 * @param menu Menu context containing bookkeeping data.
 */
static void item_reset_selected(menu_t *menu) {
    selected_item = -1;

    for(uint16_t i=0; i<item_max; i++) {
        if(item_list[i].bookkeeping_type != BOOKKEEPING_TYPE_EMPTY) {
            selected_item = i;
            break;
        }
    }  
}

/**
 * Advance the current selection to the next bookkeeping item that is not empty.
 *
 * If a later non-empty item is found, updates `selected_item` to that index and
 * plays the cursor sound. If no such item exists after the current selection,
 * `selected_item` is left unchanged.
 */
static void item_move_next() {
    int last = selected_item;

    do
    {
        selected_item++;

        if(selected_item >= item_max) {
            selected_item = last;
            break;
        } else if(item_list[selected_item].bookkeeping_type != BOOKKEEPING_TYPE_EMPTY) {
            sound_play_effect(SFX_CURSOR);
            break;
        }
    } while (true);  
}

/**
 * Move the current selection to the previous bookkeeping entry that is not empty.
 *
 * Searches backward from the current selection for the nearest earlier item whose
 * `bookkeeping_type` is not `BOOKKEEPING_TYPE_EMPTY` and sets `selected_item` to
 * that index. If such an item is found, plays the cursor sound. If no earlier
 * non-empty item exists, the selection remains unchanged.
 */
static void item_move_previous() {
    int last = selected_item;
    do
    {
        selected_item--;

        if(selected_item < 0) {
            selected_item = last;
            break;
        } else if(item_list[selected_item].bookkeeping_type != BOOKKEEPING_TYPE_EMPTY) {
            sound_play_effect(SFX_CURSOR);
            break;
        }
    } while (true);
}

/**
 * Handle input for the bookkeeping view, updating selection, navigation, load targets, and feedback sounds.
 *
 * Processes the current menu actions:
 * - Down/Up: move selection to the next/previous non-empty item.
 * - Enter (when an item is selected): mark the selected index as the load target for the active tab (favorite or history),
 *   set the complementary load id to -1, set the next menu mode to load a disk or ROM depending on the item's type, and play the enter sound.
 * - Left/Right: switch tabs/modes according to the active tab (Favorite <-> Browser, History -> Browser/Favorite) and play the cursor sound.
 * - Options (when in Favorite and an item is selected): remove the selected favorite, reset selection, and play the setting sound.
 *
 * Actions that require a selected item are ignored when no item is selected.
 *
 * @param menu Pointer to the current menu state to read actions from and to update load/next_mode and bookkeeping state.
 */
static void process(menu_t *menu) {
    if(menu->actions.go_down) {
        item_move_next();   
    } else if(menu->actions.go_up) {
        item_move_previous();
    } else if(menu->actions.enter && selected_item != -1) {
                
        if(tab_context == BOOKKEEPING_TAB_CONTEXT_FAVORITE) {
            menu->load.load_favorite_id = selected_item;
            menu->load.load_history_id = -1;
        } else if(tab_context == BOOKKEEPING_TAB_CONTEXT_HISTORY) {
            menu->load.load_history_id = selected_item;
            menu->load.load_favorite_id = -1;
        }           

        if(item_list[selected_item].bookkeeping_type == BOOKKEEPING_TYPE_DISK) {
            menu->next_mode = MENU_MODE_LOAD_DISK;
            sound_play_effect(SFX_ENTER);
        } else if(item_list[selected_item].bookkeeping_type == BOOKKEEPING_TYPE_ROM) {
            menu->next_mode = MENU_MODE_LOAD_ROM;
            sound_play_effect(SFX_ENTER);
        }
    } else if (menu->actions.go_left) {
        if(tab_context == BOOKKEEPING_TAB_CONTEXT_FAVORITE) {
            menu->next_mode = MENU_MODE_HISTORY;
        } else if(tab_context == BOOKKEEPING_TAB_CONTEXT_HISTORY) {
            menu->next_mode = MENU_MODE_BROWSER;
        }
        sound_play_effect(SFX_CURSOR);       
    } else if (menu->actions.go_right) {
        if(tab_context == BOOKKEEPING_TAB_CONTEXT_FAVORITE) {
            menu->next_mode = MENU_MODE_BROWSER;
        } else if(tab_context == BOOKKEEPING_TAB_CONTEXT_HISTORY) {
            menu->next_mode = MENU_MODE_FAVORITE;
        }
        sound_play_effect(SFX_CURSOR);
    }else if(tab_context == BOOKKEEPING_TAB_CONTEXT_FAVORITE && menu->actions.options && selected_item != -1) {
        bookkeeping_favorite_remove(&menu->bookkeeping, selected_item);
        item_reset_selected(menu);
        sound_play_effect(SFX_SETTING);
    }
}

/**
 * Render the bookkeeping item list and current selection highlight into the given display.
 *
 * Draws a highlighted box behind the currently selected item (if any), builds a multi-line
 * listing where each entry shows the item index and last component of its primary path on
 * the first line and the last component of the secondary path (indented) on the second line,
 * and prints the assembled text block into the view area with the configured layout and wrapping.
 *
 * @param menu Pointer to the menu state providing the current item list, selection, and layout context.
 * @param display Surface to draw the list and highlight onto.
 */
static void draw_list(menu_t *menu, surface_t *display) {
    if(selected_item != -1) {
        float highlight_y = VISIBLE_AREA_Y0 + TEXT_MARGIN_VERTICAL + TAB_HEIGHT +  TEXT_OFFSET_VERTICAL + (selected_item * 19 * 2);

        ui_components_box_draw(
            VISIBLE_AREA_X0,
            highlight_y,
            VISIBLE_AREA_X0 + FILE_LIST_HIGHLIGHT_WIDTH + LIST_SCROLLBAR_WIDTH,
            highlight_y + 39,
            FILE_LIST_HIGHLIGHT_COLOR
        );
    }

    char buffer[1024];
    buffer[0] = 0;

    for(uint16_t i=0; i < item_max; i++) {   
        if(path_has_value(item_list[i].primary_path)) {
            sprintf(buffer, "%s%d  : %s\n",buffer ,(i+1), path_last_get(item_list[i].primary_path));
        } else {
            sprintf(buffer, "%s%d  : \n",buffer ,(i+1));
        }

        if(path_has_value(item_list[i].secondary_path)) {
            sprintf(buffer, "%s     %s\n", buffer, path_last_get(item_list[i].secondary_path));
        } else {
            sprintf(buffer, "%s\n", buffer);
        }
    }

    int nbytes = strlen(buffer);
    rdpq_text_printn(
        &(rdpq_textparms_t) {
            .width = VISIBLE_AREA_WIDTH - (TEXT_MARGIN_HORIZONTAL * 2),
            .height = LAYOUT_ACTIONS_SEPARATOR_Y - OVERSCAN_HEIGHT - (TEXT_MARGIN_VERTICAL * 2),
            .align = ALIGN_LEFT,
            .valign = VALIGN_TOP,
            .wrap = WRAP_ELLIPSES,
            .line_spacing = TEXT_OFFSET_VERTICAL,
        },
        FNT_DEFAULT,
        VISIBLE_AREA_X0 + TEXT_MARGIN_HORIZONTAL,
        VISIBLE_AREA_Y0 + TEXT_MARGIN_VERTICAL + TAB_HEIGHT +  TEXT_OFFSET_VERTICAL,
        buffer,
        nbytes
    );           
}

/**
 * Render the bookkeeping view (Favorites or History) onto the provided display.
 *
 * Draws the background, current tab indicators, tabbed layout, and the list of bookkeeping
 * items. If an item is selected, displays a left-aligned "A: Load Game" action and, when
 * viewing Favorites, a right-aligned "R: Remove item" action. Always displays a centered
 * "< Change Tab >" action bar.
 *
 * @param menu Menu state used to determine items and current selection.
 * @param display Surface to render the view onto.
 */
static void draw(menu_t *menu, surface_t *display) {
    rdpq_attach(display, NULL);

    ui_components_background_draw();

    if(tab_context == BOOKKEEPING_TAB_CONTEXT_FAVORITE) {
        ui_components_tabs_common_draw(2);
    } else if(tab_context == BOOKKEEPING_TAB_CONTEXT_HISTORY) {
        ui_components_tabs_common_draw(1);
    }

    ui_components_layout_draw_tabbed();

    draw_list(menu, display);

    if(selected_item != -1) {
        ui_components_actions_bar_text_draw(
            STL_DEFAULT,
            ALIGN_LEFT, VALIGN_TOP,
            "A: Load Game\n"
            "\n"
        );
        
        if(tab_context == BOOKKEEPING_TAB_CONTEXT_FAVORITE && selected_item != -1) {
            ui_components_actions_bar_text_draw(
                STL_DEFAULT,
                ALIGN_RIGHT, VALIGN_TOP,
                "R: Remove item\n"
                "\n"
            );
        }
    }

    ui_components_actions_bar_text_draw(
        STL_DEFAULT,
        ALIGN_CENTER, VALIGN_TOP,
        "< Change Tab >\n"
        "\n"
    );    

    rdpq_detach_show();   
}

/**
 * Initialize the Favorites bookkeeping view and reset selection.
 *
 * Configure the view to use the Favorites tab: set the tab context to FAVORITE, point the internal
 * item list at menu->bookkeeping.favorite_items, set the item count to FAVORITES_COUNT, and select
 * the first non-empty favorite entry.
 *
 * @param menu Menu context containing bookkeeping data used to populate the view.
 */
void view_favorite_init (menu_t *menu) {
    tab_context = BOOKKEEPING_TAB_CONTEXT_FAVORITE;
    item_list = menu->bookkeeping.favorite_items;
    item_max = FAVORITES_COUNT;

    item_reset_selected(menu);
}

/**
 * Display the Favorites tab for one frame by handling input and rendering the view.
 *
 * @param menu Current menu state containing bookkeeping data and navigation state.
 * @param display Rendering surface to draw the Favorites view onto.
 */
void view_favorite_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display); 
}

/**
 * Initialize the view for the History tab.
 *
 * Configure the bookkeeping view to display history items from the provided
 * menu: select the History tab context, point the internal item list to
 * menu->bookkeeping.history_items, set the item count to HISTORY_COUNT, and
 * reset the current selection.
 *
 * @param menu Pointer to the menu state containing bookkeeping history items.
 */
void view_history_init (menu_t *menu) {
    tab_context = BOOKKEEPING_TAB_CONTEXT_HISTORY;
    item_list = menu->bookkeeping.history_items;
    item_max = HISTORY_COUNT;

    item_reset_selected(menu);
}

/**
 * Handle input and render the History view for a single frame.
 *
 * Processes user input and updates view state related to history items, then
 * draws the history UI into the provided display surface.
 *
 * @param menu Current menu/context state used by the view.
 * @param display Rendering surface to draw the view into.
 */
void view_history_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display); 
}