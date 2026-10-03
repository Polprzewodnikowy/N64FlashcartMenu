#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libdragon.h>

#include "utils/utils.h"
#include "../flashcart_utils.h"
#include "ed64_xseries_ll.h"

/* Shared FPGA command register bank (see ed64_bios_ll.c); the main/ICE40 config ports
 * live here too, at physical 0x1F800000. */
#define ED64_XSERIES_REG_BASE        (0x1F800000)
#define ED64_XSERIES_REG_FPGA_CFG    (ED64_XSERIES_REG_BASE + 0x0000)
#define ED64_XSERIES_REG_FPGA_DATA   (ED64_XSERIES_REG_BASE + 0x0200)
#define ED64_XSERIES_REG_BOOT_CFG    (ED64_XSERIES_REG_BASE + 0x0010)
#define ED64_XSERIES_REG_EDID        (ED64_XSERIES_REG_BASE + 0x0014)
#define ED64_XSERIES_REG_ICE_CFG     (ED64_XSERIES_REG_BASE + 0x801C)
#define ED64_XSERIES_REG_ICE_DATA    (ED64_XSERIES_REG_BASE + 0x8400)

#define ED64_XSERIES_FPGA_CFG_NCFG   (1 << 0)
#define ED64_XSERIES_FPGA_STA_CDON   (1 << 0)

#define ED64_XSERIES_ICE_CFG_SS      (1 << 0)
#define ED64_XSERIES_ICE_CFG_RST     (1 << 1)
#define ED64_XSERIES_ICE_CFG_ACT     (1 << 7)
#define ED64_XSERIES_ICE_STA_CDN     (1 << 0)

#define ED64_XSERIES_EDID_SIGNATURE  (0xED640000)

/* N64 PI controller's own bus-timing register (not cartridge-domain, so not reachable
 * via io_read/io_write - see libdragon's dma.h PI_* macros for the sibling registers). */
#define PI_BSD_DOM1_PWD_REG          ((volatile uint32_t *) (0xA4600018))

#define ED64_XSERIES_FPGA_IMAGE_MAX_SIZE   (KiB(256))
#define ED64_XSERIES_FPGA_CONFIG_RETRIES   (2048)

/* Same (control, data) byte-pair RLE stream as V series; see ed64_vseries_ll.c for the
 * format description. Kept as a separate copy since the two series have no other shared
 * low level file, and this is a tiny, self-contained routine. */
