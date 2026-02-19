/**
 * @file jpeg_decoder.c
 * @brief JPEG decoder — uses IJG libjpeg with DCT-domain shrink-on-decode.
 * @ingroup ui_components
 */

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <libdragon.h>
#include <jpeglib.h>
#include <jerror.h>
#include "jpeg_decoder.h"
#include "utils/fs.h"


typedef struct {
    surface_t      *image;
    jpeg_err_t      result;
    bool            done;
    jpeg_callback_t *callback;
    void            *callback_data;
} jpeg_decoder_t;

static jpeg_decoder_t *decoder;


static void jpeg_decoder_deinit (bool free_image) {
    if (decoder) {
        if (decoder->image && free_image) {
            surface_free(decoder->image);
            free(decoder->image);
        }
        free(decoder);
        decoder = NULL;
    }
}

/* libjpeg error handler that longjmps instead of calling exit(). */
typedef struct {
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buf;
} jpeg_error_mgr_ex_t;

static void jpeg_error_exit_ex (j_common_ptr cinfo) {
    jpeg_error_mgr_ex_t *err = (jpeg_error_mgr_ex_t *)cinfo->err;
    longjmp(err->setjmp_buf, 1);
}


jpeg_err_t jpeg_decoder_start (char *path, int max_width, int max_height,
                               jpeg_callback_t *callback, void *callback_data) {
    if (decoder) {
        return JPEG_ERR_BUSY;
    }

    decoder = calloc(1, sizeof(jpeg_decoder_t));
    if (!decoder) {
        return JPEG_ERR_OUT_OF_MEM;
    }
    decoder->callback      = callback;
    decoder->callback_data = callback_data;

    FILE *f = fopen(path, "rb");
    if (!f) {
        jpeg_decoder_deinit(false);
        return JPEG_ERR_NO_FILE;
    }

    struct jpeg_decompress_struct cinfo;
    jpeg_error_mgr_ex_t jerr;

    cinfo.err = jpeg_std_error(&jerr.pub);
    jerr.pub.error_exit = jpeg_error_exit_ex;

    if (setjmp(jerr.setjmp_buf)) {
        jpeg_destroy_decompress(&cinfo);
        fclose(f);
        jpeg_decoder_deinit(false);
        return JPEG_ERR_BAD_FILE;
    }

    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, f);
    jpeg_read_header(&cinfo, TRUE);

    /* Pick the best DCT scale: start at full resolution (denom=1) and step up
     * (1/2, 1/4, 1/8) only if the image won't fit in max_width x max_height.
     * This gives the highest quality decode that still fits. */
    cinfo.scale_num   = 1;
    cinfo.scale_denom = 1;
    for (int denom = 1; denom <= 8; denom *= 2) {
        cinfo.scale_denom = denom;
        jpeg_calc_output_dimensions(&cinfo);
        if ((int)cinfo.output_width  <= max_width &&
            (int)cinfo.output_height <= max_height) {
            break;
        }
    }

    /* Use fast integer DCT — fine quality for cover art on N64 */
    cinfo.dct_method = JDCT_IFAST;

    jpeg_start_decompress(&cinfo);

    int src_w = (int)cinfo.output_width;
    int src_h = (int)cinfo.output_height;
    int comps = (int)cinfo.output_components;

    /* Scale to fit within max_width x max_height, preserving aspect ratio */
    int dst_w = src_w;
    int dst_h = src_h;
    if (dst_w > max_width || dst_h > max_height) {
        if (src_w * max_height >= src_h * max_width) {
            dst_w = max_width;
            dst_h = (src_h * max_width) / src_w;
        } else {
            dst_h = max_height;
            dst_w = (src_w * max_height) / src_h;
        }
    }
    if (dst_w < 1) dst_w = 1;
    if (dst_h < 1) dst_h = 1;

    /* Check heap before allocating */
    size_t row_buf_size = (size_t)src_w * comps;
    size_t surf_size    = (size_t)dst_w * dst_h * 2;
    heap_stats_t heap;
    sys_get_heap_stats(&heap);
    if (row_buf_size + surf_size + 64 * 1024 > (size_t)(heap.total - heap.used)) {
        jpeg_abort_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
        fclose(f);
        jpeg_decoder_deinit(false);
        return JPEG_ERR_OUT_OF_MEM;
    }

    decoder->image = calloc(1, sizeof(surface_t));
    if (!decoder->image) {
        jpeg_abort_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
        fclose(f);
        jpeg_decoder_deinit(false);
        return JPEG_ERR_OUT_OF_MEM;
    }

    *decoder->image = surface_alloc(FMT_RGBA16, dst_w, dst_h);
    if (!decoder->image->buffer) {
        jpeg_abort_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
        fclose(f);
        jpeg_decoder_deinit(true);
        return JPEG_ERR_OUT_OF_MEM;
    }

    JSAMPROW row_buf = malloc(row_buf_size);
    if (!row_buf) {
        jpeg_abort_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
        fclose(f);
        jpeg_decoder_deinit(true);
        return JPEG_ERR_OUT_OF_MEM;
    }

    int scan_y = 0;
    while ((int)cinfo.output_scanline < src_h) {
        jpeg_read_scanlines(&cinfo, &row_buf, 1);
        int dst_y = (scan_y * dst_h) / src_h;
        uint16_t *dst_row = (uint16_t *)((uint8_t *)decoder->image->buffer +
                                         dst_y * decoder->image->stride);
        for (int dst_x = 0; dst_x < dst_w; dst_x++) {
            int src_x = (dst_x * src_w) / dst_w;
            uint8_t *p = row_buf + src_x * comps;
            uint8_t r = p[0];
            uint8_t g = (comps > 1) ? p[1] : r;
            uint8_t b = (comps > 2) ? p[2] : r;
            dst_row[dst_x] = ((r >> 3) << 11) | ((g >> 3) << 6) | ((b >> 3) << 1) | 1;
        }
        scan_y++;
    }

    free(row_buf);
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(f);

    decoder->result = JPEG_OK;
    decoder->done   = true;
    return JPEG_OK;
}

void jpeg_decoder_abort (void) {
    jpeg_decoder_deinit(true);
}

float jpeg_decoder_get_progress (void) {
    if (!decoder) return 0.0f;
    return decoder->done ? 1.0f : 0.0f;
}

void jpeg_decoder_poll (void) {
    if (!decoder || !decoder->done) return;

    jpeg_err_t result  = decoder->result;
    surface_t *image   = (result == JPEG_OK) ? decoder->image : NULL;
    decoder->callback(result, image, decoder->callback_data);
    jpeg_decoder_deinit(false);
}
