/**
 * @file music_player.c
 * @brief Music Player View
 * @ingroup views
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <libdragon.h>

#include "../jpeg_decoder.h"
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
static bool cover_loading_jpeg = false;
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

static const char *cover_image_extensions[] = { "png", "jpg", "jpeg", NULL };

/* Directory saved for fallback use in PNG async callback */
static path_t *cover_dir = NULL;
/* Persistent dir scan state for async continuation across PNG callbacks */
static dir_t cover_dir_entry;
static bool cover_dir_scan_active = false;

/* Forward declaration */
static void try_next_cover_source (void);

static void cover_art_png_callback (png_err_t err, surface_t *decoded_image, void *callback_data) {
    cover_loading = false;
    if (err == PNG_OK && decoded_image) {
        debugf("[cover] PNG decoded OK\n");
        cover_image = decoded_image;
    } else {
        debugf("[cover] PNG decode failed (err=%d), trying next source\n", err);
        try_next_cover_source();
    }
}

static void cover_art_jpeg_callback (jpeg_err_t err, surface_t *decoded_image, void *callback_data) {
    cover_loading = false;
    if (err == JPEG_OK && decoded_image) {
        debugf("[cover] JPEG decoded OK\n");
        cover_image = decoded_image;
    } else {
        debugf("[cover] JPEG decode failed (err=%d), trying next source\n", err);
        try_next_cover_source();
    }
}

/* Compute the largest square decode size affordable given current free heap.
 * Uses 80% of free heap for the RGBA16 surface (2 bytes/pixel).
 * Result is clamped to [COVER_ART_MAX_SIZE, 400]. */
static int cover_art_budget_size (void) {
    heap_stats_t heap;
    sys_get_heap_stats(&heap);
    size_t budget = (size_t)(heap.total - heap.used) * 8 / 10;
    int dim = (int)__builtin_sqrt((double)(budget / 2));
    if (dim < COVER_ART_MAX_SIZE) dim = COVER_ART_MAX_SIZE;
    if (dim > 400)                dim = 400;
    debugf("[cover] heap free=%u KB, budget dim=%d\n",
           (unsigned)((heap.total - heap.used) / 1024), dim);
    return dim;
}

/* Try to load a single image path — starts async decode for both JPEG and PNG.
 * Returns true if loading was initiated successfully. */
static bool try_cover_path (const char *path) {
    if (!path || !path[0]) return false;
    int64_t fsize = file_get_size((char *)path);
    if (fsize <= 0) {
        debugf("[cover] missing or empty: %s\n", path);
        return false;
    }
    int max_size = cover_art_budget_size();
    size_t plen = strlen(path);
    bool is_jpeg = (plen >= 4 && strcmp(path + plen - 4, ".jpg")  == 0) ||
                   (plen >= 5 && strcmp(path + plen - 5, ".jpeg") == 0);
    if (is_jpeg) {
        debugf("[cover] trying JPEG: %s (max %d)\n", path, max_size);
        if (jpeg_decoder_start((char *)path, max_size, max_size,
                               cover_art_jpeg_callback, NULL) == JPEG_OK) {
            cover_loading = true;
            cover_loading_jpeg = true;
            return true;
        }
        return false;
    } else {
        debugf("[cover] trying PNG: %s (max %d)\n", path, max_size);
        if (png_decoder_start((char *)path, max_size, max_size,
                              cover_art_png_callback, NULL) == PNG_OK) {
            cover_loading = true;
            cover_loading_jpeg = false;
            return true;
        }
        return false;
    }
}

/* Scan the directory for the next image file and try to load it.
 * Continues from where the previous call left off (via cover_dir_entry state).
 * Called initially and from the PNG callback on failure. */
static void try_next_cover_source (void) {
    if (!cover_dir || !cover_dir_scan_active) return;

    while (cover_dir_scan_active) {
        if (cover_dir_entry.d_type == DT_REG &&
            file_has_extensions(cover_dir_entry.d_name, cover_image_extensions)) {
            path_t *candidate = path_clone_push(cover_dir, cover_dir_entry.d_name);
            debugf("[cover] candidate: %s\n", path_get(candidate));
            /* Advance scan state now — PNG callback may call us again before we return */
            if (dir_findnext(path_get(cover_dir), &cover_dir_entry) != 0) {
                cover_dir_scan_active = false;
            }
            bool started = try_cover_path(path_get(candidate));
            path_free(candidate);
            if (started) return;
            /* File was empty/corrupt — loop to try the next one */
        } else {
            if (dir_findnext(path_get(cover_dir), &cover_dir_entry) != 0) {
                cover_dir_scan_active = false;
            }
        }
    }

    debugf("[cover] all sources exhausted, no cover art\n");
}

