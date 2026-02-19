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

static bool restore_controller_pak(int controller) {
    sprintf(failure_message, " ");

    if (!has_cpak(controller)) {
        sprintf(failure_message, "No Controller Pak detected on controller %d!", controller + 1);
        return false;
    }

    cpakfs_unmount(controller);

    cpak_io_context_t ctx;
    cpak_io_err_t err = cpak_restore_from_file(controller, cpak_path, &ctx);

    /* Log restore attempt after validation passes */
    if (err == CPAK_IO_OK || err == CPAK_IO_ERR_FILE_READ || err == CPAK_IO_ERR_PAK_WRITE) {
        debugf("Restoring Controller Pak: %ld bytes (%d banks)\n", ctx.filesize, ctx.total_banks);
    }

    switch (err) {
        case CPAK_IO_OK:
            sprintf(failure_message, "Dump restored on controller %d!", controller + 1);
            return true;
        case CPAK_IO_ERR_NO_PAK:
            sprintf(failure_message, "No Controller Pak detected on controller %d!", controller + 1);
            return false;
        case CPAK_IO_ERR_ALLOC:
            sprintf(failure_message, "Memory allocation failed!");
            return false;
        case CPAK_IO_ERR_FILE_OPEN:
            sprintf(failure_message, "Failed to open file for reading!");
            return false;
        case CPAK_IO_ERR_FILE_SEEK:
            sprintf(failure_message, "Seek failed!");
            return false;
        case CPAK_IO_ERR_FILE_FTELL:
            sprintf(failure_message, "ftell failed!");
            return false;
        case CPAK_IO_ERR_PROBE_BANKS:
            sprintf(failure_message, "Cannot probe Controller Pak banks (err=%d)!", ctx.error_code);
            return false;
        case CPAK_IO_ERR_TOO_LARGE:
            sprintf(failure_message, "Dump file too large (%d banks) for controller (%d banks)!",
                    ctx.total_banks, ctx.device_banks);
            return false;
        case CPAK_IO_ERR_FILE_READ:
            sprintf(failure_message, "Read error from dump file!");
            return false;
        case CPAK_IO_ERR_PAK_WRITE:
            if (ctx.bytes_actual != (int)ctx.bytes_expected && ctx.error_code >= 0) {
                sprintf(failure_message, "Short write on bank %d: wrote %d / %zu bytes",
                        ctx.failed_bank, ctx.bytes_actual, ctx.bytes_expected);
            } else {
                sprintf(failure_message, "Failed to write bank %d to Controller Pak! errno=%d",
                        ctx.failed_bank, ctx.error_code);
            }
            return false;
        default:
            sprintf(failure_message, "Unknown error restoring Controller Pak!");
            return false;
    }
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
        start_complete_restore = true;
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
        "◀- / -▶: Change controller",
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

void view_controller_pak_dump_info_init (menu_t *menu) {

    path_t *path = path_clone_push(menu->browser.directory, menu->browser.entry->name);

    sprintf(cpak_path, "%s", path_get(path));
    start_complete_restore = false;
    sprintf(failure_message, " ");

    path_free(path);
}

void view_controller_pak_dump_info_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}
