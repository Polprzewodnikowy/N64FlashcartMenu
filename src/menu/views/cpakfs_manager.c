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

#define MEMPAK_BANK_SIZE 32768

#define CPAK_EXTENSION ".pak"   
#define CPAK_NOTE_EXTENSION ".paknote"

static bool use_rtc;
static char string_datetime_cpak[26];
static char failure_message_note[255];

static int16_t controller_selected;
static int16_t index_selected;

static bool mounted[4] = { false, false, false, false };
static bool has_pak[4] = { false, false, false, false };
static bool corrupted[4] = { false, false, false, false };
static cpakfs_stats_t stats_per_port[4];

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
 * Reset controller-pak UI and operation state flags to their default (inactive) values.
 *
 * Clears global flags that track accessory presence, corruption, data-listing loop state,
 * confirmation dialogs, start/process completion signals, and error display state.
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
    process_complete_full_dump = false;
    process_complete_note_dump = false;
    process_complete_format = false;
    process_complete_delete = false;
    error_message_displayed = false;
}

/**
 * Ensure a directory exists at the given filesystem path by attempting to create it.
 *
 * If the directory already exists the function leaves it unchanged; creation errors are ignored.
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
 * Write the current local date and time into the provided buffer using the format YYYY-MM-DD_HHMMSS.
 * @param formatted_time Buffer to receive the null-terminated timestamp string; must be at least 18 bytes.
 */
static void get_rtc_time(char* formatted_time) {
    time_t t = time(NULL);

    struct tm tm = *localtime(&t);

    sprintf(formatted_time, "%04d-%02d-%02d_%02d%02d%02d",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);
}

/**
 * Reset per-note name, bank-size, and path fields to a single-space string.
 *
 * Clears UI-visible arrays that store controller pak note filenames, their
 * displayed bank-size strings, and parsed CPAK filesystem path components
 * (gamecode, pubcode, filename, ext) for all note slots.
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
 * Check whether a Controller Pak is present on the given controller port and update related state.
 *
 * Detects the accessory on the specified controller port and updates global state accordingly:
 * - sets `has_pak[controller]`, `mounted[controller]`, `corrupted[controller]`, and `stats_per_port[controller]`
 * - sets global flags `has_mem`, `corrupted_pak`, and `cpakfs_stats`
 * - mounts the pak when newly present and retrieves stats, or unmounts and clears state when removed
 * - when a pak is newly detected or removed, clears per-note name data and resets `ctr_p_data_loop`
 *
 * @param controller Index of the controller port to check (typically 0–3).
 */
static void check_accessories(int controller) {
    bool was_present = has_pak[controller];

    joypad_accessory_type_t acc = joypad_get_accessory_type(controller);
    bool present = (acc == JOYPAD_ACCESSORY_TYPE_CONTROLLER_PAK);
    has_pak[controller] = present;

    if (!present) {
        if (mounted[controller]) {
            cpakfs_unmount(controller);
            mounted[controller] = false;
        }
        corrupted[controller] = false;
        memset(&stats_per_port[controller], 0, sizeof(stats_per_port[controller]));

        if (was_present) {
            free_controller_pak_name_notes();
            ctr_p_data_loop = false;
        }

        has_mem       = false;
        corrupted_pak = false;
        memset(&cpakfs_stats, 0, sizeof(cpakfs_stats));
        return;
    }

    if (!mounted[controller]) {
        corrupted[controller] = (mount_cpakfs(controller) < 0);
        if (!corrupted[controller]) {
            cpakfs_get_stats(controller, &stats_per_port[controller]);
            mounted[controller] = true;
        } else {
            mounted[controller] = false;
        }
    } else {
        if (!corrupted[controller]) {
            cpakfs_get_stats(controller, &stats_per_port[controller]);
        }
    }

    has_mem       = has_pak[controller];
    corrupted_pak = corrupted[controller];
    cpakfs_stats  = stats_per_port[controller];

    if (!was_present && present) {
        free_controller_pak_name_notes();
        ctr_p_data_loop = false;
    }
}

