#include <stdbool.h>
#include <stdio.h>
#include <libdragon.h>
#include "views.h"
#include "../sound.h"
#include "../fonts.h"
#include <fatfs/ff.h>
#include <errno.h>
#include <dir.h>
#include "utils/cpakfs_utils.h"

#define MAX_STRING_LENGTH 62

#define CPAK_EXTENSION ".mpk"   
#define CPAK_NOTE_EXTENSION ".mpkn"

static bool use_rtc;
static rtc_time_t rtc_time;
static char string_datetime_cpak[26];
static char failure_message_note[255];

static int16_t controller_selected;
static int16_t index_selected;

static bool has_mem;
static bool corrupted_pak;
static bool unmounted;
static bool ctr_p_data_loop; // to avoid repopulating the list multiple times
static cpakfs_stats_t cpakfs_stats;
static dir_t dir_entry;

static bool process_complete_full_dump;
static bool process_complete_note_dump;
static bool process_complete_format;
static bool process_complete_delete;
static bool error_message_displayed;

static char controller_pak_name_notes[MAX_NUM_NOTES][MAX_STRING_LENGTH];
static char controller_pak_name_notes_bank_size[MAX_NUM_NOTES][6]; // "(XXX)" = 5 chars + \0

static cpakfs_path_strings_t cpakfs_path_strings[MAX_NUM_NOTES];

static bool show_complete_dump_confirm_message;
static bool show_single_note_dump_confirm_message;
static bool show_single_note_delete_confirm_message;
static bool show_format_controller_pak_confirm_message;
static bool show_complete_write_confirm_message;

static bool start_complete_dump;
static bool start_single_note_dump;
static bool start_single_note_delete;
static bool start_format_controller_pak;

static char * CPAK_PATH = "sd:/cpak_saves";
static char * CPAK_PATH_NO_PRE = "/cpak_saves";
static char * CPAK_NOTES_PATH = "sd:/cpak_saves/notes";
static char * CPAK_NOTES_PATH_NO_PRE = "/cpak_saves/notes";

/**
 * Reset internal Controller Pak view state to initial defaults.
 *
 * Clears all operation and confirmation flags, process-complete and error indicators,
 * resets selection index and loop guard so the Controller Pak view starts fresh.
 */
static void reset_vars(){
    has_mem = false;
    corrupted_pak = false;
    ctr_p_data_loop = false;
    show_complete_dump_confirm_message = false;
    show_single_note_dump_confirm_message = false;
    show_single_note_delete_confirm_message = false;
    show_format_controller_pak_confirm_message = false;
    show_complete_write_confirm_message = false;
    start_complete_dump = false;
    start_single_note_dump = false;
    start_single_note_delete = false;
    start_format_controller_pak = false;
    index_selected = 0;
    process_complete_full_dump = false;
    process_complete_note_dump = false;
    process_complete_format = false;
    process_complete_delete = false;
    error_message_displayed = false;
}

/**
 * Ensure a directory exists by attempting to create it.
 *
 * Attempts to create the directory at `dirpath` using `f_mkdir`.
 * Does not propagate errors; `FR_EXIST` (already exists) is treated as a non-fatal condition.
 *
 * @param dirpath Path of the directory to create.
 */
static void create_directory(const char *dirpath) {
    FRESULT res = f_mkdir(dirpath);
    
    if (res == FR_OK) {
        //debugf("Directory created: %s\n", dirpath);
    } else if (res == FR_EXIST) {
        //debugf("Directory already exists: %s\n", dirpath);
    } else {
        //debugf("Failed to create directory: %s (Error Code: %d)\n", dirpath, res);
    }
}

/**
 * Fill formatted_time with the current RTC timestamp as "YYYY-MM-DD_HHMMSS".
 *
 * Retrieves the real-time clock via rtc_get() and writes a null-terminated
 * timestamp string in the format: year-month-day_hourminutesecond
 * (e.g. "2025-09-21_142530").
 *
 * @param formatted_time Buffer that receives the timestamp; must be at least 18 bytes.
 */
static void get_rtc_time(char* formatted_time) {
    rtc_get(&rtc_time);

    sprintf(formatted_time, "%04d-%02d-%02d_%02d%02d%02d",
            rtc_time.year, rtc_time.month + 1, rtc_time.day,
            rtc_time.hour, rtc_time.min, rtc_time.sec);
}

/**
 * Clear per-note display buffers used for Controller Pak listing.
 *
 * Resets all entries (0..MAX_NUM_NOTES-1) of controller_pak_name_notes,
 * controller_pak_name_notes_bank_size, and the fields of cpakfs_path_strings
 * (gamecode, pubcode, filename, ext) to a single-space string, effectively
 * clearing any previous content for UI display/storage.
 */
static void free_controller_pak_name_notes() {
    for (int i = 0; i < MAX_NUM_NOTES; ++i) {
        sprintf(controller_pak_name_notes[i], " ");
        sprintf(controller_pak_name_notes_bank_size[i], " ");
        sprintf(cpakfs_path_strings[i].gamecode, " ");
        sprintf(cpakfs_path_strings[i].pubcode, " ");
        sprintf(cpakfs_path_strings[i].filename, " ");
        sprintf(cpakfs_path_strings[i].ext, " ");
    }
}

