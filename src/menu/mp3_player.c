/**
 * @file mp3_player.c
 * @brief MP3 Player component implementation
 * @ingroup ui_components
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>
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
    uint8_t buffer[8 * 1024]; /**< File read-ahead buffer */
    uint8_t *buffer_ptr; /**< Pointer to the current position in the buffer */
    size_t buffer_left; /**< Amount of data left in the buffer */

    mp3dec_t dec; /**< MP3 decoder */
    mp3dec_frame_info_t info; /**< MP3 frame information */

    int seek_predecode_frames; /**< Number of frames to pre-decode when seeking */
    float duration; /**< Duration of the MP3 file */
    float bitrate; /**< Bitrate of the MP3 file */

    mp3_metadata_t metadata; /**< ID3 tag metadata */

    waveform_t wave; /**< Waveform structure for playback */
} mp3player_t;

static mp3player_t *p = NULL;

/**
 * @brief Trim trailing whitespace and null bytes from a fixed-length ID3 field.
 * 
 * @param dst   Destination buffer (must be at least dst_size bytes).
 * @param src   Source bytes from the tag.
 * @param len   Number of source bytes to copy.
 * @param dst_size  Size of destination buffer.
 */
static void id3_trim_copy (char *dst, const uint8_t *src, size_t len, size_t dst_size) {
    /* Skip leading spaces and nulls */
    while (len > 0 && (*src == ' ' || *src == '\0')) { src++; len--; }
    if (len >= dst_size) len = dst_size - 1;
    memcpy(dst, src, len);
    dst[len] = '\0';
    /* Strip trailing spaces and nulls */
    for (int i = (int)len - 1; i >= 0 && (dst[i] == ' ' || dst[i] == '\0'); i--) {
        dst[i] = '\0';
    }
}

/**
 * @brief Parse ID3v1 tag from the last 128 bytes of a file.
 * 
 * @param f         Open file handle.
 * @param file_size Total file size.
 * @param meta      Metadata struct to populate.
 * @return true if an ID3v1 tag was found and parsed.
 */
static bool parse_id3v1 (FILE *f, size_t file_size, mp3_metadata_t *meta) {
    if (file_size < 128) return false;

    uint8_t tag[128];
    long saved = ftell(f);
    if (fseek(f, -(long)sizeof(tag), SEEK_END)) return false;
    size_t rd = fread(tag, 1, sizeof(tag), f);
    fseek(f, saved, SEEK_SET);
    if (rd < 128) return false;
    if (memcmp(tag, "TAG", 3) != 0) return false;

    id3_trim_copy(meta->title,  &tag[3],  30, MP3_METADATA_MAX_LEN);
    id3_trim_copy(meta->artist, &tag[33], 30, MP3_METADATA_MAX_LEN);
    id3_trim_copy(meta->album,  &tag[63], 30, MP3_METADATA_MAX_LEN);
    /* ID3v1.1: if tag[125]==0 and tag[126]!=0, tag[126] is track number */
    if (tag[125] == 0 && tag[126] != 0) {
        meta->track_number = tag[126];
    }

    meta->has_metadata = (meta->title[0] || meta->artist[0] || meta->album[0]);
    return meta->has_metadata;
}

/**
 * @brief Parse ID3v2 tag frames from a raw buffer.
 * 
 * Supports ID3v2.3 and ID3v2.4 (4-char frame IDs). Extracts TIT2 (title),
 * TPE1 (artist), TALB (album), and TRCK (track number).
 * 
 * @param buf       Raw file bytes starting at the "ID3" header.
 * @param buf_size  Number of available bytes in the buffer.
 * @param meta      Metadata struct to populate.
 * @return true if any metadata was extracted.
 */
