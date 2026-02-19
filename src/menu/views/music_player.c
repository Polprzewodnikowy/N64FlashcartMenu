/**
 * @file music_player.c
 * @brief Music Player View
 * @ingroup views
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../mp3_player.h"
#include "../png_decoder.h"
#include "../sound.h"
#include "../path.h"
#include "../fonts.h"
#include "../ui_components/constants.h"
#include "utils/fs.h"
#include "views.h"


#define SEEK_SECONDS_MIN        (2)
#define SEEK_SECONDS_MAX        (60)
#define SEEK_RAMP_TICKS         (120)   /* ticks held before reaching max seek rate (~2s at 60fps) */
#define SEEK_MAX_DURATION_FRAC  (0.10f) /* max seek = 10% of song duration, capped at SEEK_SECONDS_MAX */
#define COVER_ART_MAX_SIZE  (158)
#define COVER_ART_Y         (180)

static bool advance_failed = false;
static bool cover_loading = false;
static surface_t *cover_image = NULL;
static int seek_hold_ticks = 0;
static bool seek_inhibit = false;


static char *convert_error_message (mp3player_err_t err) {
    switch (err) {
        case MP3PLAYER_ERR_OUT_OF_MEM: return "MP3 player failed due to insufficient memory";
        case MP3PLAYER_ERR_IO: return "I/O error during MP3 playback";
        case MP3PLAYER_ERR_NO_FILE: return "No MP3 file is loaded";
        case MP3PLAYER_ERR_INVALID_FILE: return "Invalid MP3 file";
        default: return "Unknown MP3 player error";
    }
}

static void cover_art_callback (png_err_t err, surface_t *decoded_image, void *callback_data) {
    cover_loading = false;
    if (err == PNG_OK) {
        cover_image = decoded_image;
    }
}

static void load_cover_art (path_t *dir) {
    if (cover_loading) {
        png_decoder_abort();
        cover_loading = false;
    }
    if (cover_image) {
        surface_free(cover_image);
        free(cover_image);
        cover_image = NULL;
    }

    /* Try embedded PNG cover art from ID3 APIC tag first */
    const mp3_metadata_t *meta = mp3player_get_metadata();
    if (meta->has_cover_art && meta->cover_art_path[0]) {
        if (png_decoder_start((char *)meta->cover_art_path, COVER_ART_MAX_SIZE, COVER_ART_MAX_SIZE,
                               cover_art_callback, NULL) == PNG_OK) {
            cover_loading = true;
            return;
        }
    }

    /* Fall back to a PNG file in the same directory as the MP3.
     * Only PNG is supported — JPEG cover art is not decoded. */
    static const char *cover_filenames[] = {
        "cover.png", "folder.png", "album.png", "front.png", NULL
    };
    for (const char **name = cover_filenames; *name != NULL; name++) {
        path_t *candidate = path_clone_push(dir, (char *)*name);
        if (file_exists(path_get(candidate))) {
            if (png_decoder_start(path_get(candidate), COVER_ART_MAX_SIZE, COVER_ART_MAX_SIZE,
                                   cover_art_callback, NULL) == PNG_OK) {
                cover_loading = true;
                path_free(candidate);
                return;
            }
        }
        path_free(candidate);
    }
}

static bool try_skip_track (menu_t *menu, int direction) {
    int start = menu->browser.selected;
    int count = menu->browser.entries;
    for (int offset = 1; offset < count; offset++) {
        /* Wrap index in both directions: +count*offset keeps modulo positive for direction=-1 */
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
        seek_hold_ticks = 0;
        seek_inhibit = true;
        load_cover_art(menu->browser.directory);
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
        if (seek_inhibit) {
            /* Swallow input until user releases seek after a track change */
        } else {
            if (seek_hold_ticks < SEEK_RAMP_TICKS) {
                seek_hold_ticks++;
            }
            float duration = mp3player_get_duration();
            float seek_max = duration * SEEK_MAX_DURATION_FRAC;
            if (seek_max > SEEK_SECONDS_MAX) { seek_max = SEEK_SECONDS_MAX; }
            if (seek_max < SEEK_SECONDS_MIN) { seek_max = SEEK_SECONDS_MIN; }
            float t = seek_hold_ticks / (float) SEEK_RAMP_TICKS;
            int seconds = (int) (SEEK_SECONDS_MIN + t * (seek_max - SEEK_SECONDS_MIN));
            err = mp3player_seek(menu->actions.go_left ? (-seconds) : seconds);
            if (err != MP3PLAYER_OK) {
                menu_show_error(menu, convert_error_message(err));
            }
        }
    } else {
        seek_hold_ticks = 0;
        seek_inhibit = false;
    }
}

