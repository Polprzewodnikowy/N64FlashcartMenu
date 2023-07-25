#include <libspng/spng/spng.h>

#include "path.h"
#include "png_decoder.h"


png_err_t png_decode (char *path, surface_t *image, int max_width, int max_height) {
    spng_ctx *ctx;
    enum spng_errno err = SPNG_OK;
    path_t *file_path;
    FILE *file;
    size_t image_size;
    struct spng_ihdr ihdr;
    uint8_t *row_buffer;
    uint16_t *image_buffer;

    image->buffer = NULL;

    if ((ctx = spng_ctx_new(SPNG_CTX_IGNORE_ADLER32)) == NULL) {
        return PNG_ERR_OUT_OF_MEM;
    }

    if (spng_set_crc_action(ctx, SPNG_CRC_USE, SPNG_CRC_USE) != SPNG_OK) {
        spng_ctx_free(ctx);
        return PNG_ERR_INT;
    }

    if (spng_set_image_limits(ctx, max_width, max_height) != SPNG_OK) {
        spng_ctx_free(ctx);
        return PNG_ERR_INT;
    }

    file_path = path_init("sd:/");
    path_append(file_path, path);
    if ((file = fopen(path_get(file_path), "r")) == NULL) {
        spng_ctx_free(ctx);
        path_free(file_path);
        return PNG_ERR_NO_FILE;
    }
    path_free(file_path);

    if (spng_set_png_file(ctx, file) != SPNG_OK) {
        spng_ctx_free(ctx);
        fclose(file);
        return PNG_ERR_INT;
    }

    if (spng_decoded_image_size(ctx, SPNG_FMT_RGB8, &image_size) != SPNG_OK) {
        spng_ctx_free(ctx);
        fclose(file);
        return PNG_ERR_BAD_FILE;
    }

    if (spng_decode_image(ctx, NULL, image_size, SPNG_FMT_RGB8, SPNG_DECODE_PROGRESSIVE) != SPNG_OK) {
        spng_ctx_free(ctx);
        fclose(file);
        return PNG_ERR_BAD_FILE;
    }

    if (spng_get_ihdr(ctx, &ihdr) != SPNG_OK) {
        spng_ctx_free(ctx);
        fclose(file);
        return PNG_ERR_BAD_FILE;
    }

    *image = surface_alloc(FMT_RGBA16, ihdr.width, ihdr.height);
    image_buffer = image->buffer;
    if (image_buffer == NULL) {
        spng_ctx_free(ctx);
        fclose(file);
        return PNG_ERR_OUT_OF_MEM;
    }

    if ((row_buffer = malloc(ihdr.width * 3)) == NULL) {
        spng_ctx_free(ctx);
        fclose(file);
        surface_free(image);
        return PNG_ERR_OUT_OF_MEM;
    }

    while ((err = spng_decode_row(ctx, row_buffer, ihdr.width * 3)) == SPNG_OK) {
        for (int i = 0; i < ihdr.width * 3; i += 3) {
            uint8_t r = row_buffer[i + 0] >> 3;
            uint8_t g = row_buffer[i + 1] >> 3;
            uint8_t b = row_buffer[i + 2] >> 3;
            *image_buffer++ = (r << 11) | (g << 6) | (b << 1) | 1;
        }
    }

    spng_ctx_free(ctx);
    free(row_buffer);
    fclose(file);

    if ((err != SPNG_OK) && (err != SPNG_EOI)) {
        surface_free(image);
        return PNG_ERR_BAD_FILE;
    }

    return PNG_OK;
}
