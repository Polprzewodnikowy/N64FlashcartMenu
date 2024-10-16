#include "utils/fs.h"
#include "views.h"


static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach_clear(d, NULL);
    rdpq_detach_show();
}


void view_startup_init (menu_t *menu) {
    // FIXME: rather than use a controller button, would it be better to use the cart button?
    JOYPAD_PORT_FOREACH (port) {
        for (int i = 0; i < 50; i++) { // something like this is needed to poll enough.
            joypad_poll();
        }
        joypad_buttons_t b_held = joypad_get_buttons_held(port);

        if (menu->settings.rom_autoload_enabled && b_held.start) {
            menu->settings.rom_autoload_enabled = false;
            menu->settings.rom_autoload_path = "";
            menu->settings.rom_autoload_filename = "";
            settings_save(&menu->settings);
        }
    }
    if (menu->settings.rom_autoload_enabled) {
        menu->browser.directory = path_init(menu->storage_prefix, menu->settings.rom_autoload_path);
        menu->browser.entry->name = menu->settings.rom_autoload_filename;

        menu->rom_load_pending = true;
        menu->next_mode = MENU_MODE_LOAD_ROM;

    }
    
    menu->next_mode = MENU_MODE_BROWSER;
}

void view_startup_display (menu_t *menu, surface_t *display) {
    draw(menu, display);
}