/**
 * Format the currently selected Controller Pak and reset related state.
 *
 * If formatting fails, sets `failure_message_note` with an error message and
 * sets `error_message_displayed` to true. After attempting format (regardless
 * of success), clears operation flags, unmounts the pak for the selected
 * controller, clears per-port presence/corruption flags, and signals completion
 * by setting `process_complete_format` to true.
 */
static void format_controller_pak () {
    sprintf(failure_message_note, " ");
    int res = cpakfs_format(controller_selected, false);
    if (res < 0) {
        sprintf(failure_message_note, "Unable to format Controller Pak on controller %d!\nError code: %d", controller_selected + 1, res);
        error_message_displayed = true;
    }
    reset_vars();
    cpakfs_unmount(controller_selected);
    mounted[controller_selected] = false;
    has_pak[controller_selected] = false;
    corrupted[controller_selected] = false; 
    process_complete_format = true;
}

/**
 * Request showing the single-note delete confirmation dialog.
 *
 * Sets the internal flag that causes the UI to present a confirmation message
 * for deleting the currently selected note.
 *
 * @param menu Pointer to the current menu context (unused).
 * @param arg  Callback argument (unused).
 */
static void active_single_note_delete_message(menu_t *menu, void *arg) {
    show_single_note_delete_confirm_message = true;
}

/**
 * Show the confirmation dialog to format the currently selected Controller Pak.
 */
static void active_format_controller_pak_message(menu_t *menu, void *arg) {
    show_format_controller_pak_confirm_message = true;
}

/**
 * Display the confirmation dialog to restore a dump to the selected Controller Pak.
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
 * Populate per-slot UI metadata for a Controller Pak note entry.
 *
 * Sets the display name, a formatted bank-size string (a single space if size cannot be determined),
 * and parses the note filename into the cpakfs_path_strings entry for the specified controller slot.
 *
 * @param controller Controller port index (0–3) whose mount path is used to resolve the entry.
 * @param index Array slot index where name, bank-size and parsed path will be written.
 * @param entry_name Filename of the note as reported by the Controller Pak filesystem (relative to the mount).
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
 * Populate the in-memory list of Controller Pak note entries for the currently
 * selected controller port when a pak is present and the list has not yet been loaded.
 *
 * Updates internal per-note arrays (filenames, formatted bank-size strings and
 * parsed path components) for up to MAX_NUM_NOTES entries by enumerating the
 * mounted pak directory for controller_selected. Sets ctr_p_data_loop to true
 * after the list has been populated to avoid repeated re-enumeration.
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
 * Create a timestamped full dump of the Controller Pak attached to the specified port and save it to a CPAK file.
 *
 * Attempts to probe the number of banks, reads each bank from the Controller Pak, and writes the concatenated banks
 * into a timestamped file under CPAK_PATH with the CPAK_EXTENSION. On failure, writes a human-readable message into
 * `failure_message_note` and sets `error_message_displayed`. On success, sets `process_complete_full_dump` to true.
 *
 * @param port Index of the controller port whose Controller Pak will be dumped (typically 0–3).
 */
