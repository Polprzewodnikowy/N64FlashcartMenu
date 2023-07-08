#include <fatfs/ff.h>
#include <libdragon.h>

#include "mp3player.h"
#include "utils/utils.h"

#define MINIMP3_IMPLEMENTATION
#include "libs/minimp3/minimp3.h"
#include "libs/minimp3/minimp3_ex.h"


#define BUFFER_SIZE     (16 * 1024)
#define MIXER_CHANNEL   (0)


typedef struct {
    bool loaded;
    bool io_error;

    mp3dec_t dec;
    mp3dec_frame_info_t info;

    FIL fil;
    FSIZE_t data_start;

    uint8_t buffer[BUFFER_SIZE];
    uint8_t *buffer_ptr;
    size_t buffer_left;

    short samples[MINIMP3_MAX_SAMPLES_PER_FRAME];
    short *samples_ptr;
    int samples_left;

    waveform_t wave;
} mp3player_t;

static mp3player_t *p = NULL;


static void mp3player_reset_decoder (void) {
    mp3dec_init(&p->dec);
    p->buffer_ptr = p->buffer;
    p->buffer_left = 0;
    p->samples_ptr = p->samples;
    p->samples_left = 0;
}

static void mp3player_fill_buffer (void) {
    UINT bytes_read;

    if (f_eof(&p->fil)) {
        return;
    }

    if (p->buffer_left >= (MAX_FREE_FORMAT_FRAME_SIZE * 3)) {
        return;
    }

    if ((p->buffer_ptr != p->buffer) && (p->buffer_left > 0)) {
        memmove(p->buffer, p->buffer_ptr, p->buffer_left);
        p->buffer_ptr = p->buffer;
    }

    if (f_read(&p->fil, p->buffer + p->buffer_left, BUFFER_SIZE - p->buffer_left, &bytes_read) == FR_OK) {
        p->buffer_left += bytes_read;
    } else {
        p->io_error = true;
    }
}

static void mp3player_decode_samples (short *buffer, int buffer_samples) {
    if (p->samples_left > 0) {
        int samples_to_copy = MIN(p->samples_left, buffer_samples);

        memcpy(buffer, p->samples_ptr, samples_to_copy * sizeof(short) * p->info.channels);

        p->samples_ptr += samples_to_copy * p->info.channels;
        p->samples_left -= samples_to_copy;

        buffer += samples_to_copy * p->info.channels;
        buffer_samples -= samples_to_copy;
    }

    while (buffer_samples > 0) {
        mp3player_fill_buffer();

        int samples = mp3dec_decode_frame(&p->dec, p->buffer_ptr, p->buffer_left, p->samples, &p->info);

        p->buffer_ptr += p->info.frame_bytes;
        p->buffer_left -= p->info.frame_bytes;

        if (samples > 0) {
            int samples_to_copy = MIN(samples, buffer_samples);

            memcpy(buffer, p->samples, samples_to_copy * sizeof(short) * p->info.channels);

            p->samples_ptr = p->samples + samples_to_copy * p->info.channels;
            p->samples_left = samples - samples_to_copy;

            buffer += samples_to_copy * p->info.channels;
            buffer_samples -= samples_to_copy;
        }

        if (p->info.frame_bytes == 0) {
            memset(buffer, 0, buffer_samples * sizeof(short) * p->info.channels);
            buffer_samples = 0;
        }
    }
}

static void mp3player_wave_read (void *ctx, samplebuffer_t *sbuf, int wpos, int wlen, bool seeking) {
    short *buf = (short *) (samplebuffer_append(sbuf, wlen));
    mp3player_decode_samples(buf, wlen);
}


mp3player_err_t mp3player_init (void) {
    p = calloc(1, sizeof(mp3player_t));

    if (p == NULL) {
        return MP3PLAYER_ERR_MALLOC;
    }

    mp3player_reset_decoder();

    p->loaded = false;
    p->io_error = false;

    p->wave = (waveform_t) {
        .name = "mp3player",
        .bits = 16,
        .channels = 2,
        .frequency = 44100,
        .len = WAVEFORM_MAX_LEN - 1,
        .loop_len = WAVEFORM_MAX_LEN - 1,
        .read = mp3player_wave_read,
        .ctx = p,
    };

    return MP3PLAYER_OK;
}

void mp3player_deinit (void) {
    mp3player_unload();
    free(p);
    p = NULL;
}

