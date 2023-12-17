/**
 * @file png_decoder.h
 * @brief PNG decoder
 * @ingroup menu 
 */

#ifndef PNG_DECODER_H__
#define PNG_DECODER_H__


#include <surface.h>


/** @brief PNG decoder errors */
typedef enum {
    PNG_OK,
    PNG_ERR_INT,
    PNG_ERR_BUSY,
    PNG_ERR_OUT_OF_MEM,
    PNG_ERR_NO_FILE,
    PNG_ERR_BAD_FILE,
} png_err_t;

typedef void png_callback_t (png_err_t err, surface_t *decoded_image, void *callback_data);


png_err_t png_decoder_start (char *path, int max_width, int max_height, png_callback_t *callback, void *callback_data);
void png_decoder_abort (void);
float png_decoder_get_progress (void);
void png_decoder_poll (void);


#endif
