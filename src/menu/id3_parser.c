/**
 * @file id3_parser.c
 * @brief ID3 tag metadata parser
 * @ingroup menu
 *
 * Supports ID3v1, ID3v2.2 (3-char frames), ID3v2.3, and ID3v2.4
 * (4-char frames). Handles UTF-16 to UTF-8 conversion for text
 * frames and ID3v2 unsynchronization.
 */


#include <libdragon.h>
#include <stdlib.h>
#include <string.h>

#include "utils/utf_converter.h"
#include "id3_parser.h"

#define ID3V2_MAX_TAG_SIZE      (1024 * 1024)


/* --- String helpers --- */

/** Copy and trim leading/trailing whitespace and nulls from a tag field. */
static void trim_copy(char *dst, const uint8_t *src, size_t len, size_t dst_size) {
    if (dst_size == 0) return;
    while (len > 0 && (*src == ' ' || *src == '\0')) { src++; len--; }
    if (len >= dst_size) len = dst_size - 1;
    memcpy(dst, src, len);
    dst[len] = '\0';
    for (int i = (int)len - 1; i >= 0 && (dst[i] == ' ' || dst[i] == '\0'); i--) {
        dst[i] = '\0';
    }
}


/* --- Unsynchronization --- */

/**
 * @brief Remove ID3v2 unsynchronization in place.
 *
 * The ID3v2 spec allows inserting 0x00 after every 0xFF byte to prevent
 * false sync detection by hardware decoders. This removes those padding
 * bytes so the frame data is the original payload.
 *
 * @param data  Buffer to de-unsynchronize in place.
 * @param len   Length of input data.
 * @return      New length after removing padding bytes.
 */
static size_t de_unsync(uint8_t *data, size_t len) {
    size_t out = 0;
    for (size_t i = 0; i < len; i++) {
        data[out++] = data[i];
        if (data[i] == 0xFF && i + 1 < len && data[i + 1] == 0x00) {
            i++; /* skip the padding byte */
        }
    }
    return out;
}


/* --- ID3v1 --- */

static bool parse_id3v1(FILE *f, size_t file_size, id3_metadata_t *meta) {
    if (file_size < 128) return false;

    uint8_t tag[128];
    long saved = ftell(f);
    if (fseek(f, -(long)sizeof(tag), SEEK_END)) return false;
    size_t rd = fread(tag, 1, sizeof(tag), f);
    fseek(f, saved, SEEK_SET);
    if (rd < 128 || memcmp(tag, "TAG", 3) != 0) return false;

    trim_copy(meta->title,  &tag[3],  30, ID3_FIELD_MAX);
    trim_copy(meta->artist, &tag[33], 30, ID3_FIELD_MAX);
    trim_copy(meta->album,  &tag[63], 30, ID3_FIELD_MAX);
    trim_copy(meta->year,   &tag[93],  4, sizeof(meta->year));

    /* ID3v1.1: if byte 125 is zero and byte 126 is nonzero, it's the track number */
    if (tag[125] == 0 && tag[126] != 0) {
        meta->track_number = tag[126];
    }

    meta->has_metadata = (meta->title[0] || meta->artist[0] || meta->album[0]);
    return meta->has_metadata;
}


/* --- ID3v2 --- */

/** Read a synchsafe integer (7 bits per byte). */
static size_t read_syncsafe(const uint8_t *b) {
    return ((size_t)(b[0] & 0x7F) << 21) |
           ((size_t)(b[1] & 0x7F) << 14) |
           ((size_t)(b[2] & 0x7F) << 7)  |
            (size_t)(b[3] & 0x7F);
}

/** Read a plain big-endian 32-bit integer. */
static size_t read_be32(const uint8_t *b) {
    return ((size_t)b[0] << 24) | ((size_t)b[1] << 16) |
           ((size_t)b[2] << 8)  |  (size_t)b[3];
}

/** Read a plain big-endian 24-bit integer (ID3v2.2 frame size). */
static size_t read_be24(const uint8_t *b) {
    return ((size_t)b[0] << 16) | ((size_t)b[1] << 8) | (size_t)b[2];
}

/**
 * @brief Decode a text frame payload into a destination string.
 *
 * Handles all four ID3v2 text encodings:
 *   0x00 = ISO-8859-1, 0x01 = UTF-16 with BOM,
 *   0x02 = UTF-16BE, 0x03 = UTF-8.
 */
static void decode_text_frame(const uint8_t *data, size_t data_len,
                              char *dst, size_t dst_size) {
    if (data_len < 1) return;

    uint8_t encoding = data[0];
    const uint8_t *text = data + 1;
    size_t text_len = data_len - 1;

    if (encoding == 0x01 || encoding == 0x02) {
        utf16_to_utf8(text, text_len, dst, dst_size, encoding == 0x02);
    } else if (encoding == 0x00) {
        latin1_to_utf8(text, text_len, dst, dst_size);
    } else {
        /* UTF-8 (0x03), usable as-is */
        trim_copy(dst, text, text_len, dst_size);
    }
}