static bool parse_id3v2 (const uint8_t *buf, size_t buf_size, mp3_metadata_t *meta) {
    if (buf_size < 10 || memcmp(buf, "ID3", 3) != 0) return false;

    uint8_t version_major = buf[3];
    if (version_major < 3 || version_major > 4) return false;

    /* Tag size (synchsafe integer, excludes the 10-byte header) */
    size_t tag_size = (((buf[6] & 0x7f) << 21) | ((buf[7] & 0x7f) << 14) |
                       ((buf[8] & 0x7f) << 7)  |  (buf[9] & 0x7f)) + 10;
    if (tag_size > buf_size) tag_size = buf_size;

    /* Skip extended header if present */
    size_t pos = 10;
    if (buf[5] & 0x40) {
        if (pos + 4 > tag_size) return false;
        size_t ext_size = ((size_t)buf[pos] << 24) | ((size_t)buf[pos+1] << 16) |
                          ((size_t)buf[pos+2] << 8) | buf[pos+3];
        pos += ext_size;
    }

    /* Walk frames */
    while (pos + 10 <= tag_size) {
        const uint8_t *fhdr = &buf[pos];
        /* Frame ID must be 4 printable ASCII chars */
        if (fhdr[0] < 'A' || fhdr[0] > 'Z') break;

        size_t frame_size;
        if (version_major == 4) {
            /* v2.4: synchsafe frame size */
            frame_size = ((size_t)(fhdr[4] & 0x7f) << 21) | ((size_t)(fhdr[5] & 0x7f) << 14) |
                         ((size_t)(fhdr[6] & 0x7f) << 7)  |  (size_t)(fhdr[7] & 0x7f);
        } else {
            /* v2.3: plain big-endian */
            frame_size = ((size_t)fhdr[4] << 24) | ((size_t)fhdr[5] << 16) |
                         ((size_t)fhdr[6] << 8)  |  (size_t)fhdr[7];
        }

        pos += 10;
        if (frame_size == 0 || pos + frame_size > tag_size) break;

        const uint8_t *data = &buf[pos];
        size_t data_len = frame_size;

        if (data_len < 1) { pos += frame_size; continue; }

        /* First byte of text frames is the encoding byte */
        uint8_t encoding = data[0];
        const uint8_t *text = data + 1;
        size_t text_len = data_len - 1;

        /* For UTF-16 (encoding 0x01 or 0x02), convert to ASCII/Latin-1.
         * We extract the low byte of each 16-bit code unit (works for
         * Western characters). For ISO-8859-1 (0x00) and UTF-8 (0x03),
         * the bytes are usable directly. */
        char ascii_buf[MP3_METADATA_MAX_LEN];
        if (encoding == 0x01 || encoding == 0x02) {
            bool big_endian = (encoding == 0x02); /* 0x02 = UTF-16BE */

            /* Skip BOM if present */
            if (text_len >= 2) {
                if (text[0] == 0xFF && text[1] == 0xFE) {
                    big_endian = false;
                    text += 2; text_len -= 2;
                } else if (text[0] == 0xFE && text[1] == 0xFF) {
                    big_endian = true;
                    text += 2; text_len -= 2;
                }
            }

            size_t out_len = 0;
            for (size_t i = 0; i + 1 < text_len && out_len < sizeof(ascii_buf) - 1; i += 2) {
                uint8_t lo = big_endian ? text[i + 1] : text[i];
                uint8_t hi = big_endian ? text[i] : text[i + 1];
                if (lo == 0 && hi == 0) break; /* null terminator */
                ascii_buf[out_len++] = (hi == 0 && lo >= 0x20) ? (char)lo : '?';
            }
            ascii_buf[out_len] = '\0';
            text = (const uint8_t *)ascii_buf;
            text_len = out_len;
        }

        char *target = NULL;
        if      (memcmp(fhdr, "TIT2", 4) == 0) target = meta->title;
        else if (memcmp(fhdr, "TPE1", 4) == 0) target = meta->artist;
        else if (memcmp(fhdr, "TALB", 4) == 0) target = meta->album;
        else if (memcmp(fhdr, "TRCK", 4) == 0) {
            /* Track number — may be "3" or "3/12" */
            char tmp[16];
            id3_trim_copy(tmp, text, text_len, sizeof(tmp));
            meta->track_number = atoi(tmp);
        }

        if (target) {
            id3_trim_copy(target, text, text_len, MP3_METADATA_MAX_LEN);
        }

        /* APIC frame — extract embedded PNG cover art */
        if (memcmp(fhdr, "APIC", 4) == 0 && !meta->has_cover_art && data_len > 4) {
            /* Skip encoding byte */
            const uint8_t *apic = data + 1;
            size_t apic_len = data_len - 1;

            /* Read MIME type string (null-terminated) */
            const uint8_t *mime_end = memchr(apic, '\0', apic_len);
            if (mime_end) {
                size_t mime_len = mime_end - apic;
                bool is_png = (mime_len == 9 && memcmp(apic, "image/png", 9) == 0);

                apic = mime_end + 1;  /* Skip past MIME + null */
                apic_len -= (mime_len + 1);

                if (is_png && apic_len > 1) {
                    /* Skip picture type byte */
                    apic++; apic_len--;

                    /* Skip description string (null-terminated) */
                    const uint8_t *desc_end = memchr(apic, '\0', apic_len);
                    if (desc_end) {
                        size_t desc_len = desc_end - apic;
                        apic = desc_end + 1;
                        apic_len -= (desc_len + 1);

                        /* Remaining bytes are the PNG image data */
                        if (apic_len > 8) {
                            const char *tmp_path = "sd:/menu/cache/cover_tmp.png";
                            FILE *tmp = fopen(tmp_path, "wb");
                            if (tmp) {
                                fwrite(apic, 1, apic_len, tmp);
                                fclose(tmp);
                                meta->has_cover_art = true;
                                strncpy(meta->cover_art_path, tmp_path, sizeof(meta->cover_art_path) - 1);
                                meta->cover_art_path[sizeof(meta->cover_art_path) - 1] = '\0';
                            }
                        }
                    }
                }
            }
        }

        pos += frame_size;
    }

    meta->has_metadata = (meta->title[0] || meta->artist[0] || meta->album[0]);
    return meta->has_metadata;
}

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
/* NOTE: We decode into a cached stack buffer first, then memcpy to the
 *       uncached samplebuffer. Writing directly to uncached RDRAM is much slower. */