mp3player_err_t mp3player_load (char *path) {
    if (p->loaded) {
        mp3player_unload();
    }

    if (f_open(&p->fil, path, FA_READ) != FR_OK) {
        return MP3PLAYER_ERR_IO;
    }

    mp3player_reset_decoder();

    while (!(f_eof(&p->fil) && p->buffer_left == 0)) {
        mp3player_fill_buffer();

        if (p->io_error) {
            return MP3PLAYER_ERR_IO;
        }

        size_t id3v2_skip = mp3dec_skip_id3v2((const uint8_t *) (p->buffer_ptr), p->buffer_left);
        if (id3v2_skip > 0) {
            f_lseek(&p->fil, f_tell(&p->fil) - p->buffer_left + id3v2_skip);
            mp3player_reset_decoder();
            continue;
        }

        if (mp3dec_decode_frame(&p->dec, p->buffer_ptr, p->buffer_left, NULL, &p->info) > 0) {
            mp3dec_init(&p->dec);

            p->loaded = true;
            p->data_start = f_tell(&p->fil) - p->buffer_left + p->info.frame_offset;

            p->wave.channels = p->info.channels;
            p->wave.frequency = p->info.hz;

            return MP3PLAYER_OK;
        }

        p->buffer_ptr += p->info.frame_bytes;
        p->buffer_left -= p->info.frame_bytes;
    }

    if (f_close(&p->fil) != FR_OK) {
        return MP3PLAYER_ERR_IO;
    }

    return MP3PLAYER_ERR_INVALID_FILE;
}

void mp3player_unload (void) {
    mp3player_stop();
    if (p->loaded) {
        p->loaded = false;
        f_close(&p->fil);
    }
}

mp3player_err_t mp3player_process (void) {
    if (p->io_error) {
        mp3player_unload();
        return MP3PLAYER_ERR_IO;
    }

    if (mp3player_is_finished()) {
        mp3player_stop();
    }

    return MP3PLAYER_OK;
}

bool mp3player_is_playing (void) {
    return mixer_ch_playing(MIXER_CHANNEL);
}

bool mp3player_is_finished (void) {
    return f_eof(&p->fil) && p->buffer_left == 0 && p->samples_left == 0;
}

mp3player_err_t mp3player_play (void) {
    if (!p->loaded) {
        return MP3PLAYER_ERR_NO_FILE;
    }
    if (!mp3player_is_playing()) {
        if (mp3player_is_finished()) {
            if (f_lseek(&p->fil, p->data_start) != FR_OK) {
                p->io_error = true;
                return MP3PLAYER_ERR_IO;
            }
            mp3player_reset_decoder();
        }
        mixer_ch_play(MIXER_CHANNEL, &p->wave);
    }
    return MP3PLAYER_OK;
}

void mp3player_stop (void) {
    if (mp3player_is_playing()) {
        mixer_ch_stop(MIXER_CHANNEL);
    }
}

mp3player_err_t mp3player_toggle (void) {
    if (mp3player_is_playing()) {
        mp3player_stop();
    } else {
        return mp3player_play();
    }
    return MP3PLAYER_OK;
}

mp3player_err_t mp3player_seek (int seconds) {
    // NOTE: Rough approximation using last frame bitrate to calculate number of bytes to be skipped.
    //       Good enough but not very accurate for variable bitrate files.

    if (!p->loaded) {
        return MP3PLAYER_ERR_NO_FILE;
    }

    long bytes_to_move = (long) (((p->info.bitrate_kbps * 1024) * seconds) / 8);
    if (bytes_to_move == 0) {
        return MP3PLAYER_OK;
    }

    long position = ((long) (f_tell(&p->fil)) - p->buffer_left + bytes_to_move);
    if (position < (long) (p->data_start)) {
        position = p->data_start;
    }

    if (f_lseek(&p->fil, position) != FR_OK) {
        p->io_error = true;
        return MP3PLAYER_ERR_IO;
    }

    mp3player_reset_decoder();

    return MP3PLAYER_OK;
}

float mp3player_get_progress (void) {
    // NOTE: Rough approximation using file pointer instead of processed samples.
    //       Good enough but not very accurate for variable bitrate files.

    FSIZE_t data_size = f_size(&p->fil) - p->data_start;
    FSIZE_t data_consumed = f_tell(&p->fil) - p->buffer_left;
    FSIZE_t data_position = (data_consumed > p->data_start) ? (data_consumed - p->data_start) : 0;

    return data_position / (float) (data_size);
}
