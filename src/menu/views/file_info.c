#include <sys/stat.h>
#include "../sound.h"

#include "views.h"

static struct stat st;

static file_info_t info;

static void process (menu_t *menu) {
    if (info.pak_file_attributes.is_controller_pak_dump && menu->actions.enter) {
        sound_play_effect(SFX_ENTER);
        menu->next_mode = MENU_MODE_CONTROLLER_PAK_DUMP_INFO;
    } else if (info.pak_file_attributes.is_controller_pak_dump_note  && menu->actions.enter) {
        sound_play_effect(SFX_ENTER);
        menu->next_mode = MENU_MODE_CONTROLLER_PAK_DUMP_NOTE_INFO;
    } else if (menu->actions.back) {
        sound_play_effect(SFX_EXIT);
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    ui_components_background_draw();

    ui_components_layout_draw();

    ui_components_file_info_draw(menu->browser.entry->name, &info);

    if (info.pak_file_attributes.is_controller_pak_dump) {
        ui_components_actions_bar_text_draw(STL_DEFAULT,
            ALIGN_LEFT, VALIGN_TOP,
            "A: Restore to Controller Pak\n"
            "B: Back"
        );
    } else if (info.pak_file_attributes.is_controller_pak_dump_note) {
        ui_components_actions_bar_text_draw(STL_DEFAULT,
            ALIGN_LEFT, VALIGN_TOP,
            "A: Restore note to Controller Pak\n"
            "B: Back"
        );
    } else {
        ui_components_actions_bar_text_draw(STL_DEFAULT,
            ALIGN_LEFT, VALIGN_TOP,
            "\n"
            "B: Exit"
        );
    }

    rdpq_detach_show();
}


void view_file_info_init (menu_t *menu) {
    path_t *path = path_clone_push(menu->browser.directory, menu->browser.entry->name);

    if (stat(path_get(path), &st)) {
        menu_show_error(menu, "Couldn't obtain file information");
        info = (file_info_t){0};
    } else {
        info = (file_info_t){
            .directory = S_ISDIR(st.st_mode),
            .mtime = st.st_mtime,
            .size = st.st_size,
            .fat_file_attributes = { .is_read_only = FAT_ATTR_IS_RDO(&st), .is_hidden = FAT_ATTR_IS_HID(&st), .is_system = FAT_ATTR_IS_SYS(&st), .is_archive = FAT_ATTR_IS_ARC(&st) },
            .zip_file_attributes = { .writeable = (st.st_mode & S_IWUSR), .encrypted = false, .compressed_size = 0, .crc32 = 0 },
            .pak_file_attributes = { .is_controller_pak_dump = false, .is_controller_pak_dump_note = false }
        };
    }

    path_free(path);
}

void view_file_info_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}
