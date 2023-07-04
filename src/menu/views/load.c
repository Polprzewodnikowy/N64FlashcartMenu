#include <libdragon.h>
#include "../menu.h"
#include "../menu_res_setup.h"
#include "../rom_database.h"
#include "../../flashcart/flashcart.h"


static void draw (menu_t *menu, surface_t *d) {
    int x = overscan_horizontal_pixels;
    int y = overscan_vertical_pixels;

    graphics_fill_screen(d, graphics_make_color(0, 0, 0, 255));

    graphics_draw_text(d, x, y, "booting...");

    display_show(d);
}

static void load (menu_t *menu) {
    menu->next_mode = MENU_MODE_BOOT;

    path_t *path = path_clone(menu->browser.directory);

    path_push(path, menu->browser.list[menu->browser.selected].name);

    rom_header_t temp_header = file_read_rom_header(path_get(path));
    
    uint8_t save_type = rom_db_match_save_type(temp_header);

    if (flashcart_load_rom(path_get(path)) != FLASHCART_OK) {
        menu->next_mode = MENU_MODE_ERROR;
        path_free(path);
        return;
    }

    path_ext_replace(path, "sav");
    if (flashcart_load_save(path_get(path), (flashcart_save_type_t) (save_type), true) != FLASHCART_OK) {
        menu->next_mode = MENU_MODE_ERROR;
        path_free(path);
        return;
    }

    path_free(path);
}


void view_load_init (menu_t *menu) {
    // Nothing to initialize (yet)
}

void view_load_display (menu_t *menu, surface_t *display) {
    draw(menu, display);
    load(menu);
}
