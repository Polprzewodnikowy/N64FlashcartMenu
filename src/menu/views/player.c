#include <libdragon.h>

#include "fragments/fragments.h"
#include "views.h"

#include "../mp3player.h"


static void format_name (char *buffer, char *name) {
    int cutoff_length = 57;
    int name_length = strlen(name);
    strcpy(buffer, " ");
    if (name_length > cutoff_length) {
        strncat(buffer, name, cutoff_length - 1);
        strcat(buffer, "…");
    } else {
        strcat(buffer, name);
    }
}


static void process (menu_t *menu) {
    if (mp3player_process() != MP3PLAYER_OK) {
        menu->next_mode = MENU_MODE_ERROR;
    } else if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
    } else if (menu->actions.enter) {
        if (mp3player_toggle() != MP3PLAYER_OK) {
            menu->next_mode = MENU_MODE_ERROR;
        }
    } else if (menu->actions.go_left || menu->actions.go_right) {
        int seconds = ((menu->actions.go_left ? -1 : 1) * 10);
        if (mp3player_seek(seconds) != MP3PLAYER_OK) {
            menu->next_mode = MENU_MODE_ERROR;
        }
    }
}

static void draw (menu_t *menu, surface_t *d) {
    char buffer[64];

    layout_t *layout = get_layout();

    const int text_x = layout->offset_x + layout->offset_text_x;
    int text_y = layout->offset_y + layout->offset_text_y;

    const color_t bg_color = RGBA32(0x00, 0x00, 0x00, 0xFF);
    const color_t text_color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF);

    rdpq_attach(d, NULL);
    rdpq_clear(bg_color);

    // Layout
    fragment_borders(d);

    // Progressbar
    fragment_progressbar(d, mp3player_get_progress());

    // Main screen
    rdpq_font_begin(text_color);
    rdpq_font_position(text_x, text_y + menu->assets.font_height);
    rdpq_font_print(menu->assets.font, "Now playing:");
    text_y += layout->line_height;
    rdpq_font_position(text_x, text_y + menu->assets.font_height);
    format_name(buffer, menu->browser.list[menu->browser.selected].name);
    rdpq_font_print(menu->assets.font, buffer);

    // Actions bar
    text_y = layout->actions_y + layout->offset_text_y;
    rdpq_font_position(text_x, text_y + menu->assets.font_height);
    if (mp3player_is_playing()) {
        rdpq_font_print(menu->assets.font, "A: Pause");
    } else if (mp3player_is_finished()) {
        rdpq_font_print(menu->assets.font, "A: Play again");
    } else {
        rdpq_font_print(menu->assets.font, "A: Play");
    }
    text_y += layout->line_height;
    rdpq_font_position(text_x, text_y + menu->assets.font_height);
    rdpq_font_print(menu->assets.font, "B: Exit | Left/Right: Rewind/Fast forward");
    rdpq_font_end();

    rdpq_detach_show();
}


void view_player_init (menu_t *menu) {
    mp3player_err_t error;

    error = mp3player_init();
    if (error != MP3PLAYER_OK) {
        menu->next_mode = MENU_MODE_ERROR;
        mp3player_deinit();
        return;
    }

    path_t *path = path_clone(menu->browser.directory);
    path_push(path, menu->browser.list[menu->browser.selected].name);

    error = mp3player_load(path_get(path));
    if (error != MP3PLAYER_OK) {
        menu->next_mode = MENU_MODE_ERROR;
        mp3player_deinit();
    } else {
        mp3player_play();
    }

    path_free(path);
}

void view_player_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
    if (menu->next_mode != MENU_MODE_PLAYER) {
        mp3player_deinit();
    }
}
