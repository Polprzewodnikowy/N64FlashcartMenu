/**
 * @file id3_parser.h
 * @brief ID3 tag metadata parser for audio files
 * @ingroup menu
 *
 * Parses ID3v1, ID3v2.2, ID3v2.3, and ID3v2.4 tags to extract
 * title, artist, album, and track number metadata.
 */

#ifndef ID3_PARSER_H__
#define ID3_PARSER_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define ID3_FIELD_MAX   (128)

typedef struct {
    char title[ID3_FIELD_MAX];
    char artist[ID3_FIELD_MAX];
    char album[ID3_FIELD_MAX];
    char year[8];
    int track_number;
    bool has_metadata;
    bool has_cover_art;
    bool cover_art_is_png;          /**< true = PNG, false = JPEG */
    size_t cover_art_size;          /**< Size of embedded art data */
    uint8_t *cover_art_data;        /**< Heap-allocated copy of art bytes (caller frees) */
} id3_metadata_t;

/** @brief Flags for id3_parse to control what gets extracted. */
#define ID3_FLAG_EXTRACT_ART    (1 << 0)  /**< Extract embedded cover art to memory */

/**
 * @brief Parse ID3 metadata from an open MP3 file.
 *
 * Tries ID3v2 first (from the beginning of the file), then falls back
 * to ID3v1 (last 128 bytes). The file position is restored afterward.
 * Any parsing failure results in zeroed metadata, never a crash.
 *
 * @param f         Open file handle (must be seekable).
 * @param file_size Total file size in bytes.
 * @param meta      Output metadata struct (zeroed on entry).
 * @param flags     Bitmask of ID3_FLAG_* options. Pass 0 for text-only parsing.
 */
void id3_parse(FILE *f, size_t file_size, id3_metadata_t *meta, int flags);

#endif /* ID3_PARSER_H__ */
