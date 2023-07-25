/**
 * @file png.h
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
    PNG_ERR_OUT_OF_MEM,
    PNG_ERR_NO_FILE,
    PNG_ERR_BAD_FILE,
} png_err_t;


png_err_t png_decode (char *path, surface_t *image, int max_width, int max_height);


#endif
