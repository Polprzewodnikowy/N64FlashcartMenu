#include <libdragon.h>

#include "../mp3player.h"
#include "fragments/fragments.h"
#include "views.h"


#define SEEK_SECONDS            10
#define SEEK_UNMUTE_TIMEOUT     17


static int unmute_counter;


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

static void format_elapsed_duration (char *buffer, float elapsed, float duration) {
    strcpy(buffer, " ");

    if (duration >= 3600) {
        sprintf(buffer + strlen(buffer), "%02d:", (int) (elapsed) / 3600);
    }
    sprintf(buffer + strlen(buffer), "%02d:%02d", ((int) (elapsed) % 3600) / 60, (int) (elapsed) % 60);

    strcat(buffer, " / ");

    if (duration >= 3600) {
        sprintf(buffer + strlen(buffer), "%02d:", (int) (duration) / 3600);
    }
    sprintf(buffer + strlen(buffer), "%02d:%02d", ((int) (duration) % 3600) / 60, (int) (duration) % 60);
}


static void process (menu_t *menu) {
    if (unmute_counter > 0) {
        unmute_counter -= 1;
        if (unmute_counter == 0) {
            mp3player_mute(false);
        }
    }

    if (mp3player_process() != MP3PLAYER_OK) {
        menu->next_mode = MENU_MODE_ERROR;
    } else if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
    } else if (menu->actions.enter) {
        if (mp3player_toggle() != MP3PLAYER_OK) {
            menu->next_mode = MENU_MODE_ERROR;
        }
    } else if (menu->actions.go_left || menu->actions.go_right) {
        mp3player_mute(true);
        unmute_counter = SEEK_UNMUTE_TIMEOUT;
        int seconds = (menu->actions.go_left ? -SEEK_SECONDS : SEEK_SECONDS);
        if (mp3player_seek(seconds) != MP3PLAYER_OK) {
            menu->next_mode = MENU_MODE_ERROR;
        }
    }
}

static void draw (menu_t *menu, surface_t *d) {
    char buffer[64];

    layout_t *layout = layout_get();

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

    // Text start
    fragment_text_start(text_color);

    // Main screen
    text_y += fragment_textf(text_x, text_y, "Now playing:");
    format_name(buffer, menu->browser.list[menu->browser.selected].name);
    text_y += fragment_textf(text_x, text_y, buffer);

    text_y += layout->line_height;
    text_y += fragment_textf(text_x, text_y, "Track elapsed / length:");
    format_elapsed_duration(buffer, mp3player_get_duration() * mp3player_get_progress(), mp3player_get_duration());
    text_y += fragment_textf(text_x, text_y, buffer);

    text_y += layout->line_height;
    text_y += fragment_textf(text_x, text_y, "Average bitrate:");
    text_y += fragment_textf(text_x, text_y, " %.0f kbps", mp3player_get_bitrate() / 1000);

    text_y += layout->line_height;
    text_y += fragment_textf(text_x, text_y, "Samplerate:");
    text_y += fragment_textf(text_x, text_y, " %d Hz", mp3player_get_samplerate());

    // Actions bar
    text_y = layout->actions_y + layout->offset_text_y;
    if (mp3player_is_playing()) {
        fragment_textf(text_x, text_y, "A: Pause");
    } else if (mp3player_is_finished()) {
        fragment_textf(text_x, text_y, "A: Play again");
    } else {
        fragment_textf(text_x, text_y, "A: Play");
    }
    text_y += layout->line_height;
    fragment_textf(text_x, text_y, "B: Exit | Left / Right: Rewind / Fast forward");

    rdpq_detach_show();
}


void view_player_init (menu_t *menu) {
    mp3player_err_t error;

    unmute_counter = 0;

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
        mp3player_mute(false);
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