static void dump_complete_cpak(int port) {
    sprintf(failure_message_note, " ");

    
    int banks = cpak_probe_banks(port);
    if (banks < 1) {
        // Fallback to 1 bank if probing not available; or show error.
        banks = 1;
    }

    get_rtc_time(string_datetime_cpak);
    char complete_filename[200];
    sprintf(complete_filename, "%s/CPAK_%s%s", CPAK_PATH, string_datetime_cpak, CPAK_EXTENSION);

    FILE *fp = fopen(complete_filename, "wb");
    if (!fp) {
        sprintf(failure_message_note, "Failed to open file for writing: %s\n", complete_filename);
        error_message_displayed = true;
        return;
    }

    uint8_t *bankbuf = malloc(MEMPAK_BANK_SIZE);
    if (!bankbuf) {
        sprintf(failure_message_note, "Memory allocation failed!");
        error_message_displayed = true;
        fclose(fp);
        return;
    }

    for (int b = 0; b < banks; ++b) {
        int rd = cpak_read((joypad_port_t)port, (uint8_t)b, 0, bankbuf, MEMPAK_BANK_SIZE);
        if (rd < 0 || rd != MEMPAK_BANK_SIZE) {
            sprintf(failure_message_note, "Failed to read Controller Pak bank %d (err=%d)", b, (rd < 0) ? errno : -1);
            error_message_displayed = true;
            free(bankbuf);
            fclose(fp);
            return;
        }

        size_t wr = fwrite(bankbuf, 1, MEMPAK_BANK_SIZE, fp);
        if (wr != MEMPAK_BANK_SIZE) {
            sprintf(failure_message_note, "Failed to write data to file: %s", complete_filename);
            error_message_displayed = true;
            free(bankbuf);
            fclose(fp);
            return;
        }
    }

    free(bankbuf);
    fclose(fp);
    process_complete_full_dump = true;
}

/**
 * Copy a single note from a Controller Pak to the CPAK notes directory with a timestamped filename.
 *
 * Attempts to open the selected note on the Controller Pak, creates a timestamped dump file under
 * CPAK_NOTES_PATH, and copies the note contents in 4KB chunks. On failure sets `failure_message_note`
 * and `error_message_displayed`. On success sets `process_complete_note_dump`.
 *
 * @param _port Index of the controller port to read from (0-3).
 * @param selected_index Index of the note slot to dump.
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
    ui_components_loader_draw(0, "Saving Controller Pak note...");
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
 * @param filename Path to the file to check.
 * @returns `true` if the file exists and can be opened for reading, `false` otherwise.
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
 * Delete a single note file from the selected Controller Pak and update mount state.
 *
 * Attempts to remove the note at the given slot for the currently selected controller.
 * On failure to find or delete the file, sets `failure_message_note` and `error_message_displayed`.
 * On success, resets internal process state, unmounts the pak for the selected controller, and sets flags to reflect the pak as unmounted and absent.
 *
 * @param _port Unused by this implementation; original caller's controller port index.
 * @param selected_index Zero-based index of the note slot to delete on the selected controller.
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
    cpakfs_unmount(controller_selected);
    mounted[controller_selected] = false;
    has_pak[controller_selected] = false;
    corrupted[controller_selected] = false; 
    unmounted = true;
    process_complete_delete = true;
}

/**
 * Determine if any controller-pak process has completed or an error message is active.
 *
 * @returns `true` if a full dump, single-note dump, format, or delete process has completed,
 *          or if an error message is currently displayed; `false` otherwise.
 */
static bool is_one_of_process_complete() {
    return process_complete_full_dump 
    || process_complete_note_dump 
    || process_complete_format 
    || process_complete_delete
    || error_message_displayed;
}


