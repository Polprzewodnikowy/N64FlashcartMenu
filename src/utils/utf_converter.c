/**
 * @file utf_converter.c
 * @brief Text encoding conversions to UTF-8
 * @ingroup utils
 *
 * Handles BOM detection, surrogate pairs, and both big/little endian.
 * Non-representable characters are skipped rather than replaced, so
 * CJK and other scripts come through as valid UTF-8 when the display
 * font supports them.
 */

#include "utf_converter.h"

void utf16_to_utf8(const uint8_t *src, size_t src_len,
                   char *dst, size_t dst_size, bool big_endian) {
    size_t out = 0;

    if (dst_size == 0) return;

    /* Detect and consume BOM */
    if (src_len >= 2) {
        if (src[0] == 0xFF && src[1] == 0xFE) {
            big_endian = false; src += 2; src_len -= 2;
        } else if (src[0] == 0xFE && src[1] == 0xFF) {
            big_endian = true;  src += 2; src_len -= 2;
        }
    }

    for (size_t i = 0; i + 1 < src_len; i += 2) {
        uint16_t code_unit = big_endian
            ? ((uint16_t)src[i] << 8) | src[i + 1]
            : ((uint16_t)src[i + 1] << 8) | src[i];

        if (code_unit == 0) break;

        uint32_t codepoint;

        /* Handle surrogate pairs for characters above U+FFFF */
        if (code_unit >= 0xD800 && code_unit <= 0xDBFF) {
            if (i + 3 >= src_len) break;
            uint16_t low = big_endian
                ? ((uint16_t)src[i + 2] << 8) | src[i + 3]
                : ((uint16_t)src[i + 3] << 8) | src[i + 2];
            if (low < 0xDC00 || low > 0xDFFF) continue;
            codepoint = 0x10000 + ((uint32_t)(code_unit - 0xD800) << 10) + (low - 0xDC00);
            i += 2;
        } else if (code_unit >= 0xDC00 && code_unit <= 0xDFFF) {
            continue; /* stray low surrogate */
        } else {
            codepoint = code_unit;
        }

        /* Encode as UTF-8 */
        if (codepoint < 0x80) {
            if (out + 1 >= dst_size) break;
            dst[out++] = (char)codepoint;
        } else if (codepoint < 0x800) {
            if (out + 2 >= dst_size) break;
            dst[out++] = (char)(0xC0 | (codepoint >> 6));
            dst[out++] = (char)(0x80 | (codepoint & 0x3F));
        } else if (codepoint < 0x10000) {
            if (out + 3 >= dst_size) break;
            dst[out++] = (char)(0xE0 | (codepoint >> 12));
            dst[out++] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
            dst[out++] = (char)(0x80 | (codepoint & 0x3F));
        } else {
            if (out + 4 >= dst_size) break;
            dst[out++] = (char)(0xF0 | (codepoint >> 18));
            dst[out++] = (char)(0x80 | ((codepoint >> 12) & 0x3F));
            dst[out++] = (char)(0x80 | ((codepoint >> 6) & 0x3F));
            dst[out++] = (char)(0x80 | (codepoint & 0x3F));
        }
    }

    dst[out] = '\0';
}

void latin1_to_utf8(const uint8_t *src, size_t src_len,
                    char *dst, size_t dst_size) {
    size_t out = 0;

    if (dst_size == 0) return;

    for (size_t i = 0; i < src_len; i++) {
        uint8_t ch = src[i];
        if (ch == 0) break;

        if (ch < 0x80) {
            if (out + 1 >= dst_size) break;
            dst[out++] = (char)ch;
        } else {
            if (out + 2 >= dst_size) break;
            dst[out++] = (char)(0xC0 | (ch >> 6));
            dst[out++] = (char)(0x80 | (ch & 0x3F));
        }
    }

    dst[out] = '\0';
}