/**
 * Check for a Controller Pak on the given controller port and update mount state.
 *
 * Queries the controller accessory type and updates internal flags:
 * - sets `has_mem` to true when a Controller Pak is present;
 * - if a pak is removed and was previously mounted, unmounts all CPAK filesystems and marks `unmounted`;
 * - if a pak is newly present and previously unmounted, attempts to mount the CPAK filesystem,
 *   sets `corrupted_pak` on mount failure, retrieves CPAK stats on success, and clears `unmounted`.
 *
 * @param controller Controller port index to query.
 */
static void check_accessories(int controller) {
    
    joypad_accessory_type_t val =  joypad_get_accessory_type(controller);

    has_mem = val == JOYPAD_ACCESSORY_TYPE_CONTROLLER_PAK;

    if (!has_mem && !unmounted) {
        unmount_all_cpakfs();
        unmounted = true;
    } else if (has_mem && unmounted) {
        corrupted_pak = mount_cpakfs(controller) < 0 ? true : false;
        if (!corrupted_pak) {
            cpakfs_get_stats(controller, &cpakfs_stats);
        }
        unmounted = false;
    }
}

/**
 * Format the currently selected Controller Pak.
 *
 * Attempts to format the Controller Pak mounted on the global `controller_selected`.
 * On failure records a human-readable message in `failure_message_note` and sets
 * `error_message_displayed`. Regardless of success or failure, resets view state
 * via `reset_vars()`, unmounts all Controller Pak filesystems with `unmount_all_cpakfs()`,
 * marks the global `unmounted` flag true, and sets `process_complete_format` to
 * indicate the operation has completed.
 */
static void format_controller_pak () {
    sprintf(failure_message_note, " ");
    int res = cpakfs_format(controller_selected, false);
    if (res < 0) {
        sprintf(failure_message_note, "Unable to format Controller Pak on controller %d!\nError code: %d", controller_selected + 1, res);
        error_message_displayed = true;
    }
    reset_vars();
    unmount_all_cpakfs();
    unmounted = true;
    process_complete_format = true;
}

/**
 * Activate the "delete single note" confirmation prompt.
 *
 * Marks the view state so the UI will show the single-note delete confirmation
 * dialog. Intended as a menu/component action handler.
 */
static void active_single_note_delete_message(menu_t *menu, void *arg) {
    show_single_note_delete_confirm_message = true;
}

/**
 * Activate the "Format Controller Pak" confirmation message.
 *
 * Sets the internal flag causing the format confirmation dialog to be displayed.
 */
static void active_format_controller_pak_message(menu_t *menu, void *arg) {
    show_format_controller_pak_confirm_message = true;
}

/**
 * Activate the "restore Controller Pak" confirmation prompt.
 *
 * Marks that the UI should display the confirmation message for completing a write/restore
 * operation by setting the internal flag `show_complete_write_confirm_message` to true.
 */
static void active_restore_controller_pak_message(menu_t *menu, void *arg) {
    show_complete_write_confirm_message = true;
}

static component_context_menu_t options_context_menu = {
    .list = {
        { .text = "Format Controller Pak", .action = active_format_controller_pak_message },
        { .text = "Delete single note", .action = active_single_note_delete_message },
        { .text = "Restore a dump to the Controller Pak", .action = active_restore_controller_pak_message },
        COMPONENT_CONTEXT_MENU_LIST_END,
    }
};

/**
 * Populate display metadata for a Controller Pak entry.
 *
 * Writes the given entry's display name, block-size string, and parsed full-path
 * metadata into the module's per-note arrays at the specified index. The function
 * resolves the full mount path using CPAK_MOUNT_ARRAY[controller], queries the
 * filesystem for the note's block size and stores a size string of the form
 * "(###)" (or a single space on error), copies the entry name into
 * controller_pak_name_notes[index], and fills cpakfs_path_strings[index] via
 * parse_cpakfs_fullname().
 *
 * @param controller Controller port index used to prefix the mount path
 *        (selects CPAK_MOUNT_ARRAY[controller]).
 * @param index Destination index into the per-note arrays (controller_pak_name_notes,
 *        controller_pak_name_notes_bank_size, and cpakfs_path_strings).
 * @param entry_name Filename (basename) of the note on the Controller Pak.
 */
static void write_note_name_info_list(int16_t controller, int index, char* entry_name) {
    char filename_cpak[256];
    sprintf(filename_cpak, "%s%s", CPAK_MOUNT_ARRAY[controller], entry_name);
    int size = get_block_size_from_fs_path(filename_cpak);

    if (size < 0) {
        sprintf(controller_pak_name_notes_bank_size[index], " ");
    } else {
        sprintf(controller_pak_name_notes_bank_size[index], "(%-3.3d)", size);
    }
    snprintf(controller_pak_name_notes[index], MAX_STRING_LENGTH, "%s", entry_name);
    parse_cpakfs_fullname(entry_name, &cpakfs_path_strings[index]);
}

