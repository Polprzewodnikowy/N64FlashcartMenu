#include <stdbool.h>
#include <stdio.h>
#include <libdragon.h>
#include "views.h"
#include "../sound.h"
#include "../fonts.h"
#include <fatfs/ff.h>
#include <errno.h>
#include <dir.h>

#define MAX_NUM_NOTES 16
#define MAX_STRING_LENGTH 62

#define FS_BLOCK_DIVIDER 8*32

static short controller_selected;
static bool has_mem;
static bool corrupted_pak;
static bool unmounted;
static bool ctr_p_data_loop;
static cpakfs_parse_err_t val_err;

static char controller_pak_name_notes[MAX_NUM_NOTES][MAX_STRING_LENGTH];

//static char * CPAK_PATH = "sd:/cpak_saves";
static char * CPAK_MOUNT_ARRAY[4] = {
    "cpak1:/",
    "cpak2:/",
    "cpak3:/",
    "cpak4:/"
};

static cpakfs_stats_t cpakfs_stats;

static dir_t dir_entry;

static int get_block_size_from_fs_path(const char *filename_cpak) {
    FILE *f = fopen(filename_cpak, "rb");

    fseek(f, 0L, SEEK_END);
    int sz = ftell(f);
    fclose(f);

    return sz / (FS_BLOCK_DIVIDER);
}

static void unmount_all_cpakfs() {
    for (int i = 0; i < 4; i++) {
        int val = cpakfs_unmount(i);
        if (val < 0) {
            debugf("Failed to unmount cpakfs on port %d: %d\n", i+1, val);
        } else {
            debugf("Unmounted cpakfs on port %d\n", i+1);
        }
    }
    unmounted = true;
}

static void free_controller_pak_name_notes() {

    // Set \0 to each note
    for (int i = 0; i < MAX_NUM_NOTES; ++i) {
        sprintf(controller_pak_name_notes[i], " ");
    }

}

static void utils_truncate_string(const char *source, char *destination, int new_length) {
    // Copy the first `new_length` characters from `source` to `destination`
    strncpy(destination, source, new_length);
    destination[new_length] = '\0'; // Null-terminate the truncated string
}

static void mount_cpakfs(int controller) {
    corrupted_pak = false;
    if (cpakfs_mount(controller, CPAK_MOUNT_ARRAY[controller]) < 0) {
        debugf("Failed to mount cpakfs on port %d\n", controller + 1);
        // if the cpak is present but can't be mounted, it is probably corrupted
         // (it might be unformatted=
        if (has_mem) {
            corrupted_pak = true;
        }
    } else {
        debugf("Mounted cpakfs on port %d\n", controller + 1);
        cpakfs_get_stats( controller, &cpakfs_stats );
    }
}

static void switch_cpakfs(int controller) {
    unmount_all_cpakfs();
}

static void reset_vars(){
    has_mem = false;
    corrupted_pak = false;
    ctr_p_data_loop = false;
}

static void check_accessories(int controller) {
    
    joypad_accessory_type_t val =  joypad_get_accessory_type(controller);

    has_mem = val == JOYPAD_ACCESSORY_TYPE_CONTROLLER_PAK;

    if (!has_mem && !unmounted) {
        unmount_all_cpakfs();
    } else if (has_mem && unmounted) {
        mount_cpakfs(controller);
        unmounted = false;
    }
}

static void format_controller_pak (menu_t *menu, void *arg) {
    cpakfs_format(controller_selected, false);
    reset_vars();
    unmount_all_cpakfs();
}

static component_context_menu_t options_context_menu = {
    .list = {
        { .text = "Format Contr. Pak", .action = format_controller_pak },
        COMPONENT_CONTEXT_MENU_LIST_END,
    }
};


