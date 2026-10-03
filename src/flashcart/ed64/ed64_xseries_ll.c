#include <stdio.h>
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
#define ED64_XSERIES_REG_SYS_CFG     (ED64_XSERIES_REG_BASE + 0x8000)
#define ED64_XSERIES_REG_KEY         (ED64_XSERIES_REG_BASE + 0x8004)

#define ED64_XSERIES_FPGA_CFG_NCFG   (1 << 0)
#define ED64_XSERIES_FPGA_STA_CDON   (1 << 0)

#define ED64_XSERIES_SYS_CFG_SDRAM_ON   (0)
#define ED64_XSERIES_KEY_UNLOCK          (0xAA55)

#define ED64_XSERIES_ICE_CFG_SS      (1 << 0)
#define ED64_XSERIES_ICE_CFG_RST     (1 << 1)
#define ED64_XSERIES_ICE_CFG_ACT     (1 << 7)
#define ED64_XSERIES_ICE_STA_CDN     (1 << 0)

#define ED64_XSERIES_EDID_SIGNATURE  (0xED640000)

/* N64 PI controller's own bus-timing registers (not cartridge-domain, so not reachable
 * via io_read/io_write - see libdragon's dma.h PI_* macros for the sibling registers). */
#define PI_BSD_DOM1_LAT_REG          ((volatile uint32_t *) (0xA4600014))
#define PI_BSD_DOM1_PWD_REG          ((volatile uint32_t *) (0xA4600018))

/* Official EverDrive OS post-reconfigure timings (its own bi_init(), called right after
 * its edConfigureFpga(), sets these two unconditionally rather than restoring whatever
 * was there before) - reconfiguring the main FPGA needs its own ROM-read timing, not
 * whatever generic timing the N64's IPL3/CIC boot left PI DOM1 at. */
#define ED64_XSERIES_PI_DOM1_LAT_POST_RECONFIG   (0x04)
#define ED64_XSERIES_PI_DOM1_PWD_POST_RECONFIG   (0x0C)

#define ED64_XSERIES_FPGA_CONFIG_RETRIES       (2048)
#define ED64_XSERIES_ICE_COMPRESSED_MAX_SIZE    (KiB(16)) // Known file is ~14 KiB.
#define ED64_XSERIES_MAIN_COMPRESSED_MAX_SIZE    (KiB(80)) // Known file is ~70 KiB.

/* Both bitstreams are read fully into memory BEFORE any FPGA reconfiguration begins -
 * matching the official firmware's edConfigureFpga(), which decompresses both from
 * statically-embedded arrays up front, with no file I/O interleaved with the hardware
 * register pokes at all. Real hardware testing showed reconfiguring the ICE40 (which
 * hosts the rom:/ read engine) leaves it briefly unready, so a rom:/ fopen() for the main
 * bitstream placed *after* the ICE reconfigure step (as a naive file-by-file loader would
 * do) reliably fails. Static rather than malloc'd for the same reason as before: a
 * deterministic, one-time BSS reservation that can't fail or fragment at runtime. */
static uint8_t ed64_xseries_ll_ice_buffer[ED64_XSERIES_ICE_COMPRESSED_MAX_SIZE];
static uint8_t ed64_xseries_ll_main_buffer[ED64_XSERIES_MAIN_COMPRESSED_MAX_SIZE];

/* The official firmware accesses every FPGA command register (not just bulk bitstream/
 * filler data) through a real PI DMA transfer rather than a direct io_write()/io_read()
 * poke - see its bi_reg_wr()/bi_reg_rd(), both of which go through sysPI_wr()/sysPI_rd().
 * A direct poke to this register bank isn't reliably honored by the hardware. */
static void ed64_xseries_ll_reg_write (uint32_t address, uint32_t value) {
    uint32_t aligned_value __attribute__((aligned(8))) = value;
    pi_dma_write_data(&aligned_value, (void *) (address), sizeof(aligned_value));
}

static uint32_t ed64_xseries_ll_reg_read (uint32_t address) {
    uint32_t aligned_value __attribute__((aligned(8)));
    pi_dma_read_data((void *) (address), &aligned_value, sizeof(aligned_value));
    return aligned_value;
}

