/**
 * @file jpeg_decoder.c
 * @brief JPEG decoder component stub - For now, we only support PNG, so this is a stub to avoid linker errors.
 */

#include <stdio.h>
#include <stdlib.h>
#include <libdragon.h>
#include "jpeg_decoder.h"

jpeg_err_t jpeg_decoder_start_mem (void *buf, size_t buf_size, int max_width, int max_height, jpeg_callback_t *callback, void *callback_data) {
    free(buf);
    if (callback) callback(JPEG_ERR_INT, NULL, callback_data);
    return JPEG_ERR_INT;
}

void jpeg_decoder_abort (void) {}

float jpeg_decoder_get_progress (void) { return 0.0f; }

void jpeg_decoder_poll (void) {}
