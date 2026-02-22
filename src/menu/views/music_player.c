/**
 * @file music_player.c
 * @brief Music Player View
 * @ingroup views
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

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
#define COVER_ART_MAX_SIZE      (158)

/* Cover art decoder state — tracks which decoder is active */
typedef enum {
    COVER_IDLE,
    COVER_LOADING_JPEG,
    COVER_LOADING_PNG,
} cover_state_t;

static bool advance_failed = false;
static cover_state_t cover_state = COVER_IDLE;
static surface_t *cover_image = NULL;
static int seek_hold_ticks = 0;
static bool seek_inhibit = false;

/* Cached blit parameters — computed once when cover_image is set */
static int cover_dst_x;
static int cover_dst_y;
static int cover_disp_size;
static int cover_s0;
static int cover_t0;


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

/* Directory saved for fallback use in async callbacks */
static path_t *cover_dir = NULL;
/* Persistent dir scan state for async continuation across callbacks */
static dir_t cover_dir_entry;
static bool cover_dir_scan_active = false;

/* Forward declaration */
static void try_next_cover_source (void);

/* Compute and cache blit parameters for the loaded cover_image.
 * Called once when cover_image is assigned. */
static void cover_cache_blit_params (void) {
    int iw = cover_image->width;
    int ih = cover_image->height;
    cover_s0 = (iw > ih) ? (iw - ih) / 2 : 0;  /* crop to square */
    cover_t0 = (ih > iw) ? (ih - iw) / 2 : 0;
    int text_bottom = VISIBLE_AREA_Y0 + 80;
    int bar_top     = SEEKBAR_Y - 8;
    int available   = bar_top - text_bottom;
    cover_disp_size = available - 32;  /* 16px padding top and bottom */
    cover_dst_y = text_bottom + 16;
    cover_dst_x = DISPLAY_CENTER_X - (cover_disp_size / 2);
}

static void cover_art_png_callback (png_err_t err, surface_t *decoded_image, void *callback_data) {
    cover_state = COVER_IDLE;
    if (err == PNG_OK && decoded_image) {
        cover_image = decoded_image;
        cover_cache_blit_params();
    } else {
        try_next_cover_source();
    }
}

static void cover_art_jpeg_callback (jpeg_err_t err, surface_t *decoded_image, void *callback_data) {
    cover_state = COVER_IDLE;
    if (err == JPEG_OK && decoded_image) {
        cover_image = decoded_image;
        cover_cache_blit_params();
    } else {
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
    int dim = (int)sqrtf((float)(budget / 2));
    if (dim < COVER_ART_MAX_SIZE) dim = COVER_ART_MAX_SIZE;
    if (dim > 400)                dim = 400;
    return dim;
}

/* Try to load a single image path — starts async decode for JPEG or PNG.
 * max_size is passed in (computed once per scan, not per file).
 * Returns true if loading was initiated successfully. */
static bool try_cover_path (const char *path, int max_size) {
    if (!path || !path[0]) return false;
    size_t plen = strlen(path);
    bool is_jpeg = (plen >= 4 && strcmp(path + plen - 4, ".jpg")  == 0) ||
                   (plen >= 5 && strcmp(path + plen - 5, ".jpeg") == 0);
    if (is_jpeg) {
        if (jpeg_decoder_start((char *)path, max_size, max_size,
                               cover_art_jpeg_callback, NULL) == JPEG_OK) {
            cover_state = COVER_LOADING_JPEG;
            return true;
        }
        return false;
    } else {
        if (png_decoder_start((char *)path, max_size, max_size,
                              cover_art_png_callback, NULL) == PNG_OK) {
            cover_state = COVER_LOADING_PNG;
            return true;
        }
        return false;
    }
}

/* Scan the directory for the next image file and try to load it.
 * Continues from where the previous call left off (via cover_dir_entry state).
 * Called initially and from decoder callbacks on failure. */
static void try_next_cover_source (void) {
    if (!cover_dir || !cover_dir_scan_active) return;

    /* Budget is computed once per scan pass, not per candidate file */
    int max_size = cover_art_budget_size();

    while (cover_dir_scan_active) {
        if (cover_dir_entry.d_type == DT_REG &&
            file_has_extensions(cover_dir_entry.d_name, cover_image_extensions)) {
            path_t *candidate = path_clone_push(cover_dir, cover_dir_entry.d_name);
            /* Advance scan state now — callback may call us again before we return */
            if (dir_findnext(path_get(cover_dir), &cover_dir_entry) != 0) {
                cover_dir_scan_active = false;
            }
            bool started = try_cover_path(path_get(candidate), max_size);
            path_free(candidate);
            if (started) return;
            /* File was missing/corrupt — loop to try the next one */
        } else {
            if (dir_findnext(path_get(cover_dir), &cover_dir_entry) != 0) {
                cover_dir_scan_active = false;
            }
        }
    }
}

static void load_cover_art (path_t *dir) {
    if (cover_state == COVER_LOADING_JPEG) {
        jpeg_decoder_abort();
        cover_state = COVER_IDLE;
    } else if (cover_state == COVER_LOADING_PNG) {
        png_decoder_abort();
        cover_state = COVER_IDLE;
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
        int max_size = cover_art_budget_size();
        if (try_cover_path(meta->cover_art_path, max_size)) return;
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
    if (cover_state == COVER_LOADING_PNG) {
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
        int crop = (cover_image->width < cover_image->height)
                   ? cover_image->width : cover_image->height;
        rdpq_mode_push();
        rdpq_set_mode_standard();
        rdpq_mode_filter(FILTER_BILINEAR);
        rdpq_mode_combiner(RDPQ_COMBINER_TEX_FLAT);
        rdpq_mode_blender(0);
        rdpq_tex_blit(cover_image, cover_dst_x, cover_dst_y, &(rdpq_blitparms_t){
            .s0 = cover_s0, .t0 = cover_t0,
            .width = crop, .height = crop,
            .scale_x = (float)cover_disp_size / crop,
            .scale_y = (float)cover_disp_size / crop,
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
        BTN_A " %s\n"
        BTN_B " Exit\n",
        mp3player_is_playing() ? "Pause" : mp3player_is_finished() ? "Play again" : "Play"
    );

    ui_components_actions_bar_text_draw(
        STL_DEFAULT,
        ALIGN_CENTER, VALIGN_TOP,
        BTN_CU BTN_CD " Prev/Next  " BTN_CL BTN_CR " Seek\n"
    );

    rdpq_detach_show();
}

static void deinit (void) {
    if (cover_state == COVER_LOADING_JPEG) {
        jpeg_decoder_abort();
    } else if (cover_state == COVER_LOADING_PNG) {
        png_decoder_abort();
    }
    cover_state = COVER_IDLE;
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