/**
 * Populate the in-memory list of Controller Pak notes for the currently selected controller.
 *
 * If a memory pak is present (has_mem) and the list has not yet been populated (ctr_p_data_loop is false),
 * the function clears the existing note display buffers, enumerates files in the selected controller's
 * mount directory, and for each entry calls write_note_name_info_list to record name and size metadata.
 * Enumeration stops when there are no more directory entries or when MAX_NUM_NOTES is reached.
 *
 * Side effects:
 * - Reads/writes global state: controller_selected, ctr_p_data_loop, controller_pak_name_notes arrays and related globals via write_note_name_info_list.
 * - Calls free_controller_pak_name_notes(), dir_findfirst(), dir_findnext(), and write_note_name_info_list().
 */
static void populate_list_cpakfs() {  
    if (has_mem && !ctr_p_data_loop) {
        
        free_controller_pak_name_notes();

        if (dir_findfirst(CPAK_MOUNT_ARRAY[controller_selected], &dir_entry) >= 0) {
            
            write_note_name_info_list(controller_selected, 0, dir_entry.d_name);

            int i = 1;     
            while(dir_findnext(CPAK_MOUNT_ARRAY[controller_selected], &dir_entry) == 0) {
                
                write_note_name_info_list(controller_selected, i, dir_entry.d_name);
            
                i++;
                if (i >= MAX_NUM_NOTES) break;

                ctr_p_data_loop = true;
            }
        }
    }
}

/**
 * Dump all 128 sectors of a Controller Pak to a timestamped file on storage.
 *
 * Reads 128 mempak sectors from controller port `_port` into a temporary buffer,
 * writes the buffered contents to a file named "CPAK_<YYYY-MM-DD_HHMMSS><CPAK_EXTENSION>"
 * under CPAK_PATH, and marks the operation complete by setting `process_complete_full_dump`.
 *
 * On failure this function sets `failure_message_note` with a human-readable message
 * and flips `error_message_displayed` to true. Failures covered include:
 * - heap allocation failure for the dump buffer,
 * - a sector read error from `read_mempak_sector`, and
 * - file open or write errors when persisting the dump.
 *
 * @param _port Controller port index to read the Controller Pak from.
 */
static void dump_complete_cpak(int _port) {
    sprintf(failure_message_note, " ");
    uint8_t* data = malloc(MEMPAK_BLOCK_SIZE * 128 * sizeof(uint8_t));

    if (!data) {
        sprintf(failure_message_note, "Memory allocation failed!");
        error_message_displayed = true;
        return;
    }
    
    surface_t *d;
    for (int i = 0; i < 128; i++) {

        d = display_try_get();
        rdpq_attach(d, NULL);

        ui_components_layout_draw();

        ui_components_messagebox_draw(
            "Do you want to dump the Controller Pak?\n\n"
            "A: Yes     B: No"
        );   
        

        if (read_mempak_sector(_port, i, data + (i * MEMPAK_BLOCK_SIZE)) != 0) {
            free(data);
            sprintf(failure_message_note, "Failed to read Controller Pak sector %d", i);
            error_message_displayed = true;
            return;
        }

        ui_components_loader_draw((float) i / 128.0f, "Dumping Controller Pak...");

        rdpq_detach_show();
    }

    get_rtc_time(string_datetime_cpak);

    char complete_filename[200];
    sprintf(complete_filename, "%s/CPAK_%s%s", CPAK_PATH, string_datetime_cpak, CPAK_EXTENSION);


    FILE *fp = fopen(complete_filename, "w");
    if (!fp) {
        sprintf(failure_message_note, "Failed to open file for writing: %s\n", complete_filename);
        error_message_displayed = true;
        fclose(fp);
        free(data);
        return;
    }

    if (fwrite(data, 1, MEMPAK_BLOCK_SIZE * 128, fp) != MEMPAK_BLOCK_SIZE * 128) {
        sprintf(failure_message_note, "Failed to write data to file: %s\n", complete_filename);
        error_message_displayed = true;
        fclose(fp);
        free(data);
        return;
    }
    
    fclose(fp);
    free(data);
    process_complete_full_dump = true;
}

/**
 * Dump a single Controller Pak note to the host filesystem with a timestamped filename.
 *
 * Reads the note at the given slot index from the currently selected controller's mounted
 * Controller Pak and writes it to CPAK_NOTES_PATH using the current RTC timestamp and
 * CPAK_NOTE_EXTENSION. On success sets the global flag `process_complete_note_dump`.
 *
 * On failure (source missing, unable to create destination, or write error) this function
 * sets `failure_message_note` and `error_message_displayed` with a human-readable message
 * and returns without completing the dump.
 *
 * The first parameter `_port` is unused in the current implementation and is ignored.
 *
 * @param _port Unused/ignored port parameter.
 * @param selected_index Index of the note (slot) on the Controller Pak to dump.
 */
