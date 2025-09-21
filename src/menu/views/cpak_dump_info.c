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

/**
 * Restore a 128-block Controller Pak dump from disk to a controller's mempak.
 *
 * Attempts to open and read MEMPAK_BLOCK_SIZE * 128 bytes from the global
 * cpak_path file and write each block into the specified controller's Pak
 * sectors. On failure the global failure_message is set with a short status
 * and the function returns false.
 *
 * Side effects:
 * - Reads the file at cpak_path.
 * - Writes up to 128 sectors to the Controller Pak via write_mempak_sector.
 * - Updates the global failure_message on error.
 * - Emits per-sector UI/progress updates while restoring.
 *
 * @param controller Index of the controller to restore to (0-3).
 * @return true if all 128 sectors were written successfully; false on any error
 *         (missing Pak, file open/read failure, or sector write failure).
 */
static bool restore_controller_pak(int controller) {
    sprintf(failure_message, " ");

    if (!has_cpak(controller)) {
        sprintf(failure_message, "No Controller Pak detected on controller %d!", controller + 1);
        return false;
    }

    uint8_t* data = malloc(MEMPAK_BLOCK_SIZE * 128 * sizeof(uint8_t));
    FILE *fp = fopen(cpak_path, "r");
    if (!fp) {
        sprintf(failure_message, "Failed to open file for reading!");
        free(data);
        return false;
    }
    if (fread(data, 1, MEMPAK_BLOCK_SIZE * 128, fp) != MEMPAK_BLOCK_SIZE * 128) {
        sprintf(failure_message, "Failed to read data from file!");
        fclose(fp);
        free(data);
        return false;
    }
    fclose(fp);

    for (int i = 0; i < 128; i++) { 
        if (write_mempak_sector(controller_selected, i, data + (i * MEMPAK_BLOCK_SIZE)) != 0) {
            sprintf(failure_message, "Failed to write to Controller Pak sector!");
            free(data);
            return false;
        }

        surface_t *d = display_try_get();
        rdpq_attach(d, NULL);

        ui_components_layout_draw();

        
        ui_components_messagebox_draw(
            "Do you want to restore this dump to the Controller Pak?\n\n"
            "Controller selected: %d\n\n"
            "A: Yes  B: No \n"
            "<- / ->: Change controller",
            controller_selected + 1
        );
        ui_components_loader_draw((float) i / 128.0f, "Restoring Controller Pak...");
        rdpq_detach_show();
    }

    free(data);
    return true;
}

/**
 * Process input actions for the Controller Pak dump view.
 *
 * Reads the action flags in `menu->actions` and performs the corresponding UI
 * responses: move the selected controller left/right with wrap-around,
 * trigger menu exit, or mark the restore operation to start. Side effects:
 * plays the appropriate sound effect, updates the global `controller_selected`
 * index on left/right actions, sets `menu->next_mode` to `MENU_MODE_BROWSER`
 * on back, and sets the global `start_complete_restore` flag on enter.
 *
 * @param menu Pointer to the current menu state (reads `menu->actions` and may
 *             update `menu->next_mode`).
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
 * Draws the background, layout, informational text (including the dump path and any
 * failure_message), and a message box that lets the user confirm restoring the dump
 * to a selected controller Pak. If the global flag `start_complete_restore` is set,
 * this function will finalize rendering, call `restore_controller_pak(controller_selected)`,
 * and, on success, advance `menu->next_mode` to `MENU_MODE_BROWSER`. The flag is
 * cleared before returning.
 *
 * @param menu Pointer to the current menu state; may be updated (menu->next_mode).
 * @param d Display surface to draw onto.
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
        rdpq_detach_show();
        if (restore_controller_pak(controller_selected)) {
            menu->next_mode = MENU_MODE_BROWSER;
        } 
        start_complete_restore = false;
        return;
        
    }

    rdpq_detach_show();
}

/**
 * Initialize the Controller Pak dump info view state.
 *
 * Sets the global cpak_path to the currently selected browser entry (cloned
 * from menu->browser.directory + menu->browser.entry->name), clears the
 * failure_message, and resets the start_complete_restore flag.
 */
void view_controller_pak_dump_info_init (menu_t *menu) {

    path_t *path = path_clone_push(menu->browser.directory, menu->browser.entry->name);

    sprintf(cpak_path, "%s", path_get(path));
    start_complete_restore = false;
    sprintf(failure_message, " ");

    path_free(path);
}

/**
 * Update and render the Controller Pak dump info view for one frame.
 *
 * Processes user input/actions for this view and then draws the view UI onto the given display surface.
 *
 * @param menu Current menu state for the view.
 * @param display Target surface to render the view into.
 */
void view_controller_pak_dump_info_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}