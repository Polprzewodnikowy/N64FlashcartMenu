#include <stdarg.h>
#include "views.h"
#include "../rom_history.h"
#include "../components/constants.h"
#include "../fonts.h"

static int selected_favorite = -1;

static void favorite_reset_selected(menu_t* menu) {
    selected_favorite = -1;

    for(int i=0;i<FAVORITES_COUNT;i++) {
        if(path_has_value(menu->history.favorites_rom[i]) || path_has_value(menu->history.favorites_disk[i])) {
            selected_favorite = i;
            break;
        }
    }  
}

static void process(menu_t* menu) {
    if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
    } else if(menu->actions.go_down) {
        int last_favorite = selected_favorite;

        do
        {
            selected_favorite++;

            if(selected_favorite >= FAVORITES_COUNT) {
                selected_favorite = last_favorite;
                break;
            } else if(path_has_value(menu->history.favorites_rom[selected_favorite]) || path_has_value(menu->history.favorites_disk[selected_favorite])) {
                break;
            }
        } while (true);    
    } else if(menu->actions.go_up) {
        int last_favorite = selected_favorite;

        do
        {
            selected_favorite--;

            if(selected_favorite < 0) {
                selected_favorite = last_favorite;
                break;
            } else if(path_has_value(menu->history.favorites_rom[selected_favorite]) || path_has_value(menu->history.favorites_disk[selected_favorite])) {
                break;
            }
        } while (true);    
    } else if(menu->actions.enter && selected_favorite != -1) {
        menu->favourite.load_favorite = selected_favorite;
        
        if(path_has_value(menu->history.favorites_disk[selected_favorite])) {
            menu->next_mode = MENU_MODE_LOAD_DISK;
        } else if(path_has_value(menu->history.favorites_rom[selected_favorite])) {
            menu->next_mode = MENU_MODE_LOAD_ROM;
        }
    } else if(menu->actions.options && selected_favorite != -1) {
        history_remove_favorite(&menu->history, selected_favorite);
        favorite_reset_selected(menu);
    }
}

static void draw_favorites(menu_t *menu, surface_t *display) {
    
    float y = 24;
    float x = 10;

    if(selected_favorite != -1) {    
        float highlight_y = 32 + y + (selected_favorite * 16 * 2);

        component_box_draw(
            VISIBLE_AREA_X0,
            highlight_y,
            VISIBLE_AREA_X0 + FILE_LIST_HIGHLIGHT_WIDTH,
            highlight_y + 32,
            FILE_LIST_HIGHLIGHT_COLOR
        );
    }

    for(int i=0;i < FAVORITES_COUNT; i++)
    {
        component_main_text_draw_location(x, y, "%d  :", (i+1));
        if(path_has_value(menu->history.favorites_rom[i])) {
            component_main_text_draw_location(x + 64, y, "%s", path_get(menu->history.favorites_rom[i]));
        }
        y += 16;
        
        if(path_has_value(menu->history.favorites_disk[i])) {
            component_main_text_draw_location(x + 64, y,"%s", path_get(menu->history.favorites_disk[i]));
        }
        y += 16;
    }
}

static void draw(menu_t *menu, surface_t *display) {
    rdpq_attach(display, NULL);

    component_background_draw();

    component_layout_draw();

    component_main_text_draw(
            ALIGN_CENTER, VALIGN_TOP,
            "Favorites");

    draw_favorites(menu, display);

    if(selected_favorite != -1) {
        component_actions_bar_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "A: Load Favorite\n"
            "B: Exit"
        );

        component_actions_bar_text_draw(
            ALIGN_RIGHT, VALIGN_TOP,
            "R: Remove Favorite"
        );
    } else {
        component_actions_bar_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "\n"
            "B: Exit"
        );
    }


    rdpq_detach_show();   
}




void view_favorite_init (menu_t *menu) {
    favorite_reset_selected(menu);
}

void view_favorite_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display); 
}