static void dump_single_note(int _port, int16_t selected_index) {
    sprintf(failure_message_note, " ");
    FILE *fSource, *fDump;
    char filename_note[256];

    get_rtc_time(string_datetime_cpak);

    sprintf(filename_note, "%s%s", CPAK_MOUNT_ARRAY[controller_selected], controller_pak_name_notes[selected_index]);

    fSource = fopen(filename_note, "rb");
    if (fSource == NULL) {
        sprintf(failure_message_note, "No note found in controller %d at slot %d!", controller_selected + 1, selected_index + 1);
        error_message_displayed = true;
        return;
    }

    sprintf(filename_note, "%s/%s_%s%s", CPAK_NOTES_PATH, controller_pak_name_notes[selected_index], string_datetime_cpak, CPAK_NOTE_EXTENSION);

    fDump = fopen(filename_note, "wb");
    if (fDump == NULL) {
        sprintf(failure_message_note, "Unable to create dump file: %s", filename_note);
        fclose(fSource);
        error_message_displayed = true;
        return;
    }

    surface_t *d = display_try_get();
    rdpq_attach(d, NULL);
    
    ui_components_messagebox_draw(
        "Which note would you like to dump?\n\n"
        "Note selected: N.%-2.2d\n\n"
        "A: Select    B: No\n"
        "<- / ->: Select note number",
        index_selected + 1
    );
    ui_components_loader_draw(0, "Dumping Controller Pak note...");
    rdpq_detach_show();

    char buffer[4096];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), fSource)) > 0) {
        size_t bytesWritten = fwrite(buffer, 1, bytesRead, fDump);
        if (bytesWritten < bytesRead) {
            fclose(fSource);
            fclose(fDump);
            sprintf(failure_message_note, "Write error while copying to destination!");
            error_message_displayed = true;
            return;
        }
    }

    fclose(fSource);
    fclose(fDump);
    process_complete_note_dump = true;

}

/**
 * Check whether a file exists and is readable.
 *
 * Attempts to open the given path for reading; returns true if the file can be opened.
 *
 * @param filename Path to the file to check.
 * @return true if the file exists and is readable, false otherwise.
 */
static bool file_exists(const char *filename)
{
    FILE *fp = fopen(filename, "r");
    bool is_exist = false;
    if (fp != NULL)
    {
        is_exist = true;
        fclose(fp);
    }
    return is_exist;
}

/**
 * Delete a single Controller Pak note file at the given slot.
 *
 * Attempts to remove the note file corresponding to `selected_index` from the
 * currently selected controller. If the file does not exist or deletion fails,
 * an error message is written to `failure_message_note` and `error_message_displayed`
 * is set. On successful deletion the view state is reset, all Controller Pak
 * filesystems are unmounted, `unmounted` is set true, and `process_complete_delete`
 * is set to indicate completion.
 *
 * @param _port Unused in this implementation (kept for API compatibility).
 * @param selected_index Zero-based index of the note slot to delete.
 */
static void delete_single_note(int _port, unsigned short selected_index) {
    sprintf(failure_message_note, " ");
    char filename_note[256];

    sprintf(filename_note, "%s%s", CPAK_MOUNT_ARRAY[controller_selected], controller_pak_name_notes[selected_index]);

    if (!file_exists(filename_note)) {
        sprintf(failure_message_note, "No note found in controller %d at slot %d!", controller_selected + 1, selected_index + 1);
        error_message_displayed = true;
        return;
    }

    remove(filename_note);

    if (file_exists(filename_note)) {
        sprintf(failure_message_note, "Failed to delete file: %s", filename_note);
        error_message_displayed = true;
        return;
    }  

    reset_vars();
    unmount_all_cpakfs();
    unmounted = true;
    process_complete_delete = true;
}

/**
 * Check whether any Controller Pak operation has completed or an error is pending.
 *
 * Returns true if any of the completion flags (full dump, single-note dump,
 * format, delete) or the error-message flag is set.
 *
 * @return true if any process-complete or error flag is set; false otherwise.
 */
static bool is_one_of_process_complete() {
    return process_complete_full_dump 
    || process_complete_note_dump 
    || process_complete_format 
    || process_complete_delete
    || error_message_displayed;
}


/**
 * Process input and advance the Controller Pak management view state machine.
 *
 * Evaluates controller/menu actions and current confirmation/operation flags to:
 * - Drive navigation between controllers and the parent browser view.
 * - Show/handle context menus and confirmation dialogs.
 * - Set start flags for operations (full dump, single-note dump, single-note delete, format).
 * - Clear and acknowledge completion/error message states.
 * - Play appropriate UI sound effects.
 *
 * Side effects:
 * - Mutates multiple module-level state flags (e.g., start_complete_dump, start_single_note_dump,
 *   start_single_note_delete, start_format_controller_pak, process_complete_*, error_message_displayed,
 *   and various show_*_confirm_message flags).
 * - May set menu->next_mode to MENU_MODE_BROWSER.
 *
 * @param menu Pointer to the current menu state and input actions.
 */
