/**
 * @file mp3_player.c
 * @brief MP3 Player component implementation
 * @ingroup ui_components
 */

#include <stdio.h>
#include <sys/stat.h>

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
    bool loaded; /**< Indicates if the MP3 file is loaded */

    FILE *f; /**< File pointer */
    size_t file_size; /**< Size of the file */
    size_t data_start; /**< Start position of the data */
    uint8_t buffer[16 * 1024]; /**< Buffer for reading data */
    uint8_t *buffer_ptr; /**< Pointer to the current position in the buffer */
    size_t buffer_left; /**< Amount of data left in the buffer */

    mp3dec_t dec; /**< MP3 decoder */
    mp3dec_frame_info_t info; /**< MP3 frame information */

    int seek_predecode_frames; /**< Number of frames to pre-decode when seeking */
    float duration; /**< Duration of the MP3 file */
    float bitrate; /**< Bitrate of the MP3 file */

    waveform_t wave; /**< Waveform structure for playback */
} mp3player_t;

static mp3player_t *p = NULL;

/**
 * @brief Reset the MP3 decoder.
 */
static void mp3player_reset_decoder (void) {
    mp3dec_init(&p->dec);
    p->seek_predecode_frames = 0;
    p->buffer_ptr = p->buffer;
    p->buffer_left = 0;
}

/**
 * @brief Fill the buffer with data from the MP3 file.
 */
static void mp3player_fill_buffer (void) {
    if (feof(p->f)) {
        return;
    }

    if (p->buffer_left >= ALIGN(MAX_FREE_FORMAT_FRAME_SIZE, FS_SECTOR_SIZE)) {
        return;
    }

    if ((p->buffer_ptr != p->buffer) && (p->buffer_left > 0)) {
        memmove(p->buffer, p->buffer_ptr, p->buffer_left);
        p->buffer_ptr = p->buffer;
    }

    p->buffer_left += fread(p->buffer + p->buffer_left, 1, sizeof(p->buffer) - p->buffer_left, p->f);
}

/**
 * @brief Read waveform data for playback.
 * 
 * @param ctx Context pointer.
 * @param sbuf Sample buffer.
 * @param wpos Write position.
 * @param wlen Write length.
 * @param seeking Indicates if seeking is in progress.
 */
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

/**
 * @brief Calculate the duration of the MP3 file.
 * 
 * @param samples Number of samples.
 */
static void mp3player_calculate_duration (int samples) {
    uint32_t frames;
    int delay, padding;

    long data_size = (p->file_size - p->data_start);
    if (mp3dec_check_vbrtag((const uint8_t *) (p->buffer_ptr), p->info.frame_bytes, &frames, &delay, &padding) > 0) {
        p->duration = (frames * samples) / (float) (p->info.hz);
        p->bitrate = (data_size * 8) / p->duration;
    } else {
        p->bitrate = p->info.bitrate_kbps * 1000;
        p->duration = data_size / (p->bitrate / 8);
    }
}

/**
 * @brief Initialize the MP3 player mixer.
 */
void mp3player_mixer_init (void) {
    // NOTE: Deliberately setting max_frequency to twice of actual maximum samplerate of mp3 file.
    //       It's tricking mixer into creating buffer long enough for appending data created by mp3dec_decode_frame.

    mixer_ch_set_limits(SOUND_MP3_PLAYER_CHANNEL, 16, 96000, 0);
}

/**
 * @brief Initialize the MP3 player.
 * 
 * @return mp3player_err_t Error code.
 */
