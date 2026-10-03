#include <stdio.h>

#include <libdragon.h>

#include "utils/utils.h"
#include "ed64_bios_ll.h"
#include "ed64_vseries_ll.h"

/* V series CPLD config register bank; physically the same bank libdragon's libcart
 * ed_init() already unlocks/uses (see libdragon/src/libcart/cart.c, ED_BASE_REG). */
#define ED64_VSERIES_REG_BASE          (0x08040000)
#define ED64_VSERIES_REG_CFG           (ED64_VSERIES_REG_BASE + 0x00)
#define ED64_VSERIES_REG_KEY           (ED64_VSERIES_REG_BASE + 0x20)
#define ED64_VSERIES_REG_FPGA_CNT      (ED64_VSERIES_REG_BASE + 0x40)
#define ED64_VSERIES_REG_FPGA_DATA     (ED64_VSERIES_REG_BASE + 0x44)

#define ED64_VSERIES_CFG_SDRAM_ON      (1 << 0)
#define ED64_VSERIES_KEY_UNLOCK        (0x1234)

#define ED64_VSERIES_FPGA_CONFIG_RETRIES       (100)
#define ED64_VSERIES_FPGA_COMPRESSED_MAX_SIZE   (KiB(64)) // Largest known file is ~57 KiB.

/* Reused across V2/V3 bitstreams (only one is ever loaded per boot). Static rather than
 * malloc'd: a deterministic, one-time BSS reservation that can't fail or fragment at
 * runtime, unlike a transient heap allocation - real hardware testing showed a prior
 * version using a pair of malloc'd ~128 KiB compressed+decompressed buffers here could
 * exhaust memory badly enough to crash unrelated later allocations (e.g. menu sprite/font
 * loading). */
static uint8_t ed64_vseries_ll_compressed_buffer[ED64_VSERIES_FPGA_COMPRESSED_MAX_SIZE];

/* Streams a (control, data) byte-pair RLE file straight into the 16-bit config port word
 * by word, decoding from the fully-read-in compressed buffer in a tight loop so there's no
 * file I/O between register writes - the CPLD's config port expects a steady stream once
 * started. control==0 terminates; control==1 means data is the length of a following
 * uncompressed run (copy verbatim); any other control value is a repeat count for a
 * single data byte. */
static bool ed64_vseries_ll_stream_image (const char *image_path) {
    FILE *file = fopen(image_path, "rb");
    if (!file) {
        debugf("ed64v: %s: fopen failed\n", image_path);
        return false;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    if ((file_size <= 0) || ((size_t) (file_size) > sizeof(ed64_vseries_ll_compressed_buffer))) {
        debugf("ed64v: %s: bad file size %ld\n", image_path, file_size);
        fclose(file);
        return false;
    }

    size_t compressed_size = fread(ed64_vseries_ll_compressed_buffer, 1, (size_t) (file_size), file);
    fclose(file);
    if (compressed_size != (size_t) (file_size)) {
        debugf("ed64v: %s: short read %u of %ld\n", image_path, (unsigned) (compressed_size), file_size);
        return false;
    }

    const uint8_t *src = ed64_vseries_ll_compressed_buffer;
    const uint8_t *end = src + compressed_size;
    uint8_t word_buffer[2];
    size_t word_fill = 0;

    while ((src < end) && (*src != 0)) {
        uint8_t control = *src++;
        if (src >= end) {
            return false;
        }
        uint8_t data = *src++;

        int count = (control == 1) ? data : control;
        for (int i = 0; i < count; i++) {
            if ((control == 1) && (src >= end)) {
                return false;
            }
            word_buffer[word_fill++] = (control == 1) ? *src++ : data;
            if (word_fill == sizeof(word_buffer)) {
                io_write(ED64_VSERIES_REG_FPGA_DATA, (word_buffer[0] << 8) | word_buffer[1]);
                word_fill = 0;
            }
        }
    }

    // Config port transfers whole 16-bit words only; pad a trailing odd byte if needed.
    if (word_fill == 1) {
        io_write(ED64_VSERIES_REG_FPGA_DATA, (word_buffer[0] << 8) | 0xFF);
    }

    return true;
}

bool ed64_vseries_ll_configure_fpga (const char *image_path) {
    io_write(ED64_VSERIES_REG_CFG, 0);
    io_write(ED64_VSERIES_REG_FPGA_CNT, 0);
    wait_ms(5);
    io_write(ED64_VSERIES_REG_FPGA_CNT, 1);
    wait_ms(5);

    bool streamed = ed64_vseries_ll_stream_image(image_path);

    for (int i = 0; i < 512; i++) {
        io_write(ED64_VSERIES_REG_FPGA_DATA, 0xFFFF);
    }

    bool configured = false;
    if (streamed) {
        for (int attempt = 0; attempt < ED64_VSERIES_FPGA_CONFIG_RETRIES; attempt++) {
            io_write(ED64_VSERIES_REG_KEY, ED64_VSERIES_KEY_UNLOCK);

            if (ed64_bios_get_cart_id() != ED64_CART_ID_UNKNOWN) {
                configured = true;
                break;
            }
            wait_ms(1);
        }
    }

    // Restore the SDRAM window regardless of outcome; ROM/save access depends on it.
    io_write(ED64_VSERIES_REG_CFG, ED64_VSERIES_CFG_SDRAM_ON);

    if (!configured) {
        debugf("ed64v: %s: cart id never became recognizable after reconfigure\n", image_path);
    }
    return configured;
}