/**
 * @brief Match a frame ID to a metadata field, handling both
 *        ID3v2.2 (3-char) and ID3v2.3/v2.4 (4-char) frame IDs.
 *
 * @return Pointer to the target field in meta, or NULL if not a field we care about.
 */
static char *match_frame_id(const uint8_t *id, int id_len, id3_metadata_t *meta) {
    if (id_len == 4) {
        if (memcmp(id, "TIT2", 4) == 0) return meta->title;
        if (memcmp(id, "TPE1", 4) == 0) return meta->artist;
        if (memcmp(id, "TALB", 4) == 0) return meta->album;
    } else if (id_len == 3) {
        if (memcmp(id, "TT2", 3) == 0) return meta->title;
        if (memcmp(id, "TP1", 3) == 0) return meta->artist;
        if (memcmp(id, "TAL", 3) == 0) return meta->album;
    }
    return NULL;
}

/** Check if a frame ID is a year/date frame. */
static bool is_year_frame(const uint8_t *id, int id_len) {
    if (id_len == 4) return memcmp(id, "TYER", 4) == 0 || memcmp(id, "TDRC", 4) == 0;
    if (id_len == 3) return memcmp(id, "TYE", 3) == 0;
    return false;
}

/** Check if a frame ID looks like a track number frame. */
static bool is_track_frame(const uint8_t *id, int id_len) {
    if (id_len == 4) return memcmp(id, "TRCK", 4) == 0;
    if (id_len == 3) return memcmp(id, "TRK", 3) == 0;
    return false;
}

/** Check if a frame ID is an attached picture frame. */
static bool is_apic_frame(const uint8_t *id, int id_len) {
    if (id_len == 4) return memcmp(id, "APIC", 4) == 0;
    if (id_len == 3) return memcmp(id, "PIC", 3) == 0;
    return false;
}

/** Extract an APIC frame's image data into a heap buffer.
 *  Returns true on success. */
static bool extract_apic(const uint8_t *data, size_t data_len, int id_len, id3_metadata_t *meta) {
    if (data_len < 4) return false;

    /* Skip encoding byte */
    const uint8_t *p = data + 1;
    size_t remaining = data_len - 1;

    bool is_png = false;
    bool is_jpeg = false;

    if (id_len == 4) {
        /* ID3v2.3/v2.4: MIME type is a null-terminated string */
        const uint8_t *mime_end = memchr(p, '\0', remaining);
        if (!mime_end) return false;
        size_t mime_len = mime_end - p;

        is_png  = (mime_len == 9  && memcmp(p, "image/png",  9) == 0);
        is_jpeg = (mime_len == 10 && memcmp(p, "image/jpeg", 10) == 0) ||
                  (mime_len == 9  && memcmp(p, "image/jpg",  9) == 0);

        remaining -= (mime_len + 1);
        p = mime_end + 1;
    } else {
        /* ID3v2.2: 3-byte image format (e.g. "JPG", "PNG") */
        if (remaining < 3) return false;
        is_png  = (memcmp(p, "PNG", 3) == 0);
        is_jpeg = (memcmp(p, "JPG", 3) == 0);
        p += 3; remaining -= 3;
    }

    if (!is_png && !is_jpeg) return false;
    if (remaining < 2) return false;

    /* Skip picture type byte */
    p++; remaining--;

    /* Skip description string. UTF-16 encodings use double-null terminator. */
    uint8_t encoding = data[0];
    if (encoding == 0x01 || encoding == 0x02) {
        /* UTF-16: scan for 0x00 0x00 on an even boundary */
        bool found = false;
        for (size_t i = 0; i + 1 < remaining; i += 2) {
            if (p[i] == 0x00 && p[i + 1] == 0x00) {
                p += i + 2;
                remaining -= (i + 2);
                found = true;
                break;
            }
        }
        if (!found) return false;
    } else {
        /* ISO-8859-1 or UTF-8: single null terminator */
        const uint8_t *desc_end = memchr(p, '\0', remaining);
        if (!desc_end) return false;
        size_t desc_len = desc_end - p;
        p = desc_end + 1;
        remaining -= (desc_len + 1);
    }

    if (remaining < 8) return false;

    /* Only allocate if we have enough free memory (keep 20% headroom) */
    heap_stats_t heap;
    sys_get_heap_stats(&heap);
    size_t free_mem = (size_t)(heap.total - heap.used);
    if (remaining > (size_t)(free_mem * 0.8f)) return false;

    uint8_t *buf = malloc(remaining);
    if (!buf) return false;
    memcpy(buf, p, remaining);

    meta->has_cover_art = true;
    meta->cover_art_is_png = is_png;
    meta->cover_art_size = remaining;
    meta->cover_art_data = buf;
    return true;
}

