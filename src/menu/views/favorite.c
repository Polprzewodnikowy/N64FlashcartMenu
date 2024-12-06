#include <stdarg.h>
#include "views.h"
#include "../rom_history.h"
#include "../fonts.h"
#include "../ui_components/constants.h"
#include "../sound.h"


typedef enum {
    BOOKKEEPING_SCREEN_MODE_HISTORY,
    BOOKKEEPING_SCREEN_MODE_FAVORITE
} bookkeeping_screen_modes_t;

static bookkeeping_screen_modes_t screen_mode = -1;
static int selected_item = -1;
static bookkeeping_item_t* item_list;
static int item_max;


static void reset_selected(menu_t* menu) {
    selected_item = -1;

    for(int i=0;i<item_max;i++) {
        if(item_list[selected_item].bookkeeping_type != HISTORY_TYPE_EMPTY) {
            selected_item = i;
            break;
        }
    }  
}



static void move_next() {
    int last = selected_item;

    do
    {
        selected_item++;

        if(selected_item >= item_max) {
            selected_item = last;
            break;
        } else if(item_list[selected_item].bookkeeping_type != HISTORY_TYPE_EMPTY) {
            sound_play_effect(SFX_CURSOR);
            break;
        }
    } while (true);  
}

static void move_back() {
    int last = selected_item;
    do
    {
        selected_item--;

        if(selected_item < 0) {
            selected_item = last;
            break;
        } else if(item_list[selected_item].bookkeeping_type != HISTORY_TYPE_EMPTY) {
            sound_play_effect(SFX_CURSOR);
            break;
        }
    } while (true);
}

static void process(menu_t* menu) {
    if(menu->actions.go_down) {
        move_next();   
    } else if(menu->actions.go_up) {
        move_back();
    } else if(menu->actions.enter && selected_item != -1) {
                
        if(screen_mode == BOOKKEEPING_SCREEN_MODE_FAVORITE) {
            menu->load.load_favorite = selected_item;
        } else if(screen_mode == BOOKKEEPING_SCREEN_MODE_HISTORY) {
            menu->load.load_history = selected_item;
        }           

        if(item_list[selected_item].bookkeeping_type == HISTORY_TYPE_DISK) {
            menu->next_mode = MENU_MODE_LOAD_DISK;
            sound_play_effect(SFX_ENTER);
        } else if(item_list[selected_item].bookkeeping_type == HISTORY_TYPE_ROM) {
            menu->next_mode = MENU_MODE_LOAD_ROM;
            sound_play_effect(SFX_ENTER);
        }
    } else if (menu->actions.previous_tab) {
        if(screen_mode == BOOKKEEPING_SCREEN_MODE_FAVORITE) {
            menu->next_mode = MENU_MODE_HISTORY;
        } else if(screen_mode == BOOKKEEPING_SCREEN_MODE_HISTORY) {
            menu->next_mode = MENU_MODE_BROWSER;
        }        
    } else if (menu->actions.next_tab) {
        if(screen_mode == BOOKKEEPING_SCREEN_MODE_FAVORITE) {
            menu->next_mode = MENU_MODE_BROWSER;
        } else if(screen_mode == BOOKKEEPING_SCREEN_MODE_HISTORY) {
            menu->next_mode = MENU_MODE_FAVORITE;
        }
    }else if(screen_mode == BOOKKEEPING_SCREEN_MODE_FAVORITE && menu->actions.options && selected_item != -1) {
        //history_favorite_remove(&menu->history, selected_item);
        reset_selected(menu);
        sound_play_effect(SFX_SETTING);
    }
}

static void draw_favorites(menu_t *menu, surface_t *display) {
    
    float y = VISIBLE_AREA_Y0;
    float x = 10 + VISIBLE_AREA_X0;

    if(selected_item != -1) {    
        float highlight_y = y + (selected_item * 18 * 2);

        ui_components_box_draw(
            VISIBLE_AREA_X0,
            highlight_y,
            VISIBLE_AREA_X0 + FILE_LIST_HIGHLIGHT_WIDTH,
            highlight_y + 36,
            FILE_LIST_HIGHLIGHT_COLOR
        );
    }

    for(int i=0;i < FAVORITES_COUNT; i++) {        
        if(path_has_value(item_list[i].primary_path)) {
            ui_components_main_text_draw_location(x, y, "%d  : %s",(i+1), path_last_get(item_list[i].primary_path));
        } else {
            ui_components_main_text_draw_location(x, y, "%d  :", (i+1));
        }
        y += 16;
        
        if(path_has_value(item_list[i].secondary_path)) {
            ui_components_main_text_draw_location(x, y,"     %s", path_last_get(item_list[i].secondary_path));
        }
        y += 20;
    }

    if(screen_mode == BOOKKEEPING_SCREEN_MODE_FAVORITE) {
        ui_compontents_tabs_common_draw(2);
    } else if(screen_mode == BOOKKEEPING_SCREEN_MODE_HISTORY) {
        ui_compontents_tabs_common_draw(1);
    }    
}

static void draw(menu_t *menu, surface_t *display) {
    rdpq_attach(display, NULL);

    ui_components_background_draw();

    ui_components_layout_draw();

    draw_favorites(menu, display);

    if(selected_item != -1) {
        ui_components_actions_bar_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "A: Load Game"
        );
        
        if(screen_mode == BOOKKEEPING_SCREEN_MODE_FAVORITE) {
            ui_components_actions_bar_text_draw(
                ALIGN_RIGHT, VALIGN_TOP,
                "R: Remove Favorite"
            );
        }
    }

    rdpq_detach_show();   
}




void view_favorite_init (menu_t *menu) {
    screen_mode = BOOKKEEPING_SCREEN_MODE_FAVORITE;
    item_list = menu->history.favorite_items;
    item_max = FAVORITES_COUNT;

    reset_selected(menu);
}

void view_favorite_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display); 
}



void view_history_init (menu_t *menu) {
    screen_mode = BOOKKEEPING_SCREEN_MODE_HISTORY;
    item_list = menu->history.history_items;
    item_max = HISTORY_COUNT;

    reset_selected(menu);
}

void view_history_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display); 
}