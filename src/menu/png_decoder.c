/**
 * @file png_decoder.c
 * @brief PNG Decoder component implementation
 * @ingroup ui_components
 */

#include <stdio.h>
#include <libspng/spng/spng.h>
#include "png_decoder.h"
#include "utils/fs.h"

/** @brief PNG File Information Structure. */
typedef struct {
    FILE *f; /**< File pointer */
    spng_ctx *ctx; /**< SPNG context */
    struct spng_ihdr ihdr; /**< SPNG image header */
    surface_t *image; /**< Image surface */
    uint8_t *row_buffer; /**< Row buffer */
    int decoded_rows; /**< Number of decoded rows */
    png_callback_t *callback; /**< Callback function */
    void *callback_data; /**< Callback data */
} png_decoder_t;

static png_decoder_t *decoder;

/**
 * @brief Deinitialize the PNG decoder.
 * 
 * @param free_image Flag indicating whether to free the image.
 */
static void png_decoder_deinit (bool free_image) {
    if (decoder != NULL) {
        fclose(decoder->f);
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

/**
 * @brief Start decoding a PNG file.
 * 
 * @param path Path to the PNG file.
 * @param max_width Maximum width of the image.
 * @param max_height Maximum height of the image.
 * @param callback Callback function to be called upon completion.
 * @param callback_data Data to be passed to the callback function.
 * @return png_err_t Error code.
 */
png_err_t png_decoder_start (char *path, int max_width, int max_height, png_callback_t *callback, void *callback_data) {
    if (decoder != NULL) {
        return PNG_ERR_BUSY;
    }

    decoder = calloc(1, sizeof(png_decoder_t));
    if (decoder == NULL) {
        return PNG_ERR_OUT_OF_MEM;
    }

    if ((decoder->f = fopen(path, "rb")) == NULL) {
        png_decoder_deinit(false);
        return PNG_ERR_NO_FILE;
    }

    setbuf(decoder->f, NULL);

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

    if (spng_set_png_file(decoder->ctx, decoder->f) != SPNG_OK) {
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

/**
 * @brief Abort the PNG decoding process.
 */
void png_decoder_abort (void) {
    png_decoder_deinit(true);
}

/**
 * @brief Get the progress of the PNG decoding process.
 * 
 * @return float Progress as a percentage.
 */
float png_decoder_get_progress (void) {
    if (!decoder) {
        return 0.0f;
    }

    return (float) (decoder->decoded_rows) / (decoder->ihdr.height);
}

/**
 * @brief Poll the PNG decoder to process the next row.
 */
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
