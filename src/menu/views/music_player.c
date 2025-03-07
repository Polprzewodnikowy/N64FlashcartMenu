#include "../mp3_player.h"
#include "../sound.h"
#include "views.h"
#include "../fonts.h"


#define SEEK_SECONDS        (5)
#define SEEK_SECONDS_FAST   (60)


static char *convert_error_message (mp3player_err_t err) {
    switch (err) {
        case MP3PLAYER_ERR_OUT_OF_MEM: return "MP3 player failed due to insufficient memory";
        case MP3PLAYER_ERR_IO: return "I/O error during MP3 playback";
        case MP3PLAYER_ERR_NO_FILE: return "No MP3 file is loaded";
        case MP3PLAYER_ERR_INVALID_FILE: return "Invalid MP3 file";
        default: return "Unknown MP3 player error";
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


static void process (menu_t *menu) {
    mp3player_err_t err;

    err = mp3player_process();
    if (err != MP3PLAYER_OK) {
        menu_show_error(menu, convert_error_message(err));
    } else if (menu->actions.back) {
        sound_play_effect(SFX_EXIT);
        menu->next_mode = MENU_MODE_BROWSER;
    } else if (menu->actions.enter) {
        err = mp3player_toggle();
        if (err != MP3PLAYER_OK) {
            menu_show_error(menu, convert_error_message(err));
        }
        sound_play_effect(SFX_ENTER);
    } else if (menu->actions.go_left || menu->actions.go_right) {
        int seconds = menu->actions.go_fast ? SEEK_SECONDS_FAST : SEEK_SECONDS;
        err = mp3player_seek(menu->actions.go_left ? (-seconds) : seconds);
        if (err != MP3PLAYER_OK) {
            menu_show_error(menu, convert_error_message(err));
        }
    }
}

static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    ui_components_background_draw();

    ui_components_layout_draw();

    ui_components_seekbar_draw(mp3player_get_progress());

    ui_components_main_text_draw(STL_DEFAULT,
        ALIGN_CENTER, VALIGN_TOP,
        "MUSIC PLAYER\n"
        "\n"
        "%s",
        menu->browser.entry->name
    );

    char formatted_track_elapsed_length[64];

    format_elapsed_duration(
        formatted_track_elapsed_length,
        mp3player_get_duration() * mp3player_get_progress(),
        mp3player_get_duration()
    );

    ui_components_main_text_draw(STL_DEFAULT,
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

    ui_components_actions_bar_text_draw(STL_DEFAULT,
        ALIGN_LEFT, VALIGN_TOP,
        "A: %s\n"
        "B: Exit | Left / Right: Rewind / Fast forward",
        mp3player_is_playing() ? "Pause" : mp3player_is_finished() ? "Play again" : "Play"
    );

    rdpq_detach_show();
}

static void deinit (void) {
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
