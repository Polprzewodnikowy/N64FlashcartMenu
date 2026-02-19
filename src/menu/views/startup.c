#include <libdragon.h>

#include "views.h"
#include "../virtual_cpak.h"
#include "utils/cpakfs_utils.h"


static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach_clear(d, NULL);
    rdpq_detach_show();
}


void view_startup_init (menu_t *menu) {
    // Check for dirty Virtual Controller Pak state (unclean exit from previous game)
    // Only if virtual cpak feature is enabled
    if (menu->settings.virtual_cpak_enabled) {
        vcpak_state_t dirty_state;
        if (vcpak_state_load(menu->storage_prefix, &dirty_state) == VCPAK_OK && dirty_state.is_dirty) {
            debugf("Startup: Detected dirty vcpak state from previous session\n");

            // Check if physical pak is present
            if (has_cpak(0)) {
                // Physical pak present - auto-backup and clear state
                debugf("Startup: Physical pak detected, auto-backing up to %s\n", dirty_state.pak_path);
                vcpak_err_t err = vcpak_backup_from_physical(dirty_state.pak_path, 0);
                if (err == VCPAK_OK) {
                    debugf("Startup: Auto-backup successful\n");
                } else {
                    debugf("Startup: Auto-backup failed with error %d\n", err);
                }
                vcpak_state_clear(menu->storage_prefix);
            } else {
                // No physical pak - show recovery dialog
                debugf("Startup: No physical pak, showing recovery dialog\n");
                menu->next_mode = MENU_MODE_VCPAK_RECOVERY;
                return;
            }
        }
    }

#ifdef FEATURE_AUTOLOAD_ROM_ENABLED
    // FIXME: rather than use a controller button, would it be better to use the cart button?
    JOYPAD_PORT_FOREACH (port) {
        joypad_poll();
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
        menu->load.rom_path = path_clone_push(menu->browser.directory, menu->settings.rom_autoload_filename);
        menu->load_pending.rom_file = true;
        menu->next_mode = MENU_MODE_LOAD_ROM;

        return;
    }
#endif
    
    if (menu->settings.first_run) {
        menu->settings.first_run = false;
        settings_save(&menu->settings);
        menu->next_mode = MENU_MODE_CREDITS;
    }
    else {
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

void view_startup_display (menu_t *menu, surface_t *display) {
    draw(menu, display);
}
