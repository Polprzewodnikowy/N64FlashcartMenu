#include <libdragon.h>

#include "flashcart/flashcart.h"

#include "fragments/fragments.h"
#include "views.h"
#include "../rom_database.h"


static bool load_pending;


static void load (menu_t *menu) {
    menu->next_mode = MENU_MODE_BOOT;

    path_t *path = path_clone(menu->browser.directory);

    path_push(path, menu->browser.list[menu->browser.selected].name);

    rom_header_t temp_header = file_read_rom_header(path_get(path));
    
    uint8_t save_type = rom_db_match_save_type(temp_header);

    if (flashcart_load_rom(path_get(path), temp_header.endian == ROM_MID_BIG_ENDIAN) != FLASHCART_OK) {
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


static void process (menu_t *menu) {
    if (menu->actions.enter) {
        load_pending = true;
    } else if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

static void draw (menu_t *menu, surface_t *d) {
    // layout_t *layout = get_layout();

    const color_t bg_color = RGBA32(0x00, 0x00, 0x00, 0xFF);

    rdpq_attach(d, NULL);
    rdpq_clear(bg_color);

    // Layout
    fragment_borders(d);

    // TODO: Display ROM information here

    rdpq_detach_show();
}


void view_load_init (menu_t *menu) {
    load_pending = false;
}

void view_load_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
    if (load_pending) {
        load_pending = false;
        load(menu);
    }
}
