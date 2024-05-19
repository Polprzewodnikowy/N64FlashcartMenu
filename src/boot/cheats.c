#include <libdragon.h>

#include "boot_io.h"
#include "cheats.h"
#include "vr4300_asm.h"

#define HIT_INVALIDATE_I ((4 << 2) | 0)
#define HIT_WRITE_BACK_D ((6 << 2) | 1)

#define D_CACHE_LINE_SIZE (16)

#define CAUSE_IRQ_PRE_NMI (1 << 12)
#define CAUSE_EXC_CODE_MASK (0x7C)
#define CAUSE_EXC_CODE_WATCH (0x5C)

#define WATCHLO_W (1 << 0)

#define RELOCATED_EXCEPTION_HANDLER_ADDRESS (0x80000120)
#define EXCEPTION_HANDLER_ADDRESS (0x80000180)
#define PATCHER_ADDRESS (0x80700000)
#define ENGINE_TEMPORARY_ADDRESS (PATCHER_ADDRESS + 0x10000)
#define DEFAULT_ENGINE_ADDRESS (0x807C5C00)

typedef struct {
    uint8_t type;
    uint32_t address;
    uint16_t value;
} cheat_t;

typedef struct {
    cheat_t main;
    cheat_t sub;
} cheat_entry_t;

typedef enum {
    SPECIAL_DISABLE_EXPANSION_PAK = 0xEE,
    SPECIAL_WRITE_BYTE_ON_BOOT = 0xF0,
    SPECIAL_WRITE_SHORT_ON_BOOT = 0xF1,
    SPECIAL_SET_STORE_LOCATION = 0xFF,
} cheat_type_special_t;

#define IS_WIDTH_16(t) ((t) & (1 << 0))
#define IS_CONDITION_NOT_EQUAL(t) ((t) & (1 << 1))
#define IS_CONDITION_GS_BUTTON(t) ((t) & (1 << 3))

#define IS_TYPE_REPEATER(t) ((t) == 0x50)
#define IS_TYPE_WRITE(t) ((((t)&0xF0) == 0x80) || (((t)&0xF0) == 0xA0))
#define IS_TYPE_CONDITIONAL(t) (((t)&0xF0) == 0xD0)

#define IS_DOUBLE_ENTRY(t) (IS_TYPE_CONDITIONAL(t) || IS_TYPE_REPEATER(t))

static bool cheats_patch_ipl3 (cic_type_t cic_type, io32_t *target) {
    uint32_t patch_offset = 0;
    uint32_t j_instruction = I_J((uint32_t)(target));

    io32_t *ipl3 = SP_MEM->DMEM;

    switch (cic_type) {
    case CIC_5101: patch_offset = 476; break;
    case CIC_6101:
    case CIC_7102: patch_offset = 476; break;
    case CIC_x102: patch_offset = 475; break;
    case CIC_x103: patch_offset = 472; break;
    case CIC_x105: patch_offset = 499; break;
    case CIC_x106: patch_offset = 488; break;
    default: return true;
    }

    // NOTE: Check for "jr $t1" instruction
    //       Libdragon IPL3 could be brute-force signed with any retail
    //       CIC seed and checksum, and we support only retail libultra IPL3
    if (cpu_io_read(&ipl3[patch_offset]) != I_JR(REG_T1)) {
        return false;
    }

    switch (cic_type) {
    case CIC_x105:
        // NOTE: This disables game code checksum verification
        cpu_io_write(&ipl3[486], I_NOP());
        break;

    case CIC_x106:
        // NOTE: CIC x106 IPL3 is partially scrambled
        j_instruction ^= 0x8188764A;
        break;

    default: break;
    }

    cpu_io_write(&ipl3[patch_offset], j_instruction);

    return false;
}