static void process (menu_t *menu) {

    if (is_one_of_process_complete()) {
     
        if(process_complete_full_dump && menu->actions.enter) {
            sound_play_effect(SFX_ENTER);
            process_complete_full_dump = false;
            return;
        }

        if(process_complete_note_dump && menu->actions.enter) {
            sound_play_effect(SFX_ENTER);
            process_complete_note_dump = false;
            return;
        }

        if(process_complete_format && menu->actions.enter) {
            sound_play_effect(SFX_ENTER);
            process_complete_format = false;
            return;
        }

        if(process_complete_delete && menu->actions.enter) {
            sound_play_effect(SFX_ENTER);
            process_complete_delete = false;
            return;
        }

        if(error_message_displayed && menu->actions.enter) {
            sound_play_effect(SFX_ENTER);
            error_message_displayed = false;
            return;
        }
        
    } else {

        if (ui_components_context_menu_process(menu, &options_context_menu)) {
            return;
        }

        if (!show_complete_dump_confirm_message && 
            !show_complete_write_confirm_message && 
            !show_single_note_dump_confirm_message &&
            !show_single_note_delete_confirm_message &&
            !show_format_controller_pak_confirm_message) {
            if(menu->actions.go_left) {
                sound_play_effect(SFX_SETTING);
                controller_selected = ((controller_selected - 1) + 4) % 4;
                reset_vars();
            } else if (menu->actions.go_right) {
                sound_play_effect(SFX_SETTING);
                controller_selected = ((controller_selected + 1) + 4) % 4;
                reset_vars();
            } else if (menu->actions.back) {
                sound_play_effect(SFX_EXIT);
                menu->next_mode = MENU_MODE_BROWSER;
            } else if (menu->actions.options && use_rtc && has_mem) {
                sound_play_effect(SFX_SETTING);
                ui_components_context_menu_show(&options_context_menu);
            }
        }

        check_accessories(controller_selected);

        populate_list_cpakfs();

        if (has_mem && !corrupted_pak) {

            // Pressing A : dump the Controller Pak
            if (menu->actions.enter && 
                use_rtc && 
                !show_complete_dump_confirm_message && 
                !show_complete_write_confirm_message &&
                !show_single_note_dump_confirm_message &&
                !show_single_note_delete_confirm_message &&
                !show_format_controller_pak_confirm_message) {
                sound_play_effect(SFX_ENTER);
                show_complete_dump_confirm_message = true;
                return;
            } 

            // Pressing L or Z : dump a single note
            else if (menu->actions.lz_context && 
                use_rtc && 
                !show_complete_write_confirm_message && 
                !show_complete_dump_confirm_message &&
                !show_single_note_dump_confirm_message &&
                !show_single_note_delete_confirm_message &&
                !show_format_controller_pak_confirm_message) {
                sound_play_effect(SFX_ENTER);
                show_single_note_dump_confirm_message = true;
                return;
            }

            if (show_complete_dump_confirm_message && 
                !show_complete_write_confirm_message &&
                !show_single_note_dump_confirm_message &&
                !show_single_note_delete_confirm_message &&
                !show_format_controller_pak_confirm_message) {
                if (menu->actions.enter) {
                    sound_play_effect(SFX_ENTER);
                    show_complete_dump_confirm_message = false;
                    start_complete_dump = true;
                } else if (menu->actions.back) {
                    sound_play_effect(SFX_EXIT);
                    show_complete_dump_confirm_message = false;
                }
                return;
            } else if (show_complete_write_confirm_message && 
                !show_complete_dump_confirm_message &&
                !show_single_note_dump_confirm_message &&
                !show_single_note_delete_confirm_message &&
                !show_format_controller_pak_confirm_message) {
                if (menu->actions.back) {
                    sound_play_effect(SFX_EXIT);
                    show_complete_write_confirm_message = false;                    
                }
                return;
            } else if (show_single_note_dump_confirm_message && 
                !show_complete_dump_confirm_message &&
                !show_complete_write_confirm_message &&
                !show_single_note_delete_confirm_message &&
                !show_format_controller_pak_confirm_message) {
                if (menu->actions.enter) {
                    sound_play_effect(SFX_ENTER);
                    show_single_note_dump_confirm_message = false;
                    start_single_note_dump = true;
                } else if (menu->actions.back) {
                    sound_play_effect(SFX_EXIT);
                    show_single_note_dump_confirm_message = false;                    
                } else if (menu->actions.go_left) {
                    sound_play_effect(SFX_CURSOR);
                    index_selected = dec_index_note(index_selected);
                } else if (menu->actions.go_right) {
                    sound_play_effect(SFX_CURSOR);
                    index_selected = inc_index_note(index_selected);
                }
                return;
            }  else if (show_single_note_delete_confirm_message && 
                !show_complete_dump_confirm_message &&
                !show_complete_write_confirm_message &&
                !show_single_note_dump_confirm_message &&
                !show_format_controller_pak_confirm_message) {
                if (menu->actions.enter) {
                    show_single_note_delete_confirm_message = false;
                    sound_play_effect(SFX_ENTER);
                    start_single_note_delete = true;
                } else if (menu->actions.back) {
                    show_single_note_delete_confirm_message = false;
                    sound_play_effect(SFX_EXIT);
                } else if (menu->actions.go_left) {
                    sound_play_effect(SFX_CURSOR);
                    index_selected = dec_index_note(index_selected);
                } else if (menu->actions.go_right) {
                    sound_play_effect(SFX_CURSOR);
                    index_selected = inc_index_note(index_selected);
                }
                return;
            } else if (show_format_controller_pak_confirm_message && 
                !show_complete_dump_confirm_message &&
                !show_complete_write_confirm_message &&
                !show_single_note_dump_confirm_message &&
                !show_single_note_delete_confirm_message) {
                if (menu->actions.enter) {
                    sound_play_effect(SFX_ENTER);
                    show_format_controller_pak_confirm_message = false;
                    start_format_controller_pak = true;
                } else if (menu->actions.back) {
                    sound_play_effect(SFX_EXIT);
                    show_format_controller_pak_confirm_message = false;                    
                } 
                return;
            }
        } else if (has_mem && corrupted_pak) {

            if (!show_format_controller_pak_confirm_message) {
                if (menu->actions.back) {
                    sound_play_effect(SFX_EXIT);
                    menu->next_mode = MENU_MODE_BROWSER;
                } else if (menu->actions.enter) {
                    sound_play_effect(SFX_ENTER);
                    show_format_controller_pak_confirm_message = true;
                }
            } else {
                if (menu->actions.enter) {
                    sound_play_effect(SFX_ENTER);
                    show_format_controller_pak_confirm_message = false;
                    start_format_controller_pak = true;
                } else if (menu->actions.back) {
                    sound_play_effect(SFX_EXIT);
                    show_format_controller_pak_confirm_message = false;
                } 
            }
        }
    }
}