static void process (menu_t *menu) {
    if (ui_components_context_menu_process(menu, &options_context_menu)) {
        return;
    }

    if(menu->actions.go_left) {
        sound_play_effect(SFX_SETTING);
        controller_selected = ((controller_selected - 1) + 4) % 4;
        reset_vars();
        switch_cpakfs(controller_selected);
    } else if (menu->actions.go_right) {
        sound_play_effect(SFX_SETTING);
        controller_selected = ((controller_selected + 1) + 4) % 4;
        reset_vars();
        switch_cpakfs(controller_selected);
    } else if (menu->actions.back) {
        sound_play_effect(SFX_EXIT);
        menu->next_mode = MENU_MODE_BROWSER;
    } else if (menu->actions.options /* && use_rtc */) {
        ui_components_context_menu_show(&options_context_menu);
        sound_play_effect(SFX_SETTING);
    }

    check_accessories(controller_selected);

    if (has_mem && !ctr_p_data_loop) {
        free_controller_pak_name_notes();
        //TODO: enable actions when a cpak is present
        dir_findfirst(CPAK_MOUNT_ARRAY[controller_selected], &dir_entry);
        char filename_cpak[256];
        sprintf(filename_cpak, "%s%s", CPAK_MOUNT_ARRAY[controller_selected], dir_entry.d_name);
        int size_blocks = get_block_size_from_fs_path(filename_cpak);
        char temp[16];
        utils_truncate_string(dir_entry.d_name, temp, 15);
        snprintf(controller_pak_name_notes[0], MAX_STRING_LENGTH, "%-35.35s (%-3.3d blocks)", temp, size_blocks);
        
        int i = 1;
        while(dir_findnext(CPAK_MOUNT_ARRAY[controller_selected], &dir_entry) == 0) {
            sprintf(filename_cpak, " ");
            sprintf(filename_cpak, "%s%s", CPAK_MOUNT_ARRAY[controller_selected], dir_entry.d_name);
            size_blocks = get_block_size_from_fs_path(filename_cpak);
            char temp2[16];
            utils_truncate_string(dir_entry.d_name, temp2, 15);
            snprintf(controller_pak_name_notes[i], MAX_STRING_LENGTH, "%-35.35s (%-3.3d blocks)", temp2, size_blocks);
        
            i++;
            if (i >= MAX_NUM_NOTES) {
                break;
            }
        ctr_p_data_loop = true;
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

    //TODO: add content here

    if (has_mem) {
        sprintf(has_mem_text, "CPAK detected");
        style = STL_GREEN;

        if (has_mem && !corrupted_pak) {
            sprintf(has_mem_text, "%s %s", has_mem_text, " (is valid)");
            style = STL_GREEN;
            sprintf(free_space_cpak_text, "It has %d/123 free blocks", cpakfs_stats.pages.total - cpakfs_stats.pages.used);
        } else if (has_mem && corrupted_pak) {
            sprintf(has_mem_text, "%s %s", has_mem_text, " (is NOT valid. Corrupted)");
            style = STL_ORANGE;
            sprintf(free_space_cpak_text, " ");
        }
    } else {
        sprintf(has_mem_text, "NO CPAK detected");
        style = STL_ORANGE;
        sprintf(free_space_cpak_text, " ");
    }

    ui_components_actions_bar_text_draw(style,
        ALIGN_LEFT, VALIGN_TOP,
        "A: Dump Pak\n"
        "\n"//"L: Dump single Note\n"
    );
    ui_components_actions_bar_text_draw(STL_GRAY,
        ALIGN_LEFT, VALIGN_TOP,
        "\n"
        "L: Dump single Note\n"
    );
    ui_components_actions_bar_text_draw(style,
        ALIGN_RIGHT, VALIGN_TOP,
        "START: Restore Pak\n"
        "R: Options\n"
    );

    ui_components_main_text_draw(STL_DEFAULT,
        ALIGN_CENTER, VALIGN_TOP,
        "CONTROLLER PAK FS MANAGEMENT (%d)\n", val_err
    );

    ui_components_main_text_draw(STL_DEFAULT,
        ALIGN_RIGHT, VALIGN_TOP,
        "B: Back\n"
    );

    ui_components_main_text_draw(STL_DEFAULT,
        ALIGN_LEFT, VALIGN_TOP,
        "\n"
        "Controller selected: < %d >\n",
            controller_selected + 1
    );

    ui_components_main_text_draw(style,
        ALIGN_LEFT, VALIGN_TOP,
        "\n"
        "                            %s\n",
        has_mem_text
    );

    if (has_mem) {
        ui_components_main_text_draw(STL_DEFAULT,
            ALIGN_CENTER, VALIGN_TOP,
            "\n"
            "\n"
            "%s\n",
            free_space_cpak_text
        );

        ui_components_main_text_draw_specific_font(FNT_JAP, style,
            ALIGN_LEFT, VALIGN_TOP,
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
            //path_cpak[0].filename,
            controller_pak_name_notes[0],
            controller_pak_name_notes[1],
            controller_pak_name_notes[2],
            controller_pak_name_notes[3],
            controller_pak_name_notes[4],
            controller_pak_name_notes[5],
            controller_pak_name_notes[6],
            controller_pak_name_notes[7],
            controller_pak_name_notes[8],
            controller_pak_name_notes[9],
            controller_pak_name_notes[10],
            controller_pak_name_notes[11],
            controller_pak_name_notes[12],
            controller_pak_name_notes[13],
            controller_pak_name_notes[14],
            controller_pak_name_notes[15],
            controller_pak_name_notes[16]
        );
    }

    ui_components_context_menu_draw(&options_context_menu);
        
    rdpq_detach_show();
}








void view_controller_pakfs_init (menu_t *menu) {
    ctr_p_data_loop = false;
    controller_selected = 0;
    reset_vars();
    unmount_all_cpakfs();
/*
    use_rtc = menu->current_time >= 0 ? true : false;

    
    create_directory(CPAK_PATH_NO_PRE);
    create_directory(CPAK_NOTES_PATH_NO_PRE); */

    ui_components_context_menu_init(&options_context_menu);

}

void view_controller_pakfs_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}