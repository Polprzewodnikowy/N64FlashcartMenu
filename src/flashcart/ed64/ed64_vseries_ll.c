#include <stdio.h>

#include <libdragon.h>

#include "utils/utils.h"
#include "../flashcart_utils.h"
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

/* Shared FPGA command register bank (see ed64_bios_ll.c / ed64_xseries_ll.c) - EDID/cart-id
 * becomes readable here only once this bank is unlocked, exactly like X series. The
 * official OS's legacyInitFpga() calls bi_unlock_regs() (this same KEY+SYS_CFG write) on
 * every retry iteration while waiting for the cart id to become valid - not the legacy CPLD
 * key above, which only unlocks the legacy bank itself, not this separate shared one. */
#define ED64_VSERIES_SHARED_BANK_BASE      (0x1F800000)
#define ED64_VSERIES_SHARED_REG_SYS_CFG    (ED64_VSERIES_SHARED_BANK_BASE + 0x8000)
#define ED64_VSERIES_SHARED_REG_KEY        (ED64_VSERIES_SHARED_BANK_BASE + 0x8004)
#define ED64_VSERIES_SHARED_KEY_UNLOCK     (0xAA55)
#define ED64_VSERIES_SHARED_SYS_CFG_SDRAM_ON (0)

/* N64 PI controller's own bus-timing registers (not cartridge-domain; see the matching
 * comment in ed64_xseries_ll.c). The official OS's legacyBcfgSave()/legacyInit() both note
 * "less than 03 does not work for v2/v3" for this specific field - required for the legacy
 * CPLD's own register bank to respond at all, not just for the FPGA bitstream load. */
#define PI_BSD_DOM2_RLS_REG             ((volatile uint32_t *) (0xA4600030))

#define ED64_VSERIES_FPGA_CONFIG_RETRIES       (100)
#define ED64_VSERIES_FPGA_COMPRESSED_MAX_SIZE   (KiB(64)) // Largest known file is ~57 KiB.

/* Reused across V2/V3 bitstreams (only one is ever loaded per boot). Static rather than
 * malloc'd: a deterministic, one-time BSS reservation that can't fail or fragment at
 * runtime, unlike a transient heap allocation - real hardware testing showed a prior
 * version using a pair of malloc'd ~128 KiB compressed+decompressed buffers here could
 * exhaust memory badly enough to crash unrelated later allocations (e.g. menu sprite/font
 * loading). */
static uint8_t ed64_vseries_ll_compressed_buffer[ED64_VSERIES_FPGA_COMPRESSED_MAX_SIZE];

/* The official firmware accesses every legacy CPLD register (reg_wr_le()/reg_rd_le()) and
 * the shared FPGA command bank (bi_reg_wr()/bi_reg_rd()) through a real PI DMA transfer
 * rather than a direct io_write()/io_read() poke - matches the fix X series needed for its
 * own command register bank (a direct poke isn't reliably honored by either bank's
 * hardware). */
static void ed64_vseries_ll_reg_write (uint32_t address, uint32_t value) {
    uint32_t aligned_value __attribute__((aligned(8))) = value;
    pi_dma_write_data(&aligned_value, (void *) (address), sizeof(aligned_value));
}

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
                ed64_vseries_ll_reg_write(ED64_VSERIES_REG_FPGA_DATA, (word_buffer[0] << 8) | word_buffer[1]);
                word_fill = 0;
            }
        }
    }

    // Config port transfers whole 16-bit words only; pad a trailing odd byte if needed.
    if (word_fill == 1) {
        ed64_vseries_ll_reg_write(ED64_VSERIES_REG_FPGA_DATA, (word_buffer[0] << 8) | 0xFF);
    }

    return true;
}

bool ed64_vseries_ll_configure_fpga (const char *image_path) {
    // Required for the legacy CPLD bank to respond at all on V2/V3 (see the register
    // definition comment above) - save/restore around the whole reconfigure, matching the
    // official OS's own legacyInit()/legacyBcfgSave().
    uint32_t old_dom2_rls = *PI_BSD_DOM2_RLS_REG;
    *PI_BSD_DOM2_RLS_REG = 0x03;

    // Unlocks the legacy CPLD bank itself (separate from the shared bank unlocked in the
    // retry loop below) - matches the official OS's legacyInit(), which writes this before
    // ever touching REG_CFG.
    ed64_vseries_ll_reg_write(ED64_VSERIES_REG_KEY, ED64_VSERIES_KEY_UNLOCK);
    ed64_vseries_ll_reg_write(ED64_VSERIES_REG_CFG, 0);
    ed64_vseries_ll_reg_write(ED64_VSERIES_REG_FPGA_CNT, 0);
    wait_ms(5);
    ed64_vseries_ll_reg_write(ED64_VSERIES_REG_FPGA_CNT, 1);
    wait_ms(5);

    bool streamed = ed64_vseries_ll_stream_image(image_path);

    for (int i = 0; i < 512; i++) {
        ed64_vseries_ll_reg_write(ED64_VSERIES_REG_FPGA_DATA, 0xFFFF);
    }

    bool configured = false;
    if (streamed) {
        for (int attempt = 0; attempt < ED64_VSERIES_FPGA_CONFIG_RETRIES; attempt++) {
            // Unlock the SHARED bank that ed64_bios_get_cart_id() actually reads EDID from -
            // not the legacy CPLD key again (see the register definition comment above).
            ed64_vseries_ll_reg_write(ED64_VSERIES_SHARED_REG_KEY, ED64_VSERIES_SHARED_KEY_UNLOCK);
            ed64_vseries_ll_reg_write(ED64_VSERIES_SHARED_REG_SYS_CFG, ED64_VSERIES_SHARED_SYS_CFG_SDRAM_ON);

            if (ed64_bios_get_cart_id() != ED64_CART_ID_UNKNOWN) {
                configured = true;
                break;
            }
            wait_ms(1);
        }
    }

    // Restore the SDRAM window regardless of outcome; ROM/save access depends on it.
    ed64_vseries_ll_reg_write(ED64_VSERIES_REG_CFG, ED64_VSERIES_CFG_SDRAM_ON);
    *PI_BSD_DOM2_RLS_REG = old_dom2_rls;

    if (!configured) {
        debugf("ed64v: %s: cart id never became recognizable after reconfigure\n", image_path);
    }
    return configured;
}