mp3player_err_t mp3player_init (void) {
    p = calloc(1, sizeof(mp3player_t));

    if (p == NULL) {
        return MP3PLAYER_ERR_OUT_OF_MEM;
    }

    mp3player_reset_decoder();

    p->loaded = false;

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

/**
 * @brief Deinitialize the MP3 player.
 */
void mp3player_deinit (void) {
    mp3player_unload();
    free(p);
    p = NULL;
}

/**
 * @brief Load an MP3 file.
 * 
 * @param path Path to the MP3 file.
 * @return mp3player_err_t Error code.
 */
mp3player_err_t mp3player_load (char *path) {
    if (p->loaded) {
        mp3player_unload();
    }

    if ((p->f = fopen(path, "rb")) == NULL) {
        return MP3PLAYER_ERR_IO;
    }
    setbuf(p->f, NULL);

    struct stat st;
    if (fstat(fileno(p->f), &st)) {
        fclose(p->f);
        return MP3PLAYER_ERR_IO;
    }
    p->file_size = st.st_size;

    mp3player_reset_decoder();

    while (!(feof(p->f) && p->buffer_left == 0)) {
        mp3player_fill_buffer();

        if (ferror(p->f)) {
            fclose(p->f);
            return MP3PLAYER_ERR_IO;
        }

        size_t id3v2_skip = mp3dec_skip_id3v2((const uint8_t *) (p->buffer_ptr), p->buffer_left);
        if (id3v2_skip > 0) {
            if (fseek(p->f, (-p->buffer_left) + id3v2_skip, SEEK_CUR)) {
                fclose(p->f);
                return MP3PLAYER_ERR_IO;
            }
            mp3player_reset_decoder();
            continue;
        }

        int samples = mp3dec_decode_frame(&p->dec, p->buffer_ptr, p->buffer_left, NULL, &p->info);
        if (samples > 0) {
            p->loaded = true;
            p->data_start = ftell(p->f) - p->buffer_left + p->info.frame_offset;

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

    if (fclose(p->f)) {
        return MP3PLAYER_ERR_IO;
    }

    return MP3PLAYER_ERR_INVALID_FILE;
}

/**
 * @brief Unload the MP3 file.
 */
void mp3player_unload (void) {
    mp3player_stop();
    if (p->loaded) {
        p->loaded = false;
        fclose(p->f);
    }
}

/**
 * @brief Process the MP3 player.
 * 
 * @return mp3player_err_t Error code.
 */
mp3player_err_t mp3player_process (void) {
    if (ferror(p->f)) {
        mp3player_unload();
        return MP3PLAYER_ERR_IO;
    }

    if (mp3player_is_finished()) {
        mp3player_stop();
    }

    return MP3PLAYER_OK;
}

/**
 * @brief Check if the MP3 player is playing.
 * 
 * @return true if playing, false otherwise.
 */
bool mp3player_is_playing (void) {
    return mixer_ch_playing(SOUND_MP3_PLAYER_CHANNEL);
}

/**
 * @brief Check if the MP3 player has finished playing.
 * 
 * @return true if finished, false otherwise.
 */
bool mp3player_is_finished (void) {
    return p->loaded && feof(p->f) && (p->buffer_left == 0);
}

/**
 * @brief Play the MP3 file.
 * 
 * @return mp3player_err_t Error code.
 */
mp3player_err_t mp3player_play (void) {
    if (!p->loaded) {
        return MP3PLAYER_ERR_NO_FILE;
    }
    if (!mp3player_is_playing()) {
        if (mp3player_is_finished()) {
            if (fseek(p->f, p->data_start, SEEK_SET)) {
                return MP3PLAYER_ERR_IO;
            }
            mp3player_reset_decoder();
        }
        mixer_ch_play(SOUND_MP3_PLAYER_CHANNEL, &p->wave);
    }
    return MP3PLAYER_OK;
}

/**
 * @brief Stop the MP3 player.
 */
void mp3player_stop (void) {
    if (mp3player_is_playing()) {
        mixer_ch_stop(SOUND_MP3_PLAYER_CHANNEL);
    }
}

/**
 * @brief Toggle the MP3 player between play and stop.
 * 
 * @return mp3player_err_t Error code.
 */
mp3player_err_t mp3player_toggle (void) {
    if (mp3player_is_playing()) {
        mp3player_stop();
    } else {
        return mp3player_play();
    }
    return MP3PLAYER_OK;
}

/**
 * @brief Mute or unmute the MP3 player.
 * 
 * @param mute True to mute, false to unmute.
 */
void mp3player_mute (bool mute) {
    float volume = mute ? 0.0f : 1.0f;
    mixer_ch_set_vol(SOUND_MP3_PLAYER_CHANNEL, volume, volume);
}

/**
 * @brief Seek to a specific position in the MP3 file.
 * 
 * @param seconds Number of seconds to seek.
 * @return mp3player_err_t Error code.
 */
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

    long position = (ftell(p->f) - p->buffer_left + bytes_to_move);
    if (position < (long) (p->data_start)) {
        position = p->data_start;
    }

    if (fseek(p->f, position, SEEK_SET)) {
        return MP3PLAYER_ERR_IO;
    }

    mp3player_reset_decoder();
    mp3player_fill_buffer();

    if (ferror(p->f)) {
        return MP3PLAYER_ERR_IO;
    }

    p->seek_predecode_frames = (position == p->data_start) ? 0 : SEEK_PREDECODE_FRAMES;

    return MP3PLAYER_OK;
}

/**
 * @brief Get the duration of the MP3 file.
 * 
 * @return float Duration in seconds.
 */
float mp3player_get_duration (void) {
    if (!p->loaded) {
        return 0.0f;
    }

    return p->duration;
}

/**
 * @brief Get the bitrate of the MP3 file.
 * 
 * @return float Bitrate in kbps.
 */
float mp3player_get_bitrate (void) {
    if (!p->loaded) {
        return 0.0f;
    }

    return p->bitrate;
}

/**
 * @brief Get the sample rate of the MP3 file.
 * 
 * @return int Sample rate in Hz.
 */
int mp3player_get_samplerate (void) {
    if (!p->loaded) {
        return 0;
    }

    return p->info.hz;
}

/**
 * @brief Get the progress of the MP3 file playback.
 * 
 * @return float Progress as a percentage.
 */
float mp3player_get_progress (void) {
    // NOTE: Rough approximation using file pointer instead of processed samples.
    //       Good enough but not very accurate for variable bitrate files.

    if (!p->loaded) {
        return 0.0f;
    }

    long data_size = p->file_size - p->data_start;
    long data_consumed = ftell(p->f) - p->buffer_left;
    long data_position = (data_consumed > p->data_start) ? (data_consumed - p->data_start) : 0;

    return data_position / (float) (data_size);
}
