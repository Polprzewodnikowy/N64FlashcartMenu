#ifndef UTF_CONVERTER_H__
#define UTF_CONVERTER_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void utf16_to_utf8(const uint8_t *src, size_t src_len,
                   char *dst, size_t dst_size, bool big_endian);

void latin1_to_utf8(const uint8_t *src, size_t src_len,
                    char *dst, size_t dst_size);

#endif // UTF_CONVERTER_H__