/* Streams a (control, data) byte-pair RLE file (see ed64_vseries_ll.c for the format
 * description) straight to a PI-bus config port in small fixed-size bursts, decoding from
 * the fully-read-in compressed buffer in a tight loop so there's no file I/O between
 * bursts - the FPGA's config port expects a steady stream once started. */
typedef struct {
    uint32_t port;
    uint8_t buffer[512] __attribute__((aligned(8)));
    size_t fill;
} ed64_xseries_ll_stream_t;

static void ed64_xseries_ll_stream_flush (ed64_xseries_ll_stream_t *stream) {
    if (stream->fill == 0) {
        return;
    }
    size_t flush_size = ALIGN(stream->fill, 2);
    while (stream->fill < flush_size) {
        stream->buffer[stream->fill++] = 0xFF;
    }
    pi_dma_write_data(stream->buffer, (void *) (stream->port), flush_size);
    stream->fill = 0;
}

static void ed64_xseries_ll_stream_put (ed64_xseries_ll_stream_t *stream, uint8_t byte) {
    stream->buffer[stream->fill++] = byte;
    if (stream->fill == sizeof(stream->buffer)) {
        ed64_xseries_ll_stream_flush(stream);
    }
}

static bool ed64_xseries_ll_load_file (const char *path, uint8_t *buffer, size_t buffer_size, size_t *out_size) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        debugf("ed64x: %s: fopen failed\n", path);
        return false;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    if ((file_size <= 0) || ((size_t) (file_size) > buffer_size)) {
        debugf("ed64x: %s: bad file size %ld\n", path, file_size);
        fclose(file);
        return false;
    }

    size_t read_size = fread(buffer, 1, (size_t) (file_size), file);
    fclose(file);
    if (read_size != (size_t) (file_size)) {
        debugf("ed64x: %s: short read %u of %ld\n", path, (unsigned) (read_size), file_size);
        return false;
    }

    *out_size = read_size;
    return true;
}

static bool ed64_xseries_ll_stream_buffer (const uint8_t *buffer, size_t size, uint32_t port) {
    const uint8_t *src = buffer;
    const uint8_t *end = src + size;
    ed64_xseries_ll_stream_t stream = { .port = port, .fill = 0 };

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
            ed64_xseries_ll_stream_put(&stream, (control == 1) ? *src++ : data);
        }
    }

    ed64_xseries_ll_stream_flush(&stream);
    return true;
}


static bool ed64_xseries_ll_configure_ice (const uint8_t *buffer, size_t size) {
    uint16_t old_boot_cfg = ed64_xseries_ll_reg_read(ED64_XSERIES_REG_BOOT_CFG) & 0xFFFF;

    ed64_xseries_ll_reg_write(ED64_XSERIES_REG_ICE_CFG, ED64_XSERIES_ICE_CFG_ACT | ED64_XSERIES_ICE_CFG_RST | ED64_XSERIES_ICE_CFG_SS);
    wait_ms(5);
    ed64_xseries_ll_reg_write(ED64_XSERIES_REG_ICE_CFG, ED64_XSERIES_ICE_CFG_ACT);
    wait_ms(5);
    ed64_xseries_ll_reg_write(ED64_XSERIES_REG_ICE_CFG, ED64_XSERIES_ICE_CFG_ACT | ED64_XSERIES_ICE_CFG_RST);
    wait_ms(5);

    if (!ed64_xseries_ll_stream_buffer(buffer, size, ED64_XSERIES_REG_ICE_DATA)) {
        return false;
    }

    // 49 dummy clocks are required after the bitstream for the ICE40 to latch it in.
    uint8_t filler[64] __attribute__((aligned(8)));
    memset(filler, 0xFF, sizeof(filler));
    pi_dma_write_data(filler, (void *) (ED64_XSERIES_REG_ICE_DATA), sizeof(filler));

    ed64_xseries_ll_reg_write(ED64_XSERIES_REG_ICE_CFG, ED64_XSERIES_ICE_CFG_RST | ED64_XSERIES_ICE_CFG_SS);
    wait_ms(5);

    bool configured = (ed64_xseries_ll_reg_read(ED64_XSERIES_REG_ICE_CFG) & ED64_XSERIES_ICE_STA_CDN) != 0;

    // The ICE40 hosts the cart's foundational, always-on register block (EDID/cart-id,
    // boot-config, timer, I2C) - the official OS reads EDID *before* deciding whether to
    // reconfigure anything at all, and reads/restores boot-config around this exact
    // sequence, which only makes sense if reconfiguring the ICE wipes its own register
    // state back to the bitstream's power-on defaults. The EDID rewrite is needed for the
    // same reason: the official source's own comment on this line ("init serial core")
    // says it also kicks the ICE's serial (SD card SPI) engine back into a working state,
    // not just the cart-id readback. Re-arm both explicitly rather than relying on
    // whatever the fresh bitstream happens to reset them to.
    ed64_xseries_ll_reg_write(ED64_XSERIES_REG_EDID, ED64_XSERIES_EDID_SIGNATURE);
    ed64_xseries_ll_reg_write(ED64_XSERIES_REG_BOOT_CFG, old_boot_cfg);

    if (!configured) {
        debugf("ed64x: ice config done bit never set\n");
    }
    return configured;
}

