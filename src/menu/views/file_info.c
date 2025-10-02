#include <sys/stat.h>
#include "../sound.h"

#include "views.h"

static struct stat st;
static bool is_memory_pak_dump;
static bool is_memory_pak_dump_note;

static void process (menu_t *menu) {
    if (is_memory_pak_dump && menu->actions.enter) {
        sound_play_effect(SFX_ENTER);
        menu->next_mode = MENU_MODE_CONTROLLER_PAK_DUMP_INFO;
    } else if (is_memory_pak_dump_note  && menu->actions.enter) {
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

    file_info_t info = {
        S_ISDIR(st.st_mode),
        st.st_mode & S_IWUSR,
        false,
        st.st_mtime,
        st.st_size
    };
    ui_components_file_info_draw(menu->browser.entry->name, &info);

    if (is_memory_pak_dump) {
        ui_components_actions_bar_text_draw(STL_DEFAULT,
            ALIGN_LEFT, VALIGN_TOP,
            "A: Restore to Controller Pak\n"
            "B: Back"
        );
    } else if (is_memory_pak_dump_note) {
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
    is_memory_pak_dump = false;
    is_memory_pak_dump_note = false;
    path_t *path = path_clone_push(menu->browser.directory, menu->browser.entry->name);

    if (stat(path_get(path), &st)) {
        menu_show_error(menu, "Couldn't obtain file information");
    }

    path_free(path);
}

void view_file_info_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}
