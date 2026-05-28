/**
 * @file igr.c
 * @brief In-Game Reset (IGR) payload implementation
 * @ingroup boot
 */

#include "igr.h"
#include "vr4300_asm.h"

/**
 * @brief Button combo mask for IGR trigger.
 *
 * Uses libultra OSContPad button bitmap:
 *   A = 0x8000, B = 0x4000, Z = 0x2000, Start = 0x1000
 *   L = 0x0020, R = 0x0010
 * Combined: L + R + Z + A + B + Start = 0xF030
 */
#define IGR_BUTTON_MASK     (0xF030)

/**
 * @brief Uncached KSEG1 address for controller 1 button data in PIF RAM.
 *
 * PIF RAM base (uncached): 0xBFC007C0
 * Controller 1 buttons are assumed at offsets 4 (high) and 5 (low).
 * NOTE: This offset may need adjustment based on the game's PIF layout.
 */
#define PIF_RAM_BUTTONS_HIGH_ADDR   (0xBFC007C4)
#define PIF_RAM_BUTTONS_LOW_ADDR    (0xBFC007C5)

/**
 * @brief SC64 AUX register uncached KSEG1 address.
 */
#define SC64_AUX_ADDRESS    (0xBFFF0018)

/**
 * @brief SC64 AUX reboot value.
 */
#define SC64_AUX_REBOOT     (0xFF000002)

void igr_append_payload (io32_t **engine_p) {
    io32_t *p = *engine_p;

    /**
     * IGR Payload — runs every exception, checks VI interrupt,
     * polls controller combo, triggers SC64 soft reboot.
     *
     * Register usage: $k0, $k1 only (kernel regs, safe in exception handler).
     */

    /* Check if this is an interrupt (exception code 0 in Cause[6:2]) */
    *p++ = I_MFC0(REG_K0, C0_REG_CAUSE);
    *p++ = I_ANDI(REG_K1, REG_K0, 0x7C);
    /* If not an interrupt, skip to not_vi (offset = 22) */
    *p++ = I_BNE(REG_K1, REG_ZERO, 22);
    /* Delay slot: check IP0 bit (RCP/Int0 — MI interrupts) */
    *p++ = I_ANDI(REG_K1, REG_K0, 0x0100);

    /* If IP0 not set, skip to not_vi (offset = 20) */
    *p++ = I_BEQ(REG_K1, REG_ZERO, 20);
    *p++ = I_NOP();

    /* Check MI_INTR_REG for VI specifically (bit 3) */
    *p++ = I_LUI(REG_K0, A_BASE(0x04300008));
    *p++ = I_LW(REG_K1, A_OFFSET(0x04300008), REG_K0);
    *p++ = I_ANDI(REG_K0, REG_K1, 0x0008);
    /* If VI bit not set, skip to not_vi (offset = 15) */
    *p++ = I_BEQ(REG_K0, REG_ZERO, 15);
    *p++ = I_NOP();

    /* Read controller 1 buttons from PIF RAM (uncached) */
    *p++ = I_LUI(REG_K0, A_BASE(PIF_RAM_BUTTONS_HIGH_ADDR));
    *p++ = I_LBU(REG_K1, A_OFFSET(PIF_RAM_BUTTONS_HIGH_ADDR), REG_K0);
    *p++ = I_LBU(REG_K0, A_OFFSET(PIF_RAM_BUTTONS_LOW_ADDR), REG_K0);
    *p++ = I_SLL(REG_K1, REG_K1, 8);
    *p++ = I_OR(REG_K1, REG_K1, REG_K0);

    /* Mask and compare against combo */
    *p++ = I_ORI(REG_K0, REG_ZERO, IGR_BUTTON_MASK);
    *p++ = I_AND(REG_K1, REG_K1, REG_K0);
    /* If masked buttons != full mask, skip to not_vi (offset = 6) */
    *p++ = I_BNE(REG_K1, REG_K0, 6);
    *p++ = I_NOP();

    /* Combo matched — trigger SC64 soft reboot via AUX register */
    *p++ = I_LUI(REG_K0, A_BASE(SC64_AUX_ADDRESS));
    *p++ = I_LUI(REG_K1, (SC64_AUX_REBOOT >> 16));
    *p++ = I_ORI(REG_K1, REG_K1, (SC64_AUX_REBOOT & 0xFFFF));
    *p++ = I_SW(REG_K1, A_OFFSET(SC64_AUX_ADDRESS), REG_K0);
    *p++ = I_NOP();

    /* not_vi: fall through to caller's next instruction (J to relocated handler) */

    *engine_p = p;
}
