/**
 * @file jpeg_decoder.h
 * @brief JPEG decoder component — wraps IJG libjpeg with shrink-on-decode support.
 * @ingroup ui_components
 */

#ifndef JPEG_DECODER_H__
#define JPEG_DECODER_H__

#include <surface.h>

/** @brief JPEG decoder error codes. */
typedef enum {
    JPEG_OK             =  0,
    JPEG_ERR_INT        = -1,
    JPEG_ERR_BUSY       = -2,
    JPEG_ERR_OUT_OF_MEM = -3,
    JPEG_ERR_NO_FILE    = -4,
    JPEG_ERR_BAD_FILE   = -5,
} jpeg_err_t;

/** @brief Callback invoked when decoding completes (or fails).
 *  On success, @p decoded_image is a heap-allocated surface_t (caller must surface_free + free).
 *  On failure, @p decoded_image is NULL. */
typedef void (jpeg_callback_t)(jpeg_err_t err, surface_t *decoded_image, void *callback_data);

/**
 * @brief Start decoding a JPEG file.
 *
 * Uses libjpeg's DCT-domain scaling to decode at the smallest scale factor
 * that produces an image >= max_width x max_height, then scales to fit exactly.
 * This avoids allocating a full-resolution pixel buffer for large source images.
 *
 * @param path           Path to the JPEG file.
 * @param max_width      Maximum output width in pixels.
 * @param max_height     Maximum output height in pixels.
 * @param callback       Called when decoding completes.
 * @param callback_data  Passed through to callback.
 * @return JPEG_OK on success. The callback is called before this function returns.
 */
jpeg_err_t jpeg_decoder_start (char *path, int max_width, int max_height,
                               jpeg_callback_t *callback, void *callback_data);

/** @brief Abort and free all resources if a decode is in progress. */
void jpeg_decoder_abort (void);

/** @brief Returns decode progress (0.0–1.0). Provided for API parity with png_decoder. */
float jpeg_decoder_get_progress (void);

/** @brief No-op for API parity with png_decoder; JPEG decoding is synchronous. */
void jpeg_decoder_poll (void);

#endif /* JPEG_DECODER_H__ */
