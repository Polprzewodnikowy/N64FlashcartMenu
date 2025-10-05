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

    unmount_all_cpakfs();

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
    if (file_exists_full(filename_note)) {
        char unique_full[256];

        // Strip the prefix from filename_note to get just the CPAK full name:
        // (title already has no prefix, so pass 'title' directly)
        if (pick_unique_fullname_with_mount(CPAK_MOUNT_ARRAY[controller],
                                            title, /* NO prefix */
                                            unique_full, sizeof unique_full,
                                            file_exists_full) == 0)
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

    sprintf(failure_message_note, "Note restored on controller %d!", controller + 1);

    return true;
}

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
        if (restore_controller_pak_note(controller_selected) && !failure_message_note[0]) {
            menu->next_mode = MENU_MODE_BROWSER;
        } 
        start_note_restore = false;
        return;
        
    }

    rdpq_detach_show();
}

void view_controller_pak_note_dump_info_init (menu_t *menu) {

    path_t *path = path_clone_push(menu->browser.directory, menu->browser.entry->name);

    sprintf(cpak_note_path, "%s", path_get(path));
    start_note_restore = false;
    sprintf(failure_message_note, " ");

    path_free(path);

}

void view_controller_pak_note_dump_info_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}
