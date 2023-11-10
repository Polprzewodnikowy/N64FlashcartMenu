#include <fatfs/ff.h>
#include <libdragon.h>

#include "mp3_player.h"
#include "sound.h"
#include "utils/fs.h"
#include "utils/utils.h"

#define MINIMP3_IMPLEMENTATION
#define MINIMP3_ONLY_MP3
#include <minimp3/minimp3_ex.h>
#include <minimp3/minimp3.h>


#define SEEK_PREDECODE_FRAMES   (5)


/** @brief MP3 File Information Structure. */
typedef struct {
    bool loaded;
    bool io_error;

    FIL fil;
    FSIZE_t data_start;
    int seek_predecode_frames;

    mp3dec_t dec;
    mp3dec_frame_info_t info;

    uint8_t buffer[MAX_FREE_FORMAT_FRAME_SIZE];
    uint8_t *buffer_ptr;
    size_t buffer_left;

    float duration;
    float bitrate;

    waveform_t wave;
} mp3player_t;

static mp3player_t *p = NULL;


static void mp3player_reset_decoder (void) {
    mp3dec_init(&p->dec);
    p->seek_predecode_frames = 0;
    p->buffer_ptr = p->buffer;
    p->buffer_left = 0;
}

static void mp3player_fill_buffer (void) {
    UINT bytes_read;

    if (f_eof(&p->fil)) {
        return;
    }

    if ((p->buffer_ptr != p->buffer) && (p->buffer_left > 0)) {
        memmove(p->buffer, p->buffer_ptr, p->buffer_left);
        p->buffer_ptr = p->buffer;
    }

    if (f_read(&p->fil, p->buffer + p->buffer_left, sizeof(p->buffer) - p->buffer_left, &bytes_read) == FR_OK) {
        p->buffer_left += bytes_read;
    } else {
        p->io_error = true;
    }
}

static void mp3player_wave_read (void *ctx, samplebuffer_t *sbuf, int wpos, int wlen, bool seeking) {
    while (wlen > 0) {
        mp3player_fill_buffer();

        int samples = mp3dec_decode_frame(&p->dec, p->buffer_ptr, p->buffer_left, NULL, &p->info);

        if (samples > 0) {
            short *buffer = (short *) (samplebuffer_append(sbuf, samples));

            p->buffer_ptr += p->info.frame_offset;
            p->buffer_left -= p->info.frame_offset;

            mp3dec_decode_frame(&p->dec, p->buffer_ptr, p->buffer_left, buffer, &p->info);

            if (p->seek_predecode_frames > 0) {
                p->seek_predecode_frames -= 1;
                memset(buffer, 0, samples * sizeof(short) * p->info.channels);
            }

            wlen -= samples;
        }

        p->buffer_ptr += p->info.frame_bytes;
        p->buffer_left -= p->info.frame_bytes;

        if (p->info.frame_bytes == 0) {
            short *buffer = (short *) (samplebuffer_append(sbuf, wlen));

            memset(buffer, 0, wlen * sizeof(short) * p->info.channels);

            wlen = 0;
        }
    }
}

static void mp3player_calculate_duration (int samples) {
    uint32_t frames;
    int delay, padding;

    long data_size = (f_size(&p->fil) - p->data_start);
    if (mp3dec_check_vbrtag((const uint8_t *) (p->buffer_ptr), p->info.frame_bytes, &frames, &delay, &padding) > 0) {
        p->duration = (frames * samples) / (float) (p->info.hz);
        p->bitrate = (data_size * 8) / p->duration;
    } else {
        p->bitrate = p->info.bitrate_kbps * 1000;
        p->duration = data_size / (p->bitrate / 8);
    }
}


void mp3player_mixer_init (void) {
    // NOTE: Deliberately setting max_frequency to twice of actual maximum samplerate of mp3 file.
    //       It's tricking mixer into creating buffer long enough for appending data created by mp3dec_decode_frame.

    mixer_ch_set_limits(SOUND_MP3_PLAYER_CHANNEL, 16, 96000, 0);
}

mp3player_err_t mp3player_init (void) {
    p = calloc(1, sizeof(mp3player_t));

    if (p == NULL) {
        return MP3PLAYER_ERR_OUT_OF_MEM;
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

    if (f_open(&p->fil, strip_sd_prefix(path), FA_READ) != FR_OK) {
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
            if (f_lseek(&p->fil, f_tell(&p->fil) - p->buffer_left + id3v2_skip) != FR_OK) {
                return MP3PLAYER_ERR_IO;
            }
            mp3player_reset_decoder();
            continue;
        }

        int samples = mp3dec_decode_frame(&p->dec, p->buffer_ptr, p->buffer_left, NULL, &p->info);
        if (samples > 0) {
            p->loaded = true;
            p->data_start = f_tell(&p->fil) - p->buffer_left + p->info.frame_offset;

            p->buffer_ptr += p->info.frame_offset;
            p->buffer_left -= p->info.frame_offset;

            p->wave.channels = p->info.channels;
            p->wave.frequency = p->info.hz;

            mp3player_calculate_duration(samples);

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
    return mixer_ch_playing(SOUND_MP3_PLAYER_CHANNEL);
}

bool mp3player_is_finished (void) {
    return p->loaded && f_eof(&p->fil) && (p->buffer_left == 0);
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
        mixer_ch_play(SOUND_MP3_PLAYER_CHANNEL, &p->wave);
    }
    return MP3PLAYER_OK;
}

void mp3player_stop (void) {
    if (mp3player_is_playing()) {
        mixer_ch_stop(SOUND_MP3_PLAYER_CHANNEL);
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

void mp3player_mute (bool mute) {
    float volume = mute ? 0.0f : 1.0f;
    mixer_ch_set_vol(SOUND_MP3_PLAYER_CHANNEL, volume, volume);
}

mp3player_err_t mp3player_seek (int seconds) {
    // NOTE: Rough approximation using average bitrate to calculate number of bytes to be skipped.
    //       Good enough but not very accurate for variable bitrate files.

    if (!p->loaded) {
        return MP3PLAYER_ERR_NO_FILE;
    }

    long bytes_to_move = (long) ((p->bitrate * seconds) / 8);
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
    mp3player_fill_buffer();

    p->seek_predecode_frames = (position == p->data_start) ? 0 : SEEK_PREDECODE_FRAMES;

    if (p->io_error) {
        return MP3PLAYER_ERR_IO;
    }

    return MP3PLAYER_OK;
}

float mp3player_get_duration (void) {
    if (!p->loaded) {
        return 0.0f;
    }

    return p->duration;
}

float mp3player_get_bitrate (void) {
    if (!p->loaded) {
        return 0.0f;
    }

    return p->bitrate;
}

int mp3player_get_samplerate (void) {
    if (!p->loaded) {
        return 0;
    }

    return p->info.hz;
}

float mp3player_get_progress (void) {
    // NOTE: Rough approximation using file pointer instead of processed samples.
    //       Good enough but not very accurate for variable bitrate files.

    if (!p->loaded) {
        return 0.0f;
    }

    FSIZE_t data_size = f_size(&p->fil) - p->data_start;
    FSIZE_t data_consumed = f_tell(&p->fil) - p->buffer_left;
    FSIZE_t data_position = (data_consumed > p->data_start) ? (data_consumed - p->data_start) : 0;

    return data_position / (float) (data_size);
}
