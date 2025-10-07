#include <stdbool.h>
#include <stdio.h>
#include <libdragon.h>
#include "views.h"
#include "../sound.h"
#include "../fonts.h"
#include <fatfs/ff.h>
#include "utils/cpakfs_utils.h"


static char cpak_path[255];
static int16_t controller_selected;
static char failure_message[255];
static bool start_complete_restore;

#define CONTROLLERPAK_BANK_SIZE 32768

/**
 * Restore a Controller Pak from the configured dump file to the specified controller.
 *
 * Attempts to write the dump file at `cpak_path` bank-by-bank to the Controller Pak
 * attached to `controller`. The function verifies the device is present, checks the
 * dump file size against the Pak's capacity, and writes each bank in sequence.
 * On completion or failure, `failure_message` is populated with a human-readable status.
 *
 * @param controller Index of the target controller (0 through 3).
 * @returns `true` if the dump was successfully restored to the Controller Pak, `false` otherwise.
 */
static bool restore_controller_pak(int controller) {
    sprintf(failure_message, " ");

    if (!has_cpak(controller)) {
        sprintf(failure_message, "No Controller Pak detected on controller %d!", controller + 1);
        return false;
    }

    cpakfs_unmount(controller);

    uint8_t *data = malloc(CONTROLLERPAK_BANK_SIZE);
    if (!data) {
        sprintf(failure_message, "Memory allocation failed!");
        return false;
    }

    FILE *fp = fopen(cpak_path, "rb");
    if (!fp) {
        sprintf(failure_message, "Failed to open file for reading!");
        free(data);
        return false;
    }

    if (fseek(fp, 0, SEEK_END) != 0) {
        sprintf(failure_message, "Seek failed!");
        fclose(fp);
        free(data);
        return false;
    }
    long filesize = ftell(fp);
    if (filesize < 0) {
        sprintf(failure_message, "ftell failed!");
        fclose(fp);
        free(data);
        return false;
    }
    rewind(fp);

    int total_banks = (int)((filesize + CONTROLLERPAK_BANK_SIZE - 1) / CONTROLLERPAK_BANK_SIZE);

    int banks_on_device = cpak_probe_banks(controller);
    if (banks_on_device < 1) {
        sprintf(failure_message, "Cannot probe Controller Pak banks (err=%d)!", banks_on_device);
        fclose(fp);
        free(data);
        return false;
    }
    if (total_banks > banks_on_device) {
        sprintf(failure_message, "Dump file too large (%d banks) for controller (%d banks)!",
                total_banks, banks_on_device);
        fclose(fp);
        free(data);
        return false;
    }

    debugf("Restoring Controller Pak: %ld bytes (%d banks)\n", filesize, total_banks);

    for (int bank = 0; bank < total_banks; bank++) {
        size_t bytesRead = fread(data, 1, CONTROLLERPAK_BANK_SIZE, fp);
        if (bytesRead == 0 && ferror(fp)) {
            sprintf(failure_message, "Read error from dump file!");
            fclose(fp);
            free(data);
            return false;
        }
        if (bytesRead == 0 && feof(fp)) break; // empty trailing chunk (shouldn't happen)

        int written = cpak_write((joypad_port_t)controller, (uint8_t)bank, 0, data, bytesRead);
        if (written < 0) {
            sprintf(failure_message, "Failed to write bank %d to Controller Pak! errno=%d", bank, written);
            fclose(fp);
            free(data);
            return false;
        }
        if ((size_t)written != bytesRead) {
            sprintf(failure_message, "Short write on bank %d: wrote %d / %zu bytes", bank, written, bytesRead);
            fclose(fp);
            free(data);
            return false;
        }
    }

    fclose(fp);
    free(data);

    sprintf(failure_message, "Dump restored on controller %d!", controller + 1);
    return true;
}

/**
 * Handle navigation and confirmation input for the Controller Pak dump view.
 *
 * Interprets the current menu actions to:
 * - move the selected controller left or right with wrap-around and play the cursor sound,
 * - cancel and return to the browser view while playing the exit sound,
 * - or initiate the complete restore process while playing the enter sound.
 *
 * @param menu Menu state containing the current input actions and where the next mode is set.
 */
static void process (menu_t *menu) {
    if (menu->actions.go_left) {
        sound_play_effect(SFX_CURSOR);
        controller_selected = ((controller_selected - 1) + 4) % 4;
    } else if (menu->actions.go_right) {
        sound_play_effect(SFX_CURSOR);
        controller_selected = ((controller_selected + 1) + 4) % 4;
    } else if (menu->actions.back) {
        sound_play_effect(SFX_EXIT);
        menu->next_mode = MENU_MODE_BROWSER;
    } else if (menu->actions.enter) {
        sound_play_effect(SFX_ENTER);
        start_complete_restore = true;
    }
}

/**
 * Render the Controller Pak dump confirmation view and handle restore initiation.
 *
 * Draws background, layout, path and status messages, and a confirmation messagebox
 * for selecting a target controller. If a complete restore has been requested,
 * a loader is shown and the dump restore is performed; on successful restore the
 * menu is advanced to the browser view. The restore flag is cleared before
 * returning.
 *
 * @param menu Menu state used for input/result handling and potentially advanced on success.
 * @param d    Rendering surface to draw into.
 */
static void draw (menu_t *menu, surface_t *d) {

    rdpq_attach(d, NULL);

    ui_components_background_draw();

    ui_components_layout_draw();

    ui_components_main_text_draw(STL_DEFAULT,
        ALIGN_CENTER, VALIGN_TOP,
        "Controller Pak dump:\n"
    );
    ui_components_main_text_draw(STL_GREEN,
        ALIGN_CENTER, VALIGN_TOP,
        "\n"
        "%s\n"
        "\n"
        "\n",
        cpak_path
    );
    ui_components_main_text_draw(STL_ORANGE,
        ALIGN_CENTER, VALIGN_TOP,
        "\n"
        "\n"
        "\n"
        "%s\n",
        failure_message
    );

    
    ui_components_messagebox_draw(
        "Do you want to restore this dump to the Controller Pak?\n\n"
        "Controller selected: %d\n\n"
        "A: Yes  B: No \n"
        "<- / ->: Change controller",
        controller_selected + 1
    );

    if (start_complete_restore) {
        ui_components_loader_draw(0, "Writing Controller Pak...");
        rdpq_detach_show();
        if (restore_controller_pak(controller_selected) && !failure_message[0]) {
            menu->next_mode = MENU_MODE_BROWSER;
        } 
        start_complete_restore = false;
        return;
        
    }

    rdpq_detach_show();
}

/**
 * Initialize the Controller Pak dump view state and prepare the full path to the selected dump file.
 *
 * Clones the browser's current directory with the selected entry to populate `cpak_path`, clears
 * `failure_message`, and resets the `start_complete_restore` flag.
 *
 * @param menu Current menu state containing the browser directory and selected entry.
 */
void view_controller_pak_dump_info_init (menu_t *menu) {

    path_t *path = path_clone_push(menu->browser.directory, menu->browser.entry->name);

    sprintf(cpak_path, "%s", path_get(path));
    start_complete_restore = false;
    sprintf(failure_message, " ");

    path_free(path);
}

/**
 * Update the Controller Pak dump info view for one frame by handling input and rendering.
 *
 * @param menu The current menu context.
 * @param display The target surface to render onto.
 */
void view_controller_pak_dump_info_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}