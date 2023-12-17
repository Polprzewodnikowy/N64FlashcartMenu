#include <fatfs/ff.h>
#include <libspng/spng/spng.h>

#include "png_decoder.h"
#include "utils/fs.h"

/** @brief PNG File Information Structure. */
typedef struct {
    FIL fil;

    spng_ctx *ctx;
    struct spng_ihdr ihdr;

    surface_t *image;
    uint8_t *row_buffer;
    int decoded_rows;

    png_callback_t *callback;
    void *callback_data;
} png_decoder_t;

static png_decoder_t *decoder;


static void png_decoder_deinit (bool free_image) {
    if (decoder != NULL) {
        f_close(&decoder->fil);
        if (decoder->ctx != NULL) {
            spng_ctx_free(decoder->ctx);
        }
        if ((decoder->image != NULL) && free_image) {
            surface_free(decoder->image);
            free(decoder->image);
        }
        if (decoder->row_buffer != NULL) {
            free(decoder->row_buffer);
        }
        free(decoder);
        decoder = NULL;
    }
}

static int png_file_read (spng_ctx *ctx, void *user, void *dst_src, size_t length) {
    UINT bytes_read = 0;
    png_decoder_t *d = (png_decoder_t *) (user);

    if (f_read(&d->fil, dst_src, length, &bytes_read) != FR_OK) {
        return SPNG_IO_ERROR;
    }

    if (bytes_read != length) {
        return SPNG_EOF;
    }

    return SPNG_OK;
}


png_err_t png_decoder_start (char *path, int max_width, int max_height, png_callback_t *callback, void *callback_data) {
    if (decoder != NULL) {
        return PNG_ERR_BUSY;
    }

    decoder = calloc(1, sizeof(png_decoder_t));
    if (decoder == NULL) {
        return PNG_ERR_OUT_OF_MEM;
    }

    if (f_open(&decoder->fil, strip_sd_prefix(path), FA_READ) != FR_OK) {
        png_decoder_deinit(false);
        return PNG_ERR_NO_FILE;
    }

    if ((decoder->ctx = spng_ctx_new(SPNG_CTX_IGNORE_ADLER32)) == NULL) {
        png_decoder_deinit(false);
        return PNG_ERR_OUT_OF_MEM;
    }

    if (spng_set_crc_action(decoder->ctx, SPNG_CRC_USE, SPNG_CRC_USE) != SPNG_OK) {
        png_decoder_deinit(false);
        return PNG_ERR_INT;
    }

    if (spng_set_image_limits(decoder->ctx, max_width, max_height) != SPNG_OK) {
        png_decoder_deinit(false);
        return PNG_ERR_INT;
    }

    if (spng_set_png_stream(decoder->ctx, png_file_read, decoder) != SPNG_OK) {
        png_decoder_deinit(false);
        return PNG_ERR_INT;
    }

    size_t image_size;

    if (spng_decoded_image_size(decoder->ctx, SPNG_FMT_RGB8, &image_size) != SPNG_OK) {
        png_decoder_deinit(false);
        return PNG_ERR_BAD_FILE;
    }

    if (spng_decode_image(decoder->ctx, NULL, image_size, SPNG_FMT_RGB8, SPNG_DECODE_PROGRESSIVE) != SPNG_OK) {
        png_decoder_deinit(false);
        return PNG_ERR_BAD_FILE;
    }

    if (spng_get_ihdr(decoder->ctx, &decoder->ihdr) != SPNG_OK) {
        png_decoder_deinit(false);
        return PNG_ERR_BAD_FILE;
    }

    decoder->image = calloc(1, sizeof(surface_t));
    if (decoder->image == NULL) {
        png_decoder_deinit(false);
        return PNG_ERR_OUT_OF_MEM;
    }

    *decoder->image = surface_alloc(FMT_RGBA16, decoder->ihdr.width, decoder->ihdr.height);
    if (decoder->image->buffer == NULL) {
        png_decoder_deinit(true);
        return PNG_ERR_OUT_OF_MEM;
    }

    if ((decoder->row_buffer = malloc(decoder->ihdr.width * 3)) == NULL) {
        png_decoder_deinit(true);
        return PNG_ERR_OUT_OF_MEM;
    }

    decoder->decoded_rows = 0;

    decoder->callback = callback;
    decoder->callback_data = callback_data;

    return PNG_OK;
}

void png_decoder_abort (void) {
    png_decoder_deinit(true);
}

float png_decoder_get_progress (void) {
    if (!decoder) {
        return 0.0f;
    }

    return (float) (decoder->decoded_rows) / (decoder->ihdr.height);
}

void png_decoder_poll (void) {
    if (!decoder) {
        return;
    }

    enum spng_errno err;
    struct spng_row_info row_info;

    if ((err = spng_get_row_info(decoder->ctx, &row_info)) != SPNG_OK) {
        decoder->callback(PNG_ERR_BAD_FILE, NULL, decoder->callback_data);
        png_decoder_deinit(true);
        return;
    }

    err = spng_decode_row(decoder->ctx, decoder->row_buffer, decoder->ihdr.width * 3);

    if (err == SPNG_OK || err == SPNG_EOI) {
        decoder->decoded_rows += 1;
        uint16_t *image_buffer = decoder->image->buffer + (row_info.row_num * decoder->image->stride);
        for (int i = 0; i < decoder->ihdr.width * 3; i += 3) {
            uint8_t r = decoder->row_buffer[i + 0] >> 3;
            uint8_t g = decoder->row_buffer[i + 1] >> 3;
            uint8_t b = decoder->row_buffer[i + 2] >> 3;
            *image_buffer++ = (r << 11) | (g << 6) | (b << 1) | 1;
        }
    }

    if (err == SPNG_EOI) {
        decoder->callback(PNG_OK, decoder->image, decoder->callback_data);
        png_decoder_deinit(false);
    } else if (err != SPNG_OK) {
        decoder->callback(PNG_ERR_BAD_FILE, NULL, decoder->callback_data);
        png_decoder_deinit(true);
    }
}
