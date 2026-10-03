#include <stdio.h>
#include <stdlib.h>

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

#define ED64_VSERIES_FPGA_IMAGE_MAX_SIZE   (KiB(128))
#define ED64_VSERIES_FPGA_CONFIG_RETRIES   (100)

/* Decode the cart firmware's (control, data) byte-pair RLE stream used for its FPGA images:
 * control==0 terminates; control==1 means data is the length of a following uncompressed
 * run (copy verbatim); any other control value is a repeat count for a single data byte. */
static size_t ed64_vseries_ll_rle_decompress (const uint8_t *src, uint8_t *dst) {
    size_t size = 0;

    while (true) {
        uint8_t control = *src++;
        uint8_t data = *src++;

        if (control == 0) {
            break;
        } else if (control == 1) {
            for (uint8_t i = 0; i < data; i++) {
                *dst++ = *src++;
            }
            size += data;
        } else {
            for (uint8_t i = 0; i < control; i++) {
                *dst++ = data;
            }
            size += control;
        }
    }

    return size;
}

bool ed64_vseries_ll_configure_fpga (const char *image_path) {
    uint8_t *compressed = malloc(ED64_VSERIES_FPGA_IMAGE_MAX_SIZE);
    uint8_t *image = malloc(ED64_VSERIES_FPGA_IMAGE_MAX_SIZE);
    if (!compressed || !image) {
        free(compressed);
        free(image);
        return false;
    }

    FILE *file = fopen(image_path, "rb");
    if (!file) {
        free(compressed);
        free(image);
        return false;
    }
    size_t compressed_size = fread(compressed, 1, ED64_VSERIES_FPGA_IMAGE_MAX_SIZE, file);
    fclose(file);
    if (compressed_size == 0) {
        free(compressed);
        free(image);
        return false;
    }

    size_t size = ed64_vseries_ll_rle_decompress(compressed, image);
    free(compressed);
    size = ALIGN(size, 2); // Config port transfers whole 16-bit words only.

    io_write(ED64_VSERIES_REG_CFG, 0);
    io_write(ED64_VSERIES_REG_FPGA_CNT, 0);
    wait_ms(5);
    io_write(ED64_VSERIES_REG_FPGA_CNT, 1);
    wait_ms(5);

    for (size_t offset = 0; offset < size; offset += 2) {
        uint16_t word = (image[offset] << 8) | image[offset + 1];
        io_write(ED64_VSERIES_REG_FPGA_DATA, word);
    }
    for (int i = 0; i < 512; i++) {
        io_write(ED64_VSERIES_REG_FPGA_DATA, 0xFFFF);
    }

    bool configured = false;
    for (int attempt = 0; attempt < ED64_VSERIES_FPGA_CONFIG_RETRIES; attempt++) {
        io_write(ED64_VSERIES_REG_KEY, ED64_VSERIES_KEY_UNLOCK);

        if (ed64_bios_get_cart_id() != ED64_CART_ID_UNKNOWN) {
            configured = true;
            break;
        }
        wait_ms(1);
    }

    // Restore the SDRAM window regardless of outcome; ROM/save access depends on it.
    io_write(ED64_VSERIES_REG_CFG, ED64_VSERIES_CFG_SDRAM_ON);

    free(image);
    return configured;
}