static bool parse_id3v2(const uint8_t *buf, size_t buf_size, id3_metadata_t *meta, int flags) {
    if (buf_size < 10 || memcmp(buf, "ID3", 3) != 0) return false;

    uint8_t version = buf[3];
    if (version < 2 || version > 4) return false;

    uint8_t hdr_flags = buf[5];
    bool tag_unsync = (hdr_flags & 0x80) != 0;

    size_t tag_size = read_syncsafe(&buf[6]) + 10;
    if (tag_size > buf_size) tag_size = buf_size;

    /* For unsynchronized tags, work on a mutable copy */
    uint8_t *work_buf = NULL;
    if (tag_unsync) {
        work_buf = malloc(tag_size);
        if (!work_buf) return false;
        memcpy(work_buf, buf, tag_size);
        tag_size = 10 + de_unsync(work_buf + 10, tag_size - 10);
        buf = work_buf;
    }

    /* Frame header sizes differ by version */
    int id_len = (version == 2) ? 3 : 4;
    int frame_hdr_size = (version == 2) ? 6 : 10;

    /* Skip extended header if present (v2.3/v2.4 only) */
    size_t pos = 10;
    if (version >= 3 && (hdr_flags & 0x40)) {
        if (pos + 4 > tag_size) goto done;
        size_t ext_size = (version == 4) ? read_syncsafe(&buf[pos]) : read_be32(&buf[pos]);
        size_t skip = (version == 3) ? ext_size + 4 : ext_size;
        if (skip > tag_size - pos) goto done;
        pos += skip;
    }

    /* Walk frames */
    while (pos + frame_hdr_size <= tag_size) {
        const uint8_t *fhdr = &buf[pos];

        /* Frame ID must start with an uppercase letter */
        if (fhdr[0] < 'A' || fhdr[0] > 'Z') break;

        size_t frame_size;
        if (version == 2) {
            frame_size = read_be24(&fhdr[3]);
        } else if (version == 4) {
            frame_size = read_syncsafe(&fhdr[4]);
        } else {
            frame_size = read_be32(&fhdr[4]);
        }

        pos += frame_hdr_size;
        if (frame_size == 0 || pos > tag_size || frame_size > tag_size - pos) break;

        const uint8_t *data = &buf[pos];
        size_t data_len = frame_size;

        /* Check for text fields we want */
        char *target = match_frame_id(fhdr, id_len, meta);
        if (target) {
            decode_text_frame(data, data_len, target, ID3_FIELD_MAX);
        } else if (is_track_frame(fhdr, id_len)) {
            char tmp[16];
            decode_text_frame(data, data_len, tmp, sizeof(tmp));
            meta->track_number = atoi(tmp);
        } else if (is_year_frame(fhdr, id_len) && !meta->year[0]) {
            char tmp[32];
            decode_text_frame(data, data_len, tmp, sizeof(tmp));
            /* TDRC can be "2024-03-15", just take the first 4 chars (year) */
            strncpy(meta->year, tmp, 4);
            meta->year[4] = '\0';
        } else if ((flags & ID3_FLAG_EXTRACT_ART) && !meta->has_cover_art && is_apic_frame(fhdr, id_len)) {
            extract_apic(data, data_len, id_len, meta);
        }

        pos += frame_size;
    }

done:
    free(work_buf);
    meta->has_metadata = (meta->title[0] || meta->artist[0] || meta->album[0]);
    return meta->has_metadata;
}


/* --- Public API --- */

void id3_parse(FILE *f, size_t file_size, id3_metadata_t *meta, int flags) {
    memset(meta, 0, sizeof(*meta));

    long saved = ftell(f);

    /* Try ID3v2 from the start of the file */
    fseek(f, 0, SEEK_SET);

    uint8_t header[10];
    if (fread(header, 1, 10, f) == 10 && memcmp(header, "ID3", 3) == 0) {
        size_t tag_size = read_syncsafe(&header[6]) + 10;
        if (tag_size <= ID3V2_MAX_TAG_SIZE) {
            heap_stats_t heap;
            sys_get_heap_stats(&heap);
            size_t free_mem = (size_t)(heap.total - heap.used);
            uint8_t *tag_buf = (free_mem > tag_size + 64 * 1024) ? malloc(tag_size) : NULL;
            if (tag_buf) {
                fseek(f, 0, SEEK_SET);
                size_t rd = fread(tag_buf, 1, tag_size, f);
                if (rd == tag_size) {
                    parse_id3v2(tag_buf, tag_size, meta, flags);
                }
                free(tag_buf);
            }
        }
    }

    /* Fall back to ID3v1 if nothing found */
    if (!meta->has_metadata) {
        parse_id3v1(f, file_size, meta);
    }

    fseek(f, saved, SEEK_SET);
}
