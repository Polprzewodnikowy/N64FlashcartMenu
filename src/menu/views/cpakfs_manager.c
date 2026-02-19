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

static void get_rtc_time(char* formatted_time) {
    time_t t = time(NULL);

    struct tm tm = *localtime(&t);

    sprintf(formatted_time, "%04d-%02d-%02d_%02d%02d%02d",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec);
}

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

static void active_single_note_delete_message(menu_t *menu, void *arg) {
    show_single_note_delete_confirm_message = true;
}

static void active_format_controller_pak_message(menu_t *menu, void *arg) {
    show_format_controller_pak_confirm_message = true;
}

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

static void dump_complete_cpak(int port) {
    sprintf(failure_message_note, " ");

    get_rtc_time(string_datetime_cpak);
    char complete_filename[200];
    sprintf(complete_filename, "%s/CPAK_%s%s", CPAK_PATH, string_datetime_cpak, CPAK_EXTENSION);

    cpak_io_context_t ctx;
    cpak_io_err_t err = cpak_backup_to_file(port, complete_filename, &ctx);

    switch (err) {
        case CPAK_IO_OK:
            process_complete_full_dump = true;
            return;
        case CPAK_IO_ERR_NO_PAK:
            sprintf(failure_message_note, "No Controller Pak detected in port %d!", port + 1);
            error_message_displayed = true;
            return;
        case CPAK_IO_ERR_ALLOC:
            sprintf(failure_message_note, "Memory allocation failed!");
            error_message_displayed = true;
            return;
        case CPAK_IO_ERR_FILE_OPEN:
            sprintf(failure_message_note, "Failed to open file for writing: %s\n", complete_filename);
            error_message_displayed = true;
            return;
        case CPAK_IO_ERR_PAK_READ:
            sprintf(failure_message_note, "Failed to read Controller Pak bank %d (err=%d)",
                    ctx.failed_bank, ctx.error_code);
            error_message_displayed = true;
            return;
        case CPAK_IO_ERR_FILE_WRITE:
            sprintf(failure_message_note, "Failed to write data to file: %s", complete_filename);
            error_message_displayed = true;
            return;
        default:
            sprintf(failure_message_note, "Unknown error backing up Controller Pak!");
            error_message_displayed = true;
            return;
    }
}

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
        "▼▲: Select note number",
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

static bool is_one_of_process_complete() {
    return process_complete_full_dump 
    || process_complete_note_dump 
    || process_complete_format 
    || process_complete_delete
    || error_message_displayed;
}


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
                } else if (menu->actions.go_up) {
                    sound_play_effect(SFX_CURSOR);
                    index_selected = dec_index_note(index_selected);
                } else if (menu->actions.go_down) {
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
        "◀ Change Controller ▶\n"
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
            "▼▲: Select note number",
            index_selected + 1
        );
    }

    if (show_single_note_delete_confirm_message &&
        !start_single_note_delete) {
        ui_components_messagebox_draw(
            "Which note would you like to delete?\n\n"
            "Note selected: N.%-2.2d\n\n"
            "A: Select    B: No\n"
            "▼▲: Select note number",
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

void view_controller_pakfs_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}
