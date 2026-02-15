#include <string.h>
#include <stdio.h>

#include "../mp3_player.h"
#include "../sound.h"
#include "../path.h"
#include "views.h"


#define SEEK_SECONDS        (5)
#define SEEK_SECONDS_FAST   (60)

static bool advance_failed = false;
static menu_t *current_menu = NULL;


static char *convert_error_message (mp3player_err_t err) {
    switch (err) {
        case MP3PLAYER_ERR_OUT_OF_MEM: return "MP3 player failed due to insufficient memory";
        case MP3PLAYER_ERR_IO: return "I/O error during MP3 playback";
        case MP3PLAYER_ERR_NO_FILE: return "No MP3 file is loaded";
        case MP3PLAYER_ERR_INVALID_FILE: return "Invalid MP3 file";
        default: return "Unknown MP3 player error";
    }
}

static bool try_skip_track (menu_t *menu, int direction) {
    int start = menu->browser.selected;
    int count = menu->browser.entries;
    for (int offset = 1; offset < count; offset++) {
        int idx = (start + direction * offset + count * offset) % count;
        entry_t *e = &menu->browser.list[idx];
        if (e->type != ENTRY_TYPE_MUSIC) continue;

        path_t *path = path_clone_push(menu->browser.directory, e->name);
        mp3player_err_t err = mp3player_load(path_get(path));
        path_free(path);
        if (err != MP3PLAYER_OK) continue;

        err = mp3player_play();
        if (err != MP3PLAYER_OK) continue;

        menu->browser.selected = idx;
        menu->browser.entry = e;

        advance_failed = false;
        return true;
    }
    return false;
}

static void process (menu_t *menu) {
    mp3player_err_t err;

    err = mp3player_process();
    if (err != MP3PLAYER_OK) {
        menu_show_error(menu, convert_error_message(err));
        return;
    }

    /* Auto-advance to next track when current one finishes */
    if (mp3player_is_finished() && !advance_failed) {
        if (!try_skip_track(menu, 1)) {
            advance_failed = true;
        }
    }

    if (menu->actions.back) {
        sound_play_effect(SFX_EXIT);
        menu->next_mode = MENU_MODE_BROWSER;
    } else if (menu->actions.enter) {
        err = mp3player_toggle();
        if (err != MP3PLAYER_OK) {
            menu_show_error(menu, convert_error_message(err));
        }
        sound_play_effect(SFX_ENTER);
    } else if (menu->actions.go_up || menu->actions.go_down) {
        try_skip_track(menu, menu->actions.go_up ? -1 : 1);
    } else if (menu->actions.go_left || menu->actions.go_right) {
        int seconds = menu->actions.go_fast ? SEEK_SECONDS_FAST : SEEK_SECONDS;
        err = mp3player_seek(menu->actions.go_left ? (-seconds) : seconds);
        if (err != MP3PLAYER_OK) {
            menu_show_error(menu, convert_error_message(err));
        }
    }
}

static void format_elapsed_duration (char *buffer, float elapsed, float duration) {
    strcpy(buffer, "");

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

static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    ui_components_background_draw();

    ui_components_layout_draw();

    ui_components_seekbar_draw(mp3player_get_progress());

    const mp3_metadata_t *meta = mp3player_get_metadata();

    if (meta->has_metadata && meta->title[0]) {
        char title_line[128];
        if (meta->track_number > 0) {
            sprintf(title_line, "%d. %s", meta->track_number, meta->title);
        } else {
            strncpy(title_line, meta->title, sizeof(title_line) - 1);
            title_line[sizeof(title_line) - 1] = '\0';
        }
        ui_components_main_text_draw(
            STL_DEFAULT,
            ALIGN_CENTER, VALIGN_TOP,
            "MUSIC PLAYER\n"
            "\n"
            "%s\n"
            "%s%s%s",
            title_line,
            meta->artist[0] ? meta->artist : "",
            (meta->artist[0] && meta->album[0]) ? " - " : "",
            meta->album[0] ? meta->album : ""
        );
    } else {
        ui_components_main_text_draw(
            STL_DEFAULT,
            ALIGN_CENTER, VALIGN_TOP,
            "MUSIC PLAYER\n"
            "\n"
            "%s",
            menu->browser.entry->name
        );
    }

    char formatted_track_elapsed_length[64];

    format_elapsed_duration(
        formatted_track_elapsed_length,
        mp3player_get_duration() * mp3player_get_progress(),
        mp3player_get_duration()
    );

    ui_components_main_text_draw(
        STL_DEFAULT,
        ALIGN_LEFT, VALIGN_TOP,
        "\n"
        "\n"
        "\n"
        "\n"
        " Track elapsed / length:\n"
        "  %s\n"
        "\n"
        " Average bitrate:\n"
        "  %.0f kbps\n"
        "\n"
        " Samplerate:\n"
        "  %d Hz",
        formatted_track_elapsed_length,
        mp3player_get_bitrate() / 1000,
        mp3player_get_samplerate()
    );

    ui_components_actions_bar_text_draw(
        STL_DEFAULT,
        ALIGN_LEFT, VALIGN_TOP,
        "A: %s\n"
        "B: Exit\n",
        mp3player_is_playing() ? "Pause" : mp3player_is_finished() ? "Play again" : "Play"
    );

    ui_components_actions_bar_text_draw(
        STL_DEFAULT,
        ALIGN_CENTER, VALIGN_TOP,
        "◀ Seek ▶  |  ▲ Prev ▼ Next\n"
    );

    rdpq_detach_show();
}

static void deinit (void) {
    current_menu = NULL;
    sound_init_default();
    mp3player_deinit();
}


void view_music_player_init (menu_t *menu) {
    mp3player_err_t err;

    err = mp3player_init();
    if (err != MP3PLAYER_OK) {
        menu_show_error(menu, convert_error_message(err));
        mp3player_deinit();
        return;
    }

    path_t *path = path_clone_push(menu->browser.directory, menu->browser.entry->name);

    err = mp3player_load(path_get(path));
    if (err != MP3PLAYER_OK) {
        menu_show_error(menu, convert_error_message(err));
        mp3player_deinit();
    } else {
        sound_init_mp3_playback();
        mp3player_mute(false);
        err = mp3player_play();
        if (err != MP3PLAYER_OK) {
            menu_show_error(menu, convert_error_message(err));
            mp3player_deinit();
        } else {
            advance_failed = false;
            current_menu = menu;
        }
    }

    path_free(path);
}

void view_music_player_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);

    if (menu->next_mode != MENU_MODE_MUSIC_PLAYER) {
        deinit();
    }
}