static bool ed64_xseries_ll_configure_main (const uint8_t *buffer, size_t size) {
    ed64_xseries_ll_reg_write(ED64_XSERIES_REG_FPGA_CFG, 0);
    wait_ms(5);
    ed64_xseries_ll_reg_write(ED64_XSERIES_REG_FPGA_CFG, ED64_XSERIES_FPGA_CFG_NCFG);
    wait_ms(5);

    if (!ed64_xseries_ll_stream_buffer(buffer, size, ED64_XSERIES_REG_FPGA_DATA)) {
        return false;
    }

    uint8_t filler[256] __attribute__((aligned(8)));
    memset(filler, 0xFF, sizeof(filler));

    bool configured = false;
    for (int attempt = 0; attempt < ED64_XSERIES_FPGA_CONFIG_RETRIES; attempt++) {
        if (ed64_xseries_ll_reg_read(ED64_XSERIES_REG_FPGA_CFG) & ED64_XSERIES_FPGA_STA_CDON) {
            configured = true;
            break;
        }
        pi_dma_write_data(filler, (void *) (ED64_XSERIES_REG_FPGA_DATA), sizeof(filler));
    }

    if (!configured) {
        debugf("ed64x: main config done bit never set\n");
    }
    return configured;
}

bool ed64_xseries_ll_configure_fpga (const char *ice_image_path, const char *main_image_path) {
    size_t ice_size, main_size;
    if (!ed64_xseries_ll_load_file(ice_image_path, ed64_xseries_ll_ice_buffer, sizeof(ed64_xseries_ll_ice_buffer), &ice_size)) {
        return false;
    }
    if (!ed64_xseries_ll_load_file(main_image_path, ed64_xseries_ll_main_buffer, sizeof(ed64_xseries_ll_main_buffer), &main_size)) {
        return false;
    }

    // Loading needs a faster PI DOM1 pulse width than normal ROM/SD access.
    dma_wait();
    *PI_BSD_DOM1_PWD_REG = 0x14;

    bool configured = ed64_xseries_ll_configure_ice(ed64_xseries_ll_ice_buffer, ice_size)
        && ed64_xseries_ll_configure_main(ed64_xseries_ll_main_buffer, main_size);

    // Reconfiguring the main FPGA resets the 0x8000+ register bank to its bitstream's
    // power-on defaults, including the key lock (own implementation, not a libcart call -
    // see KEY_UNLOCK/SYS_CFG_REG comment in the header) and the SDRAM/ROM window mapping
    // (OFF by default); unlock the bank then re-enable SDRAM regardless of outcome, or
    // every later rom:/ read breaks.
    ed64_xseries_ll_reg_write(ED64_XSERIES_REG_KEY, ED64_XSERIES_KEY_UNLOCK);
    ed64_xseries_ll_reg_write(ED64_XSERIES_REG_SYS_CFG, ED64_XSERIES_SYS_CFG_SDRAM_ON);

    // Restoring the PRE-reconfigure pulse width isn't right either: the reconfigured main
    // FPGA's own ROM-read logic needs its own specific timing, not whatever generic timing
    // the N64's IPL3/CIC boot left PI DOM1 at. Set the fixed values regardless of outcome.
    *PI_BSD_DOM1_LAT_REG = ED64_XSERIES_PI_DOM1_LAT_POST_RECONFIG;
    *PI_BSD_DOM1_PWD_REG = ED64_XSERIES_PI_DOM1_PWD_POST_RECONFIG;
    wait_ms(5);

    return configured;
}