static void format_time (char *buffer, float seconds) {
    int s = (int) seconds;
    if (s >= 3600) {
        sprintf(buffer, "%02d:%02d:%02d", s / 3600, (s % 3600) / 60, s % 60);
    } else {
        sprintf(buffer, "%02d:%02d", s / 60, s % 60);
    }
}

static void draw (menu_t *menu, surface_t *d) {
    if (cover_loading) {
        png_decoder_poll();
    }

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
            "%s%s%s\n"
            "\n"
            " %.0f kbps  |  %d Hz",
            title_line,
            meta->artist[0] ? meta->artist : "",
            (meta->artist[0] && meta->album[0]) ? " - " : "",
            meta->album[0] ? meta->album : "",
            mp3player_get_bitrate() / 1000,
            mp3player_get_samplerate()
        );
    } else {
        ui_components_main_text_draw(
            STL_DEFAULT,
            ALIGN_CENTER, VALIGN_TOP,
            "MUSIC PLAYER\n"
            "\n"
            "%s\n"
            "\n"
            "\n"
            " %.0f kbps  |  %d Hz",
            menu->browser.entry->name,
            mp3player_get_bitrate() / 1000,
            mp3player_get_samplerate()
        );
    }

    if (cover_image) {
        int cx = DISPLAY_CENTER_X - (cover_image->width / 2);
        int cy = COVER_ART_Y;
        rdpq_mode_push();
        rdpq_set_mode_copy(false);
        rdpq_tex_blit(cover_image, cx, cy, NULL);
        rdpq_mode_pop();
    }

    char elapsed_str[16];
    char duration_str[16];
    float duration = mp3player_get_duration();
    format_time(elapsed_str, duration * mp3player_get_progress());
    format_time(duration_str, duration);

    int seekbar_bottom_y = SEEKBAR_Y + 18;
    int seekbar_bottom_x = SEEKBAR_X + 4;
    int seekbar_bottom_w = SEEKBAR_WIDTH - 4;

    rdpq_text_printn(
        &(rdpq_textparms_t) { .style_id = STL_DEFAULT },
        FNT_DEFAULT,
        seekbar_bottom_x, seekbar_bottom_y,
        elapsed_str, strlen(elapsed_str)
    );

    int dur_len = strlen(duration_str);
    rdpq_text_printn(
        &(rdpq_textparms_t) {
            .style_id = STL_DEFAULT,
            .width = seekbar_bottom_w,
            .align = ALIGN_RIGHT,
        },
        FNT_DEFAULT,
        seekbar_bottom_x, seekbar_bottom_y,
        duration_str, dur_len
    );

    char tech_str[32];
    snprintf(tech_str, sizeof(tech_str), "%.0f kbps  |  %d Hz",
             mp3player_get_bitrate() / 1000, mp3player_get_samplerate());
    rdpq_text_printn(
        &(rdpq_textparms_t) {
            .style_id = STL_DEFAULT,
            .width = seekbar_bottom_w,
            .align = ALIGN_CENTER,
        },
        FNT_DEFAULT,
        seekbar_bottom_x, seekbar_bottom_y,
        tech_str, strlen(tech_str)
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
        "^06▲▼^00 ^03▲▼^00: Prev/Next  ^06◀▶^00 ^03◀▶^00: Seek\n"
    );

    rdpq_detach_show();
}

static void deinit (void) {
    if (cover_loading) {
        png_decoder_abort();
        cover_loading = false;
    }
    if (cover_image) {
        surface_free(cover_image);
        free(cover_image);
        cover_image = NULL;
    }
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
            load_cover_art(menu->browser.directory);
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
