#include <stdbool.h>
#include <stdio.h>
#include <libdragon.h>
#include "views.h"
#include "../sound.h"
#include "../fonts.h"
#include <fatfs/ff.h>
#include "utils/cpakfs_utils.h"

static char cpak_note_path[255];
static int16_t controller_selected;
static char failure_message_note[255];
static bool start_note_restore;

/**
 * Restore a Controller Pak note dump file to the specified controller Pak.
 *
 * Attempts to write the note dump whose absolute path is stored in the global
 * `cpak_note_path` into the Controller Pak on `controller`. The function verifies
 * presence of a Controller Pak, mounts its filesystem, checks free pages/blocks,
 * selects or generates a unique destination filename, and copies the file data.
 *
 * On failure the global `failure_message_note` is populated with a human-readable
 * error description. The function mounts and unmounts the Controller Pak as
 * needed and writes the destination file into the CPAK filesystem.
 *
 * @param controller Index of the controller/Controller Pak to restore to (0-3).
 * @return true if the note was successfully restored; false on any failure.
 */
static bool restore_controller_pak_note(int controller) {
    sprintf(failure_message_note, " ");

    if (!has_cpak(controller)) {
        sprintf(failure_message_note, "No Controller Pak detected on controller %d!", controller + 1);
        return false;
    }

    char title[256];
    char filename_note[256];
    FILE *fSource, *fDestination;

    static cpakfs_stats_t cpakfs_stats; //to monitor free space

    extract_title_from_absolute_path(cpak_note_path, title, sizeof title);

    //mounting the CPAK:
    if (mount_cpakfs(controller) < 0){
        snprintf(failure_message_note, sizeof failure_message_note,
            "Failed to mount Controller Pak on controller %d!", controller + 1);
            return false;
    }
    
    cpakfs_get_stats(controller, &cpakfs_stats );

    //checking free space in the CPAK:
    int free_blocks = cpakfs_stats.pages.total - cpakfs_stats.pages.used;
    int free_notes = cpakfs_stats.notes.total - cpakfs_stats.notes.used;
    //debugf("Free blocks: %d blocks\n", free_blocks);
    //debugf("Free notes: %d notes\n", free_notes);

    if (free_notes <= 0) {
        sprintf(failure_message_note, "Not enough pages left on Controller Pak in controller %d!\n(Required: 1 / Available: 0)", controller + 1);
        cpakfs_unmount(controller);
        return false;
    }

    //debugf("Source filename: %s\n", cpak_note_path);

    //Opening the source file (the dump note file)
    fSource = fopen(cpak_note_path, "rb");
    if (fSource == NULL) {
        snprintf(failure_message_note, sizeof failure_message_note, "Failed to open source file: %s\n", cpak_note_path);
        cpakfs_unmount(controller);
        return false;
    }

    int size = get_block_size_from_fs_path(cpak_note_path);
    //debugf("Size in blocks: %d\n", size);

    if (size > free_blocks) {
        sprintf(failure_message_note, "Not enough space on Controller Pak in controller %d!\n(Required: %d / Available: %d)", controller + 1, size, free_blocks);
        fclose(fSource);
        cpakfs_unmount(controller);
        return false;
    }


    sprintf(filename_note, "%s%s", CPAK_MOUNT_ARRAY[controller], title);
    
    //debugf("Dest. filename: %s\n", filename_note);

    //Check if the file already exists, and if so, pick a unique name
    if (my_exists_full(filename_note)) {
        char unique_full[256];

        // Strip the prefix from filename_note to get just the CPAK full name:
        // (title already has no prefix, so pass 'title' directly)
        if (pick_unique_fullname_with_mount(CPAK_MOUNT_ARRAY[controller],
                                            title, /* NO prefix */
                                            unique_full, sizeof unique_full,
                                            my_exists_full) == 0)
        {
            strcpy(filename_note, unique_full);
            //debugf("File exists, new name picked: %s\n", filename_note);
        } else {
            cpakfs_unmount(controller);
            fclose(fSource);
            snprintf(failure_message_note, sizeof failure_message_note,
                     "Unable to pick a unique destination name for %s", title);
            return false;
        }
    }

    fDestination = fopen(filename_note, "wb");
    if (!fDestination) {
        fclose(fSource);
        cpakfs_unmount(controller);
        snprintf(failure_message_note, sizeof failure_message_note,
                 "Failed to open destination file: %s", filename_note);
        return false;
    }

    surface_t *d = display_try_get();
    rdpq_attach(d, NULL);
    
    ui_components_layout_draw();
    ui_components_loader_draw(0, "Restoring Controller Pak note...");
    rdpq_detach_show();

    char buffer[4096];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), fSource)) > 0) {
        size_t bytesWritten = fwrite(buffer, 1, bytesRead, fDestination);
        if (bytesWritten < bytesRead) {
            //debugf("Write error while copying to destination!\n");
            fclose(fSource);
            fclose(fDestination);
            cpakfs_unmount(controller);
            return false;
        }
    }

    fclose(fSource);
    fclose(fDestination);

    cpakfs_unmount(controller);

    return true;
}

