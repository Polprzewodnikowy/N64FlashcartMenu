/**
 * @file jpeg_decoder.h
 * @brief JPEG decoder component stub - For now, we only support PNG, so this is a stub to avoid linker errors.
 */

#ifndef JPEG_DECODER_H__
#define JPEG_DECODER_H__

/** @brief JPEG decoder error codes. */
typedef enum {
    JPEG_OK             =  0,
    JPEG_ERR_INT        = -1,
    JPEG_ERR_BUSY       = -2,
    JPEG_ERR_OUT_OF_MEM = -3,
    JPEG_ERR_NO_FILE    = -4,
    JPEG_ERR_BAD_FILE   = -5,
} jpeg_err_t;

#endif /* JPEG_DECODER_H__ */
