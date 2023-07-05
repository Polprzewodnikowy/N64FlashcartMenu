#include <fatfs/ff.h>
#include <libdragon.h>

#include "mp3player.h"

#define MINIMP3_IMPLEMENTATION
#include "libs/minimp3/minimp3.h"


#define BUFFER_SIZE     8192


typedef struct {
    FIL fil;

    uint8_t buffer[BUFFER_SIZE];
    uint8_t *buffer_ptr;
    int buffer_left;

    short samples[MINIMP3_MAX_SAMPLES_PER_FRAME * 2];
    short *samples_ptr;
    int samples_left;

    bool playing;

    mp3dec_t dec;
    mp3dec_frame_info_t info;
} mp3player_t;

static mp3player_t *p;


static void mp3player_init (void) {
    p = malloc(sizeof(mp3player_t));

    assert(p != NULL);

    p->buffer_ptr = p->buffer;
    p->buffer_left = 0;
    p->samples_ptr = p->samples;
    p->samples_left = 0;
    p->playing = false;

    mp3dec_init(&p->dec);
}

static void mp3player_deinit (void) {
    audio_close();
    f_close(&p->fil);
    free(p);
}

static bool mp3player_fill_buffer (void) {
    UINT bytes_read;

    if (p->buffer_left >= BUFFER_SIZE) {
        return false;
    }

    if ((p->buffer_ptr != p->buffer) && (p->buffer_left > 0)) {
        memmove(p->buffer, p->buffer_ptr, p->buffer_left);
        p->buffer_ptr = p->buffer;
    }

    if (f_read(&p->fil, p->buffer + p->buffer_left, BUFFER_SIZE - p->buffer_left, &bytes_read) != FR_OK) {
        return true;
    }

    p->buffer_left += bytes_read;

    return false;
}

static bool mp3player_push_samples (void) {
    if (!p->playing) {
        while (audio_can_write()) {
            audio_write_silence();
        }
        return false;
    }



    return false;
}


bool mp3player_load (char *path) {
    mp3player_init();

    if (f_open(&p->fil, path, FA_READ) != FR_OK) {
        mp3player_deinit();
        return true;
    }

    while (!f_eof(&p->fil)) {
        if (mp3player_fill_buffer()) {
            break;
        }
        if (mp3dec_decode_frame(&p->dec, p->buffer_ptr, p->buffer_left, NULL, &p->info) > 0) {
            audio_init(p->info.hz, 4);
            audio_write_silence();
            return false;
        } else if (p->info.frame_bytes > 0) {
            p->buffer_ptr += p->info.frame_bytes;
            p->buffer_left -= p->info.frame_bytes;
        } else {
            p->buffer_ptr = p->buffer + p->buffer_left;
            p->buffer_left = 0;
        }
    }

    mp3player_deinit();
    return true;
}

void mp3player_unload (void) {
    mp3player_deinit();
}

void mp3player_start (void) {
    p->playing = true;
}

bool mp3player_process (void) {
    return mp3player_push_samples();
}