/**
 * Render the Controller Pak management view and handle view-triggered actions.
 *
 * Draws the full UI for the Controller Pak filesystem manager into the provided
 * surface: header, controller status (presence/corruption and free blocks),
 * per-slot note table (name, game code, extension, size), context menu, action
 * bars, and any active message boxes (errors, confirmations, and process
 * completions). Chooses text styles based on RTC availability and Controller
 * Pak state.
 *
 * This function also drives a small part of the view's control flow: when the
 * user confirms an operation it will detach the RDP, invoke the corresponding
 * operation helper (complete dump, single-note dump, single-note delete, or
 * format), and return early. It sets or reads global view state flags (error
 * messages, process-complete flags, start_* confirmation flags, selection
 * indices) and may update those flags as a result of operations. It attaches
 * and detaches the RDP display context around drawing.
 *
 * Note: the function has side effects beyond rendering (may call
 * dump_complete_cpak, dump_single_note, delete_single_note, format_controller_pak
 * and may set global error/process flags); callers should expect that a
 * confirmed operation will be started and that this function can return before
 * finishing the frame when such an operation begins.
 */
static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    ui_components_background_draw();

    ui_components_layout_draw();

    char has_mem_text[64];
    char free_space_cpak_text[64];
    menu_font_type_t style;

    style = STL_DEFAULT;

    if (has_mem) {
        sprintf(has_mem_text, "Controller Pak detected");
        style = STL_GREEN;

        if (has_mem && !corrupted_pak) {
            style = STL_GREEN;
            sprintf(free_space_cpak_text, "%d/123 free blocks", cpakfs_stats.pages.total - cpakfs_stats.pages.used);
        } else if (has_mem && corrupted_pak) {
            sprintf(has_mem_text, "%s %s", has_mem_text, " (is NOT valid. Corrupted)");
            style = STL_ORANGE;
            sprintf(free_space_cpak_text, " ");
        }
    } else {
        sprintf(has_mem_text, "No Controller Pak detected");
        style = STL_ORANGE;
        sprintf(free_space_cpak_text, " ");
    }

    ui_components_main_text_draw(STL_DEFAULT,
        ALIGN_CENTER, VALIGN_TOP,
        "CONTROLLER PAK MANAGEMENT\n"
    );

    ui_components_main_text_draw(STL_DEFAULT,
        ALIGN_LEFT, VALIGN_TOP,
        "\n"
        "Controller: < %d >\n",
            controller_selected + 1
    );

    ui_components_main_text_draw(style,
        ALIGN_LEFT, VALIGN_TOP,
        "\n"
        "                   %s\n",
        has_mem_text
    );

    if (has_mem) {
        ui_components_main_text_draw(STL_DEFAULT,
            ALIGN_LEFT, VALIGN_TOP,
            "\n"
            "\n"
            "                   %s\n",
            free_space_cpak_text
        );

        ui_components_main_text_draw(STL_DEFAULT,
            ALIGN_LEFT, VALIGN_TOP,
            "\n"
            "\n"
            "\n"
            "            Name           Code    Ext.    Size [blocks]\n"
        );

        ui_components_main_text_draw(style,
            ALIGN_LEFT, VALIGN_TOP,
            "\n"
            "\n"
            "\n"
            "\n"
            "N.01: %s\n"
            "N.02: %s\n"
            "N.03: %s\n"
            "N.04: %s\n"
            "N.05: %s\n"
            "N.06: %s\n"
            "N.07: %s\n"
            "N.08: %s\n"
            "N.09: %s\n"
            "N.10: %s\n"
            "N.11: %s\n"
            "N.12: %s\n"
            "N.13: %s\n"
            "N.14: %s\n"
            "N.15: %s\n"
            "N.16: %s\n",
            cpakfs_path_strings[0].filename,
            cpakfs_path_strings[1].filename,
            cpakfs_path_strings[2].filename,
            cpakfs_path_strings[3].filename,
            cpakfs_path_strings[4].filename,
            cpakfs_path_strings[5].filename,
            cpakfs_path_strings[6].filename,
            cpakfs_path_strings[7].filename,
            cpakfs_path_strings[8].filename,
            cpakfs_path_strings[9].filename,
            cpakfs_path_strings[10].filename,
            cpakfs_path_strings[11].filename,
            cpakfs_path_strings[12].filename,
            cpakfs_path_strings[13].filename,
            cpakfs_path_strings[14].filename,
            cpakfs_path_strings[15].filename
        );

        ui_components_main_text_draw(style,
            ALIGN_LEFT, VALIGN_TOP,
            "\n"
            "\n"
            "\n"
            "\n"
            "                           %s\n"
            "                           %s\n"
            "                           %s\n"
            "                           %s\n"
            "                           %s\n"
            "                           %s\n"
            "                           %s\n"
            "                           %s\n"
            "                           %s\n"
            "                           %s\n"
            "                           %s\n"
            "                           %s\n"
            "                           %s\n"
            "                           %s\n"
            "                           %s\n"
            "                           %s\n",
            cpakfs_path_strings[0].gamecode,
            cpakfs_path_strings[1].gamecode,
            cpakfs_path_strings[2].gamecode,
            cpakfs_path_strings[3].gamecode,
            cpakfs_path_strings[4].gamecode,
            cpakfs_path_strings[5].gamecode,
            cpakfs_path_strings[6].gamecode,
            cpakfs_path_strings[7].gamecode,
            cpakfs_path_strings[8].gamecode,
            cpakfs_path_strings[9].gamecode,
            cpakfs_path_strings[10].gamecode,
            cpakfs_path_strings[11].gamecode,
            cpakfs_path_strings[12].gamecode,
            cpakfs_path_strings[13].gamecode,
            cpakfs_path_strings[14].gamecode,
            cpakfs_path_strings[15].gamecode
        );

        ui_components_main_text_draw(style,
            ALIGN_LEFT, VALIGN_TOP,
            "\n"
            "\n"
            "\n"
            "\n"
            "                                    %s\n"
            "                                    %s\n"
            "                                    %s\n"
            "                                    %s\n"
            "                                    %s\n"
            "                                    %s\n"
            "                                    %s\n"
            "                                    %s\n"
            "                                    %s\n"
            "                                    %s\n"
            "                                    %s\n"
            "                                    %s\n"
            "                                    %s\n"
            "                                    %s\n"
            "                                    %s\n"
            "                                    %s\n", 
            cpakfs_path_strings[0].ext,
            cpakfs_path_strings[1].ext,
            cpakfs_path_strings[2].ext,
            cpakfs_path_strings[3].ext,
            cpakfs_path_strings[4].ext,
            cpakfs_path_strings[5].ext,
            cpakfs_path_strings[6].ext,
            cpakfs_path_strings[7].ext,
            cpakfs_path_strings[8].ext,
            cpakfs_path_strings[9].ext,
            cpakfs_path_strings[10].ext,
            cpakfs_path_strings[11].ext,
            cpakfs_path_strings[12].ext,
            cpakfs_path_strings[13].ext,
            cpakfs_path_strings[14].ext,
            cpakfs_path_strings[15].ext
        );

        ui_components_main_text_draw(style,
            ALIGN_LEFT, VALIGN_TOP,
            "\n"
            "\n"
            "\n"
            "\n"
            "                                              %s\n"
            "                                              %s\n"
            "                                              %s\n"
            "                                              %s\n"
            "                                              %s\n"
            "                                              %s\n"
            "                                              %s\n"
            "                                              %s\n"
            "                                              %s\n"
            "                                              %s\n"
            "                                              %s\n"
            "                                              %s\n"
            "                                              %s\n"
            "                                              %s\n"
            "                                              %s\n"
            "                                              %s\n", 
            controller_pak_name_notes_bank_size[0],
            controller_pak_name_notes_bank_size[1],
            controller_pak_name_notes_bank_size[2],
            controller_pak_name_notes_bank_size[3],
            controller_pak_name_notes_bank_size[4],
            controller_pak_name_notes_bank_size[5],
            controller_pak_name_notes_bank_size[6],
            controller_pak_name_notes_bank_size[7],
            controller_pak_name_notes_bank_size[8],
            controller_pak_name_notes_bank_size[9],
            controller_pak_name_notes_bank_size[10],
            controller_pak_name_notes_bank_size[11],
            controller_pak_name_notes_bank_size[12],
            controller_pak_name_notes_bank_size[13],
            controller_pak_name_notes_bank_size[14],
            controller_pak_name_notes_bank_size[15]
        );
    }

    ui_components_context_menu_draw(&options_context_menu);

    style = (has_mem && !corrupted_pak) ? STL_DEFAULT : STL_GRAY;

    if (!use_rtc) {
        ui_components_main_text_draw(STL_ORANGE,
            ALIGN_LEFT, VALIGN_TOP,
            "No RTC\n"
        );
        style = STL_GRAY;
    }

    if (!corrupted_pak) {

        ui_components_actions_bar_text_draw(style,
            ALIGN_LEFT, VALIGN_TOP,
            "A: Dump Pak\n"
            "B: Back\n"
        );
        ui_components_actions_bar_text_draw(style,
            ALIGN_RIGHT, VALIGN_TOP,
            "L|Z: Dump single Note\n"
            "R: Options\n"
        );
    } else {
        ui_components_actions_bar_text_draw(style,
            ALIGN_LEFT, VALIGN_TOP,
            "A: Format Controller Pak\n"
            "\n"
        );
    }

    if (error_message_displayed) {
        ui_components_messagebox_draw(
            "Error: %s\n\n"
            "Press A to continue.",
            failure_message_note
        );   
    }

    if (process_complete_format) {
        ui_components_messagebox_draw(
            "Controller Pak formatted.\n\n"
            "Press A to continue."
        );   
    }

    if (process_complete_full_dump) {
        ui_components_messagebox_draw(
            "Complete dump created in:\n"
            "%s\n\n"
            "Press A to continue.",
            CPAK_PATH
        );   
    }

    if (process_complete_note_dump) {
        ui_components_messagebox_draw(
            "Note dumped in:\n"
            "%s/notes\n\n"
            "Press A to continue.",
            CPAK_PATH
        );   
    }

    if (process_complete_delete) {
        ui_components_messagebox_draw(
            "Note %d deleted from Controller Pak.\n\n"
            "Press A to continue.",
            index_selected + 1
        );   
    }

    if (show_complete_dump_confirm_message && 
        !start_complete_dump) {
        ui_components_messagebox_draw(
            "Do you want to dump the Controller Pak?\n\n"
            "A: Yes        B: No"
        );   
    } else if (show_complete_write_confirm_message) {
        ui_components_messagebox_draw(
            "To write a complete dump, select a file"
            " with the extension \".mpk\".\n\n"
            "B: Back"
        );   
    } 

    if (show_single_note_dump_confirm_message &&
        !start_single_note_dump) {
        ui_components_messagebox_draw(
            "Which note would you like to dump?\n\n"
            "Note selected: N.%-2.2d\n\n"
            "A: Select    B: No\n"
            "<- / ->: Select note number",
            index_selected + 1
        );
    }

    if (show_single_note_delete_confirm_message &&
        !start_single_note_delete) {
        ui_components_messagebox_draw(
            "Which note would you like to delete?\n\n"
            "Note selected: N.%-2.2d\n\n"
            "A: Select    B: No\n"
            "<- / ->: Select note number",
            index_selected + 1
        );
    }

    if (show_format_controller_pak_confirm_message && 
        !start_format_controller_pak) {
        ui_components_messagebox_draw(
            "Do you want to format the Controller pak?\n\n"
            "A: Yes        B: No"
        );   
    }

    if (start_complete_dump) {

        if (cpakfs_stats.pages.used <= 0) {
            rdpq_detach_show();
            sprintf(failure_message_note, "No data to dump in Controller Pak on controller %d!", controller_selected + 1);
            error_message_displayed = true;
            start_complete_dump = false;
            return;

        } else {
            rdpq_detach_show();
            dump_complete_cpak(controller_selected);
            start_complete_dump = false;
            return;
        }
    }

    if (start_single_note_dump) {
        rdpq_detach_show();
        dump_single_note(controller_selected, index_selected);
        start_single_note_dump = false;
        return;
    }

    if (start_single_note_delete) {
        rdpq_detach_show();
        delete_single_note(controller_selected, index_selected);
        start_single_note_delete = false;
        return;
    }

    if (start_format_controller_pak) {
        rdpq_detach_show();
        format_controller_pak();
        start_format_controller_pak = false;
        return;
    }
    
    rdpq_detach_show();
}

/**
 * Initialize the Controller Pak FS view state.
 *
 * Resets internal view variables and flags, unmounts any existing Controller Pak
 * filesystems, ensures on-disk directories for full dumps and notes exist, and
 * initializes the view's options context menu.
 *
 * The function uses menu->current_time to set whether RTC timestamps are available.
 *
 * @param menu Pointer to the current menu state; its `current_time` field is read
 *             to determine RTC availability for timestamped filenames.
 */
void view_controller_pakfs_init (menu_t *menu) {
    ctr_p_data_loop = false;
    controller_selected = 0;
    reset_vars();
    unmount_all_cpakfs();
    unmounted = true;

    use_rtc = menu->current_time >= 0 ? true : false;

    create_directory(CPAK_PATH_NO_PRE);
    create_directory(CPAK_NOTES_PATH_NO_PRE);

    ui_components_context_menu_init(&options_context_menu);
}

/**
 * Run the Controller Pak management view: update view state then render it.
 *
 * Calls the view's processing step to handle input and state transitions, then
 * draws the current UI onto the provided display surface.
 *
 * @param menu Current menu/context for the view.
 * @param display Surface to render the view onto.
 */
void view_controller_pakfs_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}