/**
 * @file vcpak_recovery.c
 * @brief Virtual Controller Pak recovery view.
 * @ingroup view
 *
 * This view is shown when the menu detects a dirty state from a previous
 * session, indicating an unclean exit (e.g., power loss). It prompts the
 * user to backup the current Controller Pak contents.
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <libdragon.h>

#include "views.h"
#include "../sound.h"
#include "../fonts.h"
#include "../virtual_cpak.h"
#include "utils/cpakfs_utils.h"


static vcpak_state_t dirty_state;
static bool state_loaded;
static bool backup_pending;      // User requested backup, needs to be performed
static bool show_backing_up;     // Currently displaying "backing up" message
static bool show_backup_result;
static bool backup_success;
static bool no_physical_pak;

static void process(menu_t *menu) {
    // If showing backup result, wait for acknowledgment
    if (show_backup_result) {
        if (menu->actions.enter || menu->actions.back) {
            sound_play_effect(SFX_ENTER);
            // Clear dirty state and proceed to startup
            vcpak_state_clear(menu->storage_prefix);
            menu->next_mode = MENU_MODE_STARTUP;
        }
        return;
    }

    // Perform backup if pending (after "backing up" message was shown)
    if (backup_pending) {
        vcpak_err_t err = vcpak_backup_from_physical(dirty_state.pak_path, 0);
        backup_success = (err == VCPAK_OK);
        backup_pending = false;
        show_backing_up = false;
        show_backup_result = true;
        return;
    }

    // If showing "backing up" message, set pending flag for next frame
    if (show_backing_up) {
        backup_pending = true;
        return;
    }

    // Normal dialog handling
    if (menu->actions.enter) {
        // "Update Pak Now" - backup current pak contents
        sound_play_effect(SFX_ENTER);

        if (no_physical_pak) {
            // Can't backup without physical pak
            backup_success = false;
            show_backup_result = true;
        } else {
            show_backing_up = true;
        }
    } else if (menu->actions.back) {
        // "Discard Changes" - just clear state and continue
        sound_play_effect(SFX_EXIT);
        vcpak_state_clear(menu->storage_prefix);
        menu->next_mode = MENU_MODE_STARTUP;
    }
}

static void draw(menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    ui_components_background_draw();
    ui_components_layout_draw();

    // Title
    ui_components_main_text_draw(
        STL_ORANGE,
        ALIGN_CENTER, VALIGN_TOP,
        "CONTROLLER PAK RECOVERY\n"
    );

    if (!state_loaded) {
        ui_components_main_text_draw(
            STL_DEFAULT,
            ALIGN_CENTER, VALIGN_TOP,
            "\n\n\n"
            "Loading state information...\n"
        );
    } else {
        // Show dirty state info
        ui_components_main_text_draw(
            STL_DEFAULT,
            ALIGN_LEFT, VALIGN_TOP,
            "\n\n"
            "The menu did not exit cleanly after playing:\n"
        );

        time_t ts = (time_t)dirty_state.timestamp;
        struct tm tm = *localtime(&ts);

        ui_components_main_text_draw(
            STL_GREEN,
            ALIGN_LEFT, VALIGN_TOP,
            "\n\n\n\n"
            "  Game: %.20s\n"
            "  Code: %.4s\n"
            "  Time: %04d-%02d-%02d %02d:%02d:%02d\n",
            dirty_state.game_title,
            dirty_state.game_code,
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec
        );

        ui_components_main_text_draw(
            STL_DEFAULT,
            ALIGN_LEFT, VALIGN_TOP,
            "\n\n\n\n\n\n\n\n"
            "Pak file: %.40s\n",
            dirty_state.pak_path
        );

        if (no_physical_pak) {
            ui_components_main_text_draw(
                STL_ORANGE,
                ALIGN_CENTER, VALIGN_TOP,
                "\n\n\n\n\n\n\n\n\n\n\n"
                "Warning: No Controller Pak detected in Port 1.\n"
                "Insert the pak that was used, then press A.\n"
            );
        } else {
            ui_components_main_text_draw(
                STL_DEFAULT,
                ALIGN_CENTER, VALIGN_TOP,
                "\n\n\n\n\n\n\n\n\n\n\n"
                "Would you like to save the current\n"
                "Controller Pak contents to this file?\n"
            );
        }
    }

    // Actions bar
    ui_components_actions_bar_text_draw(
        STL_DEFAULT,
        ALIGN_LEFT, VALIGN_TOP,
        "A: Update Pak Now\n"
        "B: Discard Changes\n"
    );

    // Dialogs
    if (show_backing_up) {
        ui_components_messagebox_draw(
            "Backing up Controller Pak...\n\n"
            "Please wait..."
        );
    } else if (show_backup_result) {
        if (backup_success) {
            ui_components_messagebox_draw(
                "Controller Pak backed up successfully!\n\n"
                "Press A to continue."
            );
        } else {
            if (no_physical_pak) {
                ui_components_messagebox_draw(
                    "Backup failed!\n\n"
                    "No Controller Pak detected.\n"
                    "Insert pak and try again, or press B to discard.\n\n"
                    "Press A to continue without backup."
                );
            } else {
                ui_components_messagebox_draw(
                    "Backup failed!\n\n"
                    "An error occurred while backing up.\n\n"
                    "Press A to continue without backup."
                );
            }
        }
    }

    rdpq_detach_show();
}


void view_vcpak_recovery_init(menu_t *menu) {
    state_loaded = false;
    backup_pending = false;
    show_backing_up = false;
    show_backup_result = false;
    backup_success = false;
    no_physical_pak = false;

    // Load the dirty state
    vcpak_err_t err = vcpak_state_load(menu->storage_prefix, &dirty_state);
    if (err == VCPAK_OK) {
        state_loaded = true;
    } else {
        // No dirty state found, shouldn't be here
        // Just proceed to startup
        menu->next_mode = MENU_MODE_STARTUP;
        return;
    }

    // Check for physical pak
    if (!has_cpak(0)) {
        no_physical_pak = true;
    }
}

void view_vcpak_recovery_display(menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}
