/**
 * @file png_decoder.h
 * @brief PNG decoder
 * @ingroup menu 
 */

#ifndef PNG_DECODER_H__
#define PNG_DECODER_H__

#include <surface.h>

/** 
 * @brief PNG decoder errors 
 * 
 * Enumeration for different types of errors that can occur in the PNG decoder.
 */
typedef enum {
    PNG_OK,               /**< No error */
    PNG_ERR_INT,          /**< Internal error */
    PNG_ERR_BUSY,         /**< Decoder is busy */
    PNG_ERR_OUT_OF_MEM,   /**< Out of memory error */
    PNG_ERR_NO_FILE,      /**< No file found error */
    PNG_ERR_BAD_FILE,     /**< Bad file error */
} png_err_t;

/**
 * @brief PNG decoder callback type.
 * 
 * This typedef defines the callback function type used by the PNG decoder.
 * 
 * @param err Error code indicating the result of the decoding process.
 * @param decoded_image Pointer to the decoded image surface.
 * @param callback_data User-defined data passed to the callback function.
 */
typedef void png_callback_t (png_err_t err, surface_t *decoded_image, void *callback_data);

/**
 * @brief Start the PNG decoding process.
 * 
 * This function starts the PNG decoding process for the specified file.
 * 
 * @param path Path to the PNG file.
 * @param max_width Maximum width of the decoded image.
 * @param max_height Maximum height of the decoded image.
 * @param callback Callback function to be called when decoding is complete.
 * @param callback_data User-defined data to be passed to the callback function.
 * @return png_err_t Error code indicating the result of the start operation.
 */
png_err_t png_decoder_start (char *path, int max_width, int max_height, png_callback_t *callback, void *callback_data);

/**
 * @brief Abort the PNG decoding process.
 * 
 * This function aborts the ongoing PNG decoding process.
 */
void png_decoder_abort (void);

/**
 * @brief Get the progress of the PNG decoding process.
 * 
 * This function returns the current progress of the PNG decoding process as a percentage.
 * 
 * @return float Current progress of the decoding process (0.0 to 100.0).
 */
float png_decoder_get_progress (void);

/**
 * @brief Poll the PNG decoder.
 * 
 * This function polls the PNG decoder to handle any ongoing decoding tasks.
 */
void png_decoder_poll (void);

#endif /* PNG_DECODER_H__ */