static void load_cover_art (path_t *dir) {
    if (cover_loading) {
        if (cover_loading_jpeg) {
            jpeg_decoder_abort();
        } else {
            png_decoder_abort();
        }
        cover_loading = false;
        cover_loading_jpeg = false;
    }
    if (cover_image) {
        surface_free(cover_image);
        free(cover_image);
        cover_image = NULL;
    }
    if (cover_dir) {
        path_free(cover_dir);
        cover_dir = NULL;
    }
    cover_dir_scan_active = false;

    cover_dir = path_clone(dir);

    const mp3_metadata_t *meta = mp3player_get_metadata();

    /* Try embedded cover art from ID3 APIC tag first */
    if (meta->has_cover_art && meta->cover_art_path[0]) {
        debugf("[cover] has embedded art: %s\n", meta->cover_art_path);
        if (try_cover_path(meta->cover_art_path)) return;
        debugf("[cover] embedded art failed, falling back to directory\n");
    } else {
        debugf("[cover] no embedded art, trying directory\n");
    }

    /* Scan directory for any image file */
    if (dir_findfirst(path_get(cover_dir), &cover_dir_entry) == 0) {
        cover_dir_scan_active = true;
        try_next_cover_source();
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
    } else if ((menu->actions.go_left || menu->actions.go_right) && !seek_inhibit) {
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
        if (cover_loading_jpeg) {
            jpeg_decoder_poll();
        } else {
            png_decoder_poll();
        }
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

    if (cover_image) {
        int iw = cover_image->width;
        int ih = cover_image->height;
        int crop = (iw < ih) ? iw : ih;  /* crop to square using the shorter side */
        int s0 = (iw - crop) / 2;
        int t0 = (ih - crop) / 2;
        /* Center image in the space between title text and the seekbar.
         * text_bottom: below title/artist/album text (~4 lines at ~20px from y=24)
         * bar_top: just above the seekbar
         * Use 16px padding on each side so image floats centered, not pinned. */
        int text_bottom = VISIBLE_AREA_Y0 + 80;
        int bar_top     = SEEKBAR_Y - 8;
        int available   = bar_top - text_bottom;
        int disp_size   = available - 32;  /* 16px padding top and bottom */
        int dst_y = text_bottom + (available - disp_size) / 2;
        int dst_x = DISPLAY_CENTER_X - (disp_size / 2);
        rdpq_mode_push();
        rdpq_set_mode_standard();
        rdpq_mode_filter(FILTER_BILINEAR);
        rdpq_mode_combiner(RDPQ_COMBINER_TEX_FLAT);
        rdpq_mode_blender(0);
        rdpq_tex_blit(cover_image, dst_x, dst_y, &(rdpq_blitparms_t){
            .s0 = s0, .t0 = t0,
            .width = crop, .height = crop,
            .scale_x = (float)disp_size / crop,
            .scale_y = (float)disp_size / crop,
            .filtering = true,
        });
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

    rdpq_text_printn(
        &(rdpq_textparms_t) {
            .style_id = STL_DEFAULT,
            .width = seekbar_bottom_w,
            .align = ALIGN_RIGHT,
        },
        FNT_DEFAULT,
        seekbar_bottom_x, seekbar_bottom_y,
        duration_str, strlen(duration_str)
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
        if (cover_loading_jpeg) {
            jpeg_decoder_abort();
        } else {
            png_decoder_abort();
        }
        cover_loading = false;
        cover_loading_jpeg = false;
    }
    if (cover_image) {
        surface_free(cover_image);
        free(cover_image);
        cover_image = NULL;
    }
    if (cover_dir) {
        path_free(cover_dir);
        cover_dir = NULL;
    }
    cover_dir_scan_active = false;
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
    path_free(path);

    if (err != MP3PLAYER_OK) {
        menu_show_error(menu, convert_error_message(err));
        mp3player_deinit();
        return;
    }

    sound_init_mp3_playback();
    mp3player_mute(false);
    err = mp3player_play();
    if (err != MP3PLAYER_OK) {
        menu_show_error(menu, convert_error_message(err));
        mp3player_deinit();
        return;
    }

    advance_failed = false;
    load_cover_art(menu->browser.directory);
}

void view_music_player_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);

    if (menu->next_mode != MENU_MODE_MUSIC_PLAYER) {
        deinit();
    }
}