static size_t ed64_xseries_ll_rle_decompress (const uint8_t *src, uint8_t *dst) {
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

static bool ed64_xseries_ll_load_image (const char *image_path, uint8_t *image, size_t *out_size) {
    uint8_t *compressed = malloc(ED64_XSERIES_FPGA_IMAGE_MAX_SIZE);
    if (!compressed) {
        return false;
    }

    FILE *file = fopen(image_path, "rb");
    if (!file) {
        free(compressed);
        return false;
    }
    size_t compressed_size = fread(compressed, 1, ED64_XSERIES_FPGA_IMAGE_MAX_SIZE, file);
    fclose(file);
    if (compressed_size == 0) {
        free(compressed);
        return false;
    }

    size_t size = ed64_xseries_ll_rle_decompress(compressed, image);
    free(compressed);
    *out_size = ALIGN(size, 8); // Kept word-aligned for the bulk PI DMA transfer below.

    return true;
}

static void ed64_xseries_ll_stream_to_port (uint32_t port, uint8_t *image, size_t size) {
    for (size_t offset = 0; offset < size; ) {
        size_t block_size = MIN(size - offset, 512);
        pi_dma_write_data(&image[offset], (void *) (port), block_size);
        offset += block_size;
    }
}

static bool ed64_xseries_ll_configure_ice (const char *image_path) {
    uint8_t *image = malloc(ED64_XSERIES_FPGA_IMAGE_MAX_SIZE);
    if (!image) {
        return false;
    }
    size_t size;

    if (!ed64_xseries_ll_load_image(image_path, image, &size)) {
        free(image);
        return false;
    }

    uint16_t old_boot_cfg = io_read(ED64_XSERIES_REG_BOOT_CFG) & 0xFFFF;

    io_write(ED64_XSERIES_REG_ICE_CFG, ED64_XSERIES_ICE_CFG_ACT | ED64_XSERIES_ICE_CFG_RST | ED64_XSERIES_ICE_CFG_SS);
    wait_ms(5);
    io_write(ED64_XSERIES_REG_ICE_CFG, ED64_XSERIES_ICE_CFG_ACT);
    wait_ms(5);
    io_write(ED64_XSERIES_REG_ICE_CFG, ED64_XSERIES_ICE_CFG_ACT | ED64_XSERIES_ICE_CFG_RST);
    wait_ms(5);

    ed64_xseries_ll_stream_to_port(ED64_XSERIES_REG_ICE_DATA, image, size);
    free(image);

    // 49 dummy clocks are required after the bitstream for the ICE40 to latch it in.
    uint8_t filler[64] __attribute__((aligned(8)));
    memset(filler, 0xFF, sizeof(filler));
    pi_dma_write_data(filler, (void *) (ED64_XSERIES_REG_ICE_DATA), sizeof(filler));

    io_write(ED64_XSERIES_REG_ICE_CFG, ED64_XSERIES_ICE_CFG_RST | ED64_XSERIES_ICE_CFG_SS);
    wait_ms(5);

    bool configured = (io_read(ED64_XSERIES_REG_ICE_CFG) & ED64_XSERIES_ICE_STA_CDN) != 0;

    // The ICE40 hosts the cart's foundational, always-on register block (EDID/cart-id,
    // boot-config, timer, I2C) - the official OS reads EDID *before* deciding whether to
    // reconfigure anything at all, and reads/restores boot-config around this exact
    // sequence, which only makes sense if reconfiguring the ICE wipes its own register
    // state back to the bitstream's power-on defaults. The EDID rewrite is needed for the
    // same reason: the official source's own comment on this line ("init serial core")
    // says it also kicks the ICE's serial (SD card SPI) engine back into a working state,
    // not just the cart-id readback. Re-arm both explicitly rather than relying on
    // whatever the fresh bitstream happens to reset them to.
    io_write(ED64_XSERIES_REG_EDID, ED64_XSERIES_EDID_SIGNATURE);
    io_write(ED64_XSERIES_REG_BOOT_CFG, old_boot_cfg);

    return configured;
}

static bool ed64_xseries_ll_configure_main (const char *image_path) {
    uint8_t *image = malloc(ED64_XSERIES_FPGA_IMAGE_MAX_SIZE);
    if (!image) {
        return false;
    }
    size_t size;

    if (!ed64_xseries_ll_load_image(image_path, image, &size)) {
        free(image);
        return false;
    }

    io_write(ED64_XSERIES_REG_FPGA_CFG, 0);
    wait_ms(5);
    io_write(ED64_XSERIES_REG_FPGA_CFG, ED64_XSERIES_FPGA_CFG_NCFG);
    wait_ms(5);

    ed64_xseries_ll_stream_to_port(ED64_XSERIES_REG_FPGA_DATA, image, size);
    free(image);

    uint8_t filler[256] __attribute__((aligned(8)));
    memset(filler, 0xFF, sizeof(filler));

    bool configured = false;
    for (int attempt = 0; attempt < ED64_XSERIES_FPGA_CONFIG_RETRIES; attempt++) {
        if (io_read(ED64_XSERIES_REG_FPGA_CFG) & ED64_XSERIES_FPGA_STA_CDON) {
            configured = true;
            break;
        }
        pi_dma_write_data(filler, (void *) (ED64_XSERIES_REG_FPGA_DATA), sizeof(filler));
    }

    return configured;
}

bool ed64_xseries_ll_configure_fpga (const char *ice_image_path, const char *main_image_path) {
    // Loading needs a faster PI DOM1 pulse width than normal ROM/SD access; restore it
    // afterward regardless of outcome.
    dma_wait();
    uint32_t old_pulse_width = *PI_BSD_DOM1_PWD_REG;
    *PI_BSD_DOM1_PWD_REG = 0x14;

    bool configured = ed64_xseries_ll_configure_ice(ice_image_path) && ed64_xseries_ll_configure_main(main_image_path);

    *PI_BSD_DOM1_PWD_REG = old_pulse_width;
    wait_ms(5);

    return configured;
}