static void mp3player_wave_read (void *ctx, samplebuffer_t *sbuf, int wpos, int wlen, bool seeking) {
    int16_t pcm_buf[MINIMP3_MAX_SAMPLES_PER_FRAME];

    while (wlen > 0) {
        mp3player_fill_buffer();

        int samples = mp3dec_decode_frame(&p->dec, p->buffer_ptr, p->buffer_left, pcm_buf, &p->info);

        if (samples > 0) {
            int pcm_bytes = samples * p->info.channels * sizeof(int16_t);
            short *out = (short *)samplebuffer_append(sbuf, samples);
            memcpy(out, pcm_buf, pcm_bytes);

            if (p->seek_predecode_frames > 0) {
                p->seek_predecode_frames -= 1;
                memset(out, 0, pcm_bytes);
            }

            wlen -= samples;
        }

        p->buffer_ptr += p->info.frame_bytes;
        p->buffer_left -= p->info.frame_bytes;

        if (p->info.frame_bytes == 0) {
            short *out = (short *)samplebuffer_append(sbuf, wlen);
            memset(out, 0, wlen * sizeof(short) * p->info.channels);
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

    memset(&p->metadata, 0, sizeof(p->metadata));
    mp3player_reset_decoder();

    while (!(feof(p->f) && p->buffer_left == 0)) {
        mp3player_fill_buffer();

        if (ferror(p->f)) {
            fclose(p->f);
            return MP3PLAYER_ERR_IO;
        }

        size_t id3v2_skip = mp3dec_skip_id3v2((const uint8_t *) (p->buffer_ptr), p->buffer_left);
        if (id3v2_skip > 0) {
            /* Where the tag starts and ends in the file */
            long tag_start = ftell(p->f) - p->buffer_left;
            long tag_end = tag_start + (long)id3v2_skip;

            /* Extract metadata from ID3v2 — read the full tag into a temp buffer
             * since it can be much larger than our 8KB read-ahead buffer. */
            if (!p->metadata.has_metadata) {
                uint8_t *tag_buf = malloc(id3v2_skip);
                if (tag_buf) {
                    fseek(p->f, tag_start, SEEK_SET);
                    size_t rd = fread(tag_buf, 1, id3v2_skip, p->f);
                    if (rd == id3v2_skip) {
                        parse_id3v2(tag_buf, id3v2_skip, &p->metadata);
                    }
                    free(tag_buf);
                }
            }

            /* Seek past the ID3v2 tag, reset buffer state */
            if (fseek(p->f, tag_end, SEEK_SET)) {
                fclose(p->f);
                return MP3PLAYER_ERR_IO;
            }
            p->buffer_left = 0;
            p->buffer_ptr = p->buffer;
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

            /* Fall back to ID3v1 if no ID3v2 metadata was found */
            if (!p->metadata.has_metadata) {
                parse_id3v1(p->f, p->file_size, &p->metadata);
            }

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

/**
 * @brief Get metadata extracted from ID3 tags.
 * 
 * @return const mp3_metadata_t* Pointer to metadata (valid while file is loaded).
 */
const mp3_metadata_t *mp3player_get_metadata (void) {
    static const mp3_metadata_t empty = {0};
    if (!p || !p->loaded) return &empty;
    return &p->metadata;
}