/**
 * Handle per-frame input, UI state transitions, and initiation of Controller Pak operations for the Controller Pak filesystem view.
 *
 * Processes completion acknowledgements and dismissals, routes context-menu interactions, handles controller navigation and
 * view exit, updates accessory/mount state, populates the on-screen note list, and manages multi-stage confirmation flows
 * for full Pak dumps, single-note dumps, single-note deletion, and Pak formatting. When confirmations are accepted this
 * function sets the corresponding start_* flags (e.g., start_complete_dump, start_single_note_dump, start_single_note_delete,
 * start_format_controller_pak), plays UI sound effects, and updates menu mode/state (for example menu->next_mode).
 *
 * @param menu Current menu context containing input actions and where next_mode is written when changing views.
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
                unmount_all_cpakfs();
                reset_vars();
                for(int i = 0; i < 4; i++){
                    mounted[i] = false;
                    has_pak[i] = false;
                    corrupted[i] = false;
                    memset(&stats_per_port[i], 0, sizeof(stats_per_port[i]));
                }
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
 * Render the Controller Pak management screen onto the given display surface.
 *
 * Draws the full UI for Controller Pak management (status, note list, context menu,
 * action hints, and message/confirmation dialogs) for the currently selected controller.
 * When user-confirmation flags are active and corresponding start_* flags are set,
 * this function will detach the display and begin the requested operation (full dump,
 * single-note dump, single-note delete, or format), which may display a loader or
 * error message and update global process flags.
 *
 * @param menu Current menu state and input context used to determine selection and dialogs.
 * @param d    Surface to render the UI onto.
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
            "A: Backup whole Pak\n"
            "B: Back\n"
        );
        ui_components_actions_bar_text_draw(style,
            ALIGN_RIGHT, VALIGN_TOP,
            "L|Z: Backup a Note\n"
            "R: Options\n"
        );
    } else {
        ui_components_actions_bar_text_draw(style,
            ALIGN_LEFT, VALIGN_TOP,
            "A: Format Controller Pak\n"
            "\n"
        );
    }

    ui_components_actions_bar_text_draw(style,
        ALIGN_CENTER, VALIGN_TOP,
        "\n"
        "< Change Controller >\n"
    );

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
            "Pak saved to:\n"
            "%s\n\n"
            "Press A to continue.",
            CPAK_PATH
        );   
    }

    if (process_complete_note_dump) {
        ui_components_messagebox_draw(
            "Note saved to:\n"
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
            "Do you want to backup the Controller Pak?\n\n"
            "A: Yes        B: No"
        );   
    } else if (show_complete_write_confirm_message) {
        ui_components_messagebox_draw(
            "To write a complete backup, browse to a file"
            " with the extension \".mpk\" or \".pak\".\n\n"
            "B: Back"
        );   
    } 

    if (show_single_note_dump_confirm_message &&
        !start_single_note_dump) {
        ui_components_messagebox_draw(
            "Which note would you like to backup?\n\n"
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
            sprintf(failure_message_note, "No data found on Controller Pak on controller %d!", controller_selected + 1);
            error_message_displayed = true;
            start_complete_dump = false;
            return;

        } else {
            ui_components_loader_draw(0, "Saving Controller Pak...");
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
 * Initialize the Controller Pak filesystem view state and prepare required directories and UI.
 *
 * Resets internal state used by the Controller Pak UI (selection, per-port mount/ corruption flags, stats,
 * operation flags), unmounts any mounted pak filesystems, sets RTC usage based on the provided menu's current time,
 * ensures CPAK dump and notes directories exist, and initializes the view's context menu.
 *
 * @param menu Pointer to the current menu state; `menu->current_time` is used to decide whether RTC-based filenames are available.
 */
void view_controller_pakfs_init (menu_t *menu) {
    ctr_p_data_loop = false;
    controller_selected = 0;
    reset_vars();
    unmount_all_cpakfs();
    unmounted = true;

    for(int i = 0; i < 4; i++){
        mounted[i] = false;
        has_pak[i] = false;
        corrupted[i] = false;
        memset(&stats_per_port[i], 0, sizeof(stats_per_port[i]));
    }

    use_rtc = menu->current_time >= 0 ? true : false;

    create_directory(CPAK_PATH_NO_PRE);
    create_directory(CPAK_NOTES_PATH_NO_PRE);

    ui_components_context_menu_init(&options_context_menu);
}

/**
 * Update Controller Pak view state and render it to the provided surface.
 *
 * Calls the view's processing step to handle input and state transitions, then draws
 * the current view to the given display surface.
 *
 * @param menu The current menu context used for input and view state.
 * @param display The surface to render the view onto.
 */
void view_controller_pakfs_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}