static bool cheats_get_next (uint32_t **cheat_list, cheat_entry_t *cheat) {
    cheat_t *c = &cheat->main;
    cheat->sub.type = 0;

    for (int i = 0; i < 2; i++) {
        uint32_t raw[2] = {(*cheat_list)[0], (*cheat_list)[1]};

        (*cheat_list) += 2;

        if ((raw[0] == 0) && (raw[1] == 0)) {
            return false;
        }

        c->type = ((raw[0] >> 24) & 0xFF);
        c->address = (raw[0] & 0xA07FFFFF);
        c->value = (raw[1] & 0xFFFF);

        if (!IS_DOUBLE_ENTRY(c->type)) {
            break;
        }

        c = &cheat->sub;
    }

    return true;
}

static io32_t *cheats_get_engine_address (uint32_t *cheat_list) {
    cheat_entry_t cheat;
    while (cheats_get_next(&cheat_list, &cheat)) {
        if (cheat.main.type == SPECIAL_SET_STORE_LOCATION) {
            return (io32_t *)(cheat.main.address & 0x807FFFFF);
        }
    }
    return (io32_t *)(DEFAULT_ENGINE_ADDRESS);
}

static void cheats_update_cache (volatile void *start, volatile void *end) {
    data_cache_hit_writeback(start, (end - start));
    inst_cache_hit_invalidate(start, (end - start));
}