/**
 * Handle input actions for the Controller Pak note-restore view.
 *
 * Processes navigation and confirmation/back actions from the provided menu context:
 * - Left/Right: move the selected controller index circularly among four controllers and play cursor sound.
 * - Back: play exit sound and set the menu's next mode to the browser.
 * - Enter: play enter sound and arm the restore operation by setting the start flag.
 *
 * Side effects:
 * - Updates global state: controller_selected and start_note_restore.
 * - Mutates menu->next_mode when the Back action is taken.
 *
 * @param menu Pointer to the current menu context whose action flags are inspected.
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
        start_note_restore = true;
    }
}

/**
 * Draw the Controller Pak note-dump confirmation view and handle immediate restore.
 *
 * Renders the background, instructional text, the path of the note dump, and any
 * failure message. If the global flag `start_note_restore` is true, the function
 * finalizes the UI, invokes restore_controller_pak_note() for the currently
 * selected controller, updates menu->next_mode to MENU_MODE_BROWSER on success,
 * and clears the start flag.
 *
 * @param menu Pointer to the current menu state; may be modified (menu->next_mode).
 * @param d    Display surface to draw onto.
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
        "\n"
        "\n",
        cpak_note_path
    );

    ui_components_main_text_draw(STL_ORANGE,
        ALIGN_CENTER, VALIGN_TOP,
        "\n"
        "\n"
        "\n"
        "\n"
        "%s\n",
        failure_message_note
    );

    
    ui_components_messagebox_draw(
        "Do you want to restore this note dump to the Controller Pak?\n\n"
        "Controller selected: %d\n\n"
        "A: Yes  B: No \n"
        "<- / ->: Change controller",
        controller_selected + 1
    );

    if (start_note_restore) {
        rdpq_detach_show();
        if (restore_controller_pak_note(controller_selected)) {
            menu->next_mode = MENU_MODE_BROWSER;
        } 
        start_note_restore = false;
        return;
        
    }

    rdpq_detach_show();
}

/**
 * Initialize the Controller Pak note-dump info view state.
 *
 * Copies the current browser entry path into the module-global cpak_note_path,
 * clears any previous failure message, and resets the restore trigger flag so
 * the view starts in a neutral state.
 *
 * @param menu Source menu whose browser directory and selected entry name are
 *             used to construct the note file path.
 */
void view_controller_pak_note_dump_info_init (menu_t *menu) {

    path_t *path = path_clone_push(menu->browser.directory, menu->browser.entry->name);

    sprintf(cpak_note_path, "%s", path_get(path));
    start_note_restore = false;
    sprintf(failure_message_note, " ");

    path_free(path);

}

/**
 * Update and render the Controller Pak note-dump information view.
 *
 * Processes user input and view state, then draws the view to the provided display surface.
 *
 * @param menu Current menu state/context for the view.
 * @param display Target rendering surface where the view is drawn.
 */
void view_controller_pak_note_dump_info_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}