bool cheats_install (cic_type_t cic_type, uint32_t *cheat_list) {
    if (!cheat_list) {
        return false;
    }

    io32_t *engine_start = (io32_t *)(ENGINE_TEMPORARY_ADDRESS);
    io32_t *engine_p = engine_start;

    io32_t *patcher_start = (io32_t *)(PATCHER_ADDRESS);
    io32_t *patcher_p = patcher_start;

    if (cheats_patch_ipl3(cic_type, patcher_start)) {
        return false;
    }

    io32_t *final_engine_address = cheats_get_engine_address(cheat_list);

    // Original watch exception handler code written by Jay Oster 'Parasyte'
    // https://github.com/parasyte/alt64/blob/master/utils.c#L1024-L1054

    uint32_t ori_placeholder_instruction = I_ORI(REG_ZERO, REG_K0, A_OFFSET(RELOCATED_EXCEPTION_HANDLER_ADDRESS));
    uint32_t ori_placeholder_address = (uint32_t)(final_engine_address + 20);

    // Load cause register
    *engine_p++ = I_MFC0(REG_K0, C0_REG_CAUSE);

    // Disable watch exception when reset button is pressed
    *engine_p++ = I_ANDI(REG_K1, REG_K0, CAUSE_IRQ_PRE_NMI);
    *engine_p++ = I_BNEL(REG_K1, REG_ZERO, 1);
    *engine_p++ = I_MTC0(REG_ZERO, C0_REG_WATCH_LO);

    // Check if watch exception ocurred, if yes then proceed to relocate the game exception handler
    *engine_p++ = I_ANDI(REG_K0, REG_K0, CAUSE_EXC_CODE_MASK);
    *engine_p++ = I_ORI(REG_K1, REG_ZERO, CAUSE_EXC_CODE_WATCH);
    *engine_p++ = I_BNE(REG_K0, REG_K1, 15); // Skips to after the 'eret' instruction

    // Extract base register number from the store instruction
    *engine_p++ = I_MFC0(REG_K1, C0_REG_EPC);
    *engine_p++ = I_LW(REG_K1, 0, REG_K1);
    *engine_p++ = I_LUI(REG_K0, 0x03E0);
    *engine_p++ = I_AND(REG_K1, REG_K0, REG_K1);
    *engine_p++ = I_SRL(REG_K1, REG_K1, 5);

    // Update create final instruction and update its target register number
    *engine_p++ = I_LUI(REG_K0, ori_placeholder_instruction >> 16);
    *engine_p++ = I_ORI(REG_K0, REG_K0, ori_placeholder_instruction);
    *engine_p++ = I_OR(REG_K0, REG_K0, REG_K1);

    // Write created instruction into placeholder
    *engine_p++ = I_LUI(REG_K1, A_BASE(ori_placeholder_address));
    *engine_p++ = I_SW(REG_K0, A_OFFSET(ori_placeholder_address), REG_K1);

    // Force write and instruction cache invalidation
    *engine_p++ = I_CACHE(HIT_WRITE_BACK_D, A_OFFSET(ori_placeholder_address), REG_K1);
    *engine_p++ = I_CACHE(HIT_INVALIDATE_I, A_OFFSET(ori_placeholder_address), REG_K1);

    // Load address base and execute created instruction
    *engine_p++ = I_LUI(REG_K0, A_BASE(RELOCATED_EXCEPTION_HANDLER_ADDRESS));
    *engine_p++ = I_NOP();

    // Return from the exception
    *engine_p++ = I_ERET();

    cheat_entry_t cheat;

    while (cheats_get_next(&cheat_list, &cheat)) {
        cheat_t *c = &cheat.main;

        if (IS_TYPE_REPEATER(c->type)) {
            if ((!IS_TYPE_WRITE(cheat.sub.type)) || IS_CONDITION_GS_BUTTON(cheat.sub.type)) {
                continue;
            }

            int count = ((c->address >> 8) & 0xFF);
            int step = (c->address & 0xFF);
            int16_t increment = (int16_t)(c->value);

            c = &cheat.sub;

            for (int i = 0; i < count; i++) {
                *engine_p++ = I_LUI(REG_K0, A_BASE(c->address));
                *engine_p++ = I_ORI(REG_K1, REG_ZERO, c->value);
                *engine_p++ = IS_WIDTH_16(c->type) ? I_SH(REG_K1, A_OFFSET(c->address), REG_K0)
                                                   : I_SB(REG_K1, A_OFFSET(c->address), REG_K0);

                c->address += step;
                c->value += increment;
            }

            continue;
        }

        if (IS_TYPE_CONDITIONAL(c->type)) {
            if ((!IS_TYPE_WRITE(cheat.sub.type)) || IS_CONDITION_GS_BUTTON(cheat.sub.type)) {
                continue;
            }

            *engine_p++ = I_LUI(REG_K0, A_BASE(c->address));
            *engine_p++ = IS_WIDTH_16(c->type) ? I_LHU(REG_K0, A_OFFSET(c->address), REG_K0)
                                               : I_LBU(REG_K0, A_OFFSET(c->address), REG_K0);
            *engine_p++ = I_ORI(REG_K1, REG_ZERO, c->value & (IS_WIDTH_16(c->type) ? 0xFFFF : 0xFF));
            *engine_p++ = IS_CONDITION_NOT_EQUAL(c->type) ? I_BEQ(REG_K0, REG_K1, 3) : I_BNE(REG_K0, REG_K1, 3);

            c = &cheat.sub;
        }

        if (IS_TYPE_WRITE(c->type)) {
            if (IS_CONDITION_GS_BUTTON(c->type)) {
                continue;
            }

            *engine_p++ = I_LUI(REG_K0, A_BASE(c->address));
            *engine_p++ = I_ORI(REG_K1, REG_ZERO, c->value);
            *engine_p++ = IS_WIDTH_16(c->type) ? I_SH(REG_K1, A_OFFSET(c->address), REG_K0)
                                               : I_SB(REG_K1, A_OFFSET(c->address), REG_K0);

            continue;
        }

        switch (c->type) {
        case SPECIAL_WRITE_BYTE_ON_BOOT:
        case SPECIAL_WRITE_SHORT_ON_BOOT: {
            *patcher_p++ = I_LUI(REG_K0, A_BASE(c->address));
            *patcher_p++ = I_ORI(REG_K1, REG_ZERO, c->value);
            *patcher_p++ = IS_WIDTH_16(c->type) ? I_SH(REG_K1, A_OFFSET(c->address), REG_K0)
                                                : I_SB(REG_K1, A_OFFSET(c->address), REG_K0);
            break;
        }
        case SPECIAL_DISABLE_EXPANSION_PAK: {
            *patcher_p++ = I_LUI(REG_K0, 0xA000);
            *patcher_p++ = I_LUI(REG_K1, 0x0040);
            *patcher_p++ = I_SW(REG_K1, 0x318, REG_K0);
            *patcher_p++ = I_SW(REG_K1, 0x3F0, REG_K0);
            break;
        }
        default: break;
        }
    }

    *engine_p++ = I_J(RELOCATED_EXCEPTION_HANDLER_ADDRESS);
    *engine_p++ = I_NOP();

    uint32_t j_engine_from_handler = I_J((uint32_t)(final_engine_address));

    // Copy engine to the final location
    *patcher_p++ = I_LUI(REG_T3, A_BASE((uint32_t)(engine_start)));
    *patcher_p++ = I_ADDIU(REG_T3, REG_T3, A_OFFSET((uint32_t)(engine_start)));

    *patcher_p++ = I_LUI(REG_T4, A_BASE((uint32_t)(engine_p)));
    *patcher_p++ = I_ADDIU(REG_T4, REG_T4, A_OFFSET((uint32_t)(engine_p)));

    *patcher_p++ = I_LUI(REG_T5, A_BASE((uint32_t)(final_engine_address)));
    *patcher_p++ = I_ADDIU(REG_T5, REG_T5, A_OFFSET((uint32_t)(final_engine_address)));

    *patcher_p++ = I_ORI(REG_T6, REG_ZERO, 0);

    *patcher_p++ = I_LW(REG_K1, 0, REG_T3);
    *patcher_p++ = I_SW(REG_K1, 0, REG_T5);
    *patcher_p++ = I_ADDIU(REG_T3, REG_T3, 4);
    *patcher_p++ = I_ADDIU(REG_T5, REG_T5, 4);
    *patcher_p++ = I_BNE(REG_T3, REG_T4, -5);
    *patcher_p++ = I_ADDIU(REG_T6, REG_T6, 4);

    // Force write and invalidate instruction cache
    *patcher_p++ = I_LUI(REG_T5, A_BASE((uint32_t)(final_engine_address)));
    *patcher_p++ = I_ADDIU(REG_T5, REG_T5, A_OFFSET((uint32_t)(final_engine_address)));

    *patcher_p++ = I_CACHE(HIT_WRITE_BACK_D, 0, REG_T5);
    *patcher_p++ = I_CACHE(HIT_INVALIDATE_I, 0, REG_T5);
    *patcher_p++ = I_ADDIU(REG_T6, REG_T6, -D_CACHE_LINE_SIZE);
    *patcher_p++ = I_BGTZ(REG_T6, -4);
    *patcher_p++ = I_ADDIU(REG_T5, REG_T5, D_CACHE_LINE_SIZE);

    // Write jump instruction to the exception handler
    *patcher_p++ = I_LUI(REG_K0, A_BASE(EXCEPTION_HANDLER_ADDRESS));
    *patcher_p++ = I_ADDIU(REG_K0, REG_K0, A_OFFSET(EXCEPTION_HANDLER_ADDRESS));

    *patcher_p++ = I_LUI(REG_K1, j_engine_from_handler >> 16);
    *patcher_p++ = I_ORI(REG_K1, REG_K1, j_engine_from_handler);
    *patcher_p++ = I_SW(REG_K1, 0, REG_K0);
    *patcher_p++ = I_SW(REG_ZERO, 4, REG_K0);

    *patcher_p++ = I_CACHE(HIT_WRITE_BACK_D, 0, REG_K0);
    *patcher_p++ = I_CACHE(HIT_INVALIDATE_I, 0, REG_K0);

    // Set watch exception on address 0x80000180
    *patcher_p++ = I_ORI(REG_K1, REG_ZERO, EXCEPTION_HANDLER_ADDRESS | WATCHLO_W);
    *patcher_p++ = I_MTC0(REG_K1, C0_REG_WATCH_LO);
    *patcher_p++ = I_MTC0(REG_ZERO, C0_REG_WATCH_HI);

    // Jump back to the game code
    *patcher_p++ = I_JR(REG_T1);
    *patcher_p++ = I_NOP();

    cheats_update_cache(engine_start, engine_p);
    cheats_update_cache(patcher_start, patcher_p);

    return true;
}
