/**
 * @file vr4300_asm.h
 * @brief Header file for v4300 CPU-related definitions.
 * @ingroup boot
 */

#ifndef VR4300_ASM_H__
#define VR4300_ASM_H__

#include <stdint.h>

/**
 * @brief VR4300 Instruction Structure
 * 
 * This structure represents a VR4300 instruction, which can be of different types (R-type, I-type, J-type, etc.).
 */
typedef union {
    uint32_t raw; /**< Raw 32-bit instruction */

    struct {
        uint32_t op : 6;    /**< Opcode field */
        uint32_t rs : 5;    /**< Source register */
        uint32_t rt : 5;    /**< Target register */
        uint32_t imm : 16; /**< Immediate value */
    } i_type; /**< I-type instruction format */

    struct {
        uint32_t op : 6;    /**< Opcode field */
        uint32_t target : 26; /**< Target Address field */
    } j_type; /**< J-type instruction format */

    struct {
        uint32_t op : 6;    /**< Opcode field */
        uint32_t rs : 5;    /**< Source register */
        uint32_t rt : 5;    /**< Target register */
        uint32_t rd : 5;    /**< Destination register */
        uint32_t sa : 5;    /**< Shift amount */
        uint32_t funct : 6; /**< Function field */
    } r_type; /**< Alternate R-type instruction format */

    struct {
        uint32_t op : 6;    /**< Opcode field */
        uint32_t co : 1;    /**< Coprocessor operation bit */
        uint32_t funct : 25; /**< Function field */
    } c_type; /**< C-type instruction format */
} vr4300_instruction_t;

/**
 * @brief VR4300 Opcode Enumeration
 * 
 * Enumeration for different opcodes used in VR4300 instructions.
 */
typedef enum {
    OP_SPECIAL,
    OP_REGIMM,
    OP_J,
    OP_JAL,
    OP_BEQ,
    OP_BNE,
    OP_BLEZ,
    OP_BGTZ,
    OP_ADDI,
    OP_ADDIU,
    OP_SLTI,
    OP_SLTIU,
    OP_ANDI,
    OP_ORI,
    OP_XORI,
    OP_LUI,
    OP_COP0,
    OP_COP1,
    OP_COP2,
    __OP_RESERVED_19,
    OP_BEQL,
    OP_BNEL,
    OP_BLEZL,
    OP_BGTZL,
    OP_DADDI,
    OP_DADDIU,
    OP_LDL,
    OP_LDR,
    __OP_RESERVED_28,
    __OP_RESERVED_29,
    __OP_RESERVED_30,
    __OP_RESERVED_31,
    OP_LB,
    OP_LH,
    OP_LWL,
    OP_LW,
    OP_LBU,
    OP_LHU,
    OP_LWR,
    OP_LWU,
    OP_SB,
    OP_SH,
    OP_SWL,
    OP_SW,
    OP_SDL,
    OP_SDR,
    OP_SWR,
    OP_CACHE,
    OP_LL,
    OP_LWC1,
    OP_LWC2,
    __OP_RESERVED_51,
    OP_LLD,
    OP_LDC1,
    OP_LDC2,
    OP_LD,
    OP_SC,
    OP_SWC1,
    OP_SWC2,
    __OP_RESERVED_59,
    OP_SCD,
    OP_SDC1,
    OP_SDC2,
    OP_SD,
} vr4300_op_t;

typedef enum {
    FUNCT_SSL,
    __FUNCT_RESERVED_1,
    FUNCT_SRL,
    FUNCT_SRA,
    FUNCT_SLLV,
    __FUNCT_RESERVED_5,
    FUNCT_SRLV,
    FUNCT_SRAV,
    FUNCT_JR,
    FUNCT_JALR,
    __FUNCT_RESERVED_10,
    __FUNCT_RESERVED_11,
    FUNCT_SYSCALL,
    FUNCT_BREAK,
    __FUNCT_RESERVED_14,
    FUNCT_SYNC,
    FUNCT_MFHI,
    FUNCT_MTHI,
    FUNCT_MFLO,
    FUNCT_MTLO,
    FUNCT_DSLLV,
    __FUNCT_RESERVED_21,
    FUNCT_DSRLV,
    FUNCT_DSRAV,
    FUNCT_MULT,
    FUNCT_MULTU,
    FUNCT_DIV,
    FUNCT_DIVU,
    FUNCT_DMULT,
    FUNCT_DMULTU,
    FUNCT_DDIV,
    FUNCT_DDIVU,
    FUNCT_ADD,
    FUNCT_ADDU,
    FUNCT_SUB,
    FUNCT_SUBU,
    FUNCT_AND,
    FUNCT_OR,
    FUNCT_XOR,
    FUNCT_NOR,
    __FUNCT_RESERVED_40,
    __FUNCT_RESERVED_41,
    FUNCT_SLT,
    FUNCT_SLTU,
    FUNCT_DADD,
    FUNCT_DADDU,
    FUNCT_DSUB,
    FUNCT_DSUBU,
    FUNCT_TGE,
    FUNCT_TGEU,
    FUNCT_TLT,
    FUNCT_TLTU,
    FUNCT_TEQ,
    __FUNCT_RESERVED_53,
    FUNCT_TNE,
    __FUNCT_RESERVED_55,
    FUNCT_DSLL,
    __FUNCT_RESERVED_57,
    FUNCT_DSRL,
    FUNCT_DSRA,
    FUNCT_DSLL32,
    __FUNCT_RESERVED_61,
    FUNCT_DSRL32,
    FUNCT_DSRA32,
} vr4300_funct_t;

typedef enum {
    REGIMM_BLTZ,
    REGIMM_BGEZ,
    REGIMM_BLTZL,
    REGIMM_BGEZL,
    __REGIMM_RESERVED_4,
    __REGIMM_RESERVED_5,
    __REGIMM_RESERVED_6,
    __REGIMM_RESERVED_7,
    REGIMM_TGEI,
    REGIMM_TGEIU,
    REGIMM_TLTI,
    REGIMM_TLTIU,
    REGIMM_TEQI,
    __REGIMM_RESERVED_13,
    REGIMM_TNEI,
    __REGIMM_RESERVED_15,
    REGIMM_BLTZAL,
    REGIMM_BGEZAL,
    REGIMM_BLTZALL,
    REGIMM_BGEZALL,
    __REGIMM_RESERVED_20,
    __REGIMM_RESERVED_21,
    __REGIMM_RESERVED_22,
    __REGIMM_RESERVED_23,
    __REGIMM_RESERVED_24,
    __REGIMM_RESERVED_25,
    __REGIMM_RESERVED_26,
    __REGIMM_RESERVED_27,
    __REGIMM_RESERVED_28,
    __REGIMM_RESERVED_29,
    __REGIMM_RESERVED_30,
    __REGIMM_RESERVED_31,
} vr4300_regimm_t;

typedef enum {
    REG_ZERO,
    REG_AT,
    REG_V0,
    REG_V1,
    REG_A0,
    REG_A1,
    REG_A2,
    REG_A3,
    REG_T0,
    REG_T1,
    REG_T2,
    REG_T3,
    REG_T4,
    REG_T5,
    REG_T6,
    REG_T7,
    REG_S0,
    REG_S1,
    REG_S2,
    REG_S3,
    REG_S4,
    REG_S5,
    REG_S6,
    REG_S7,
    REG_T8,
    REG_T9,
    REG_K0,
    REG_K1,
    REG_GP,
    REG_SP,
    REG_FP,
    REG_RA,
} vr4300_reg_t;

typedef enum {
    C0_REG_INDEX,
    C0_REG_RANDOM,
    C0_REG_ENTRY_LO_0,
    C0_REG_ENTRY_LO_1,
    C0_REG_CONTEXT,
    C0_REG_PAGE_MASK,
    C0_REG_WIRED,
    __C0_REG_RESERVED_7,
    C0_REG_BAD_V_ADDR,
    C0_REG_COUNT,
    C0_REG_ENTRY_HI,
    C0_REG_COMPARE,
    C0_REG_STATUS,
    C0_REG_CAUSE,
    C0_REG_EPC,
    C0_REG_PR_ID,
    C0_REG_CONFIG,
    C0_REG_LL_ADDR,
    C0_REG_WATCH_LO,
    C0_REG_WATCH_HI,
    C0_REG_X_CONTEXT,
    __C0_REG_RESERVED_21,
    __C0_REG_RESERVED_22,
    __C0_REG_RESERVED_23,
    __C0_REG_RESERVED_24,
    __C0_REG_RESERVED_25,
    C0_REG_PARITY_ERROR,
    C0_REG_CACHE_ERROR,
    C0_REG_TAG_LO,
    C0_REG_TAG_HI,
    C0_REG_ERROR_EPC,
    __C0_REG_RESERVED_31,
} vr4300_c0_reg_t;

typedef enum {
    COPZ_RS_MF,
    COPZ_RS_DMF,
    COPZ_RS_CF,
    __COPZ_RS_RESERVED_3,
    COPZ_RS_MT,
    COPZ_RS_DMT,
    COPZ_RS_CT,
    __COPZ_RS_RESERVED_7,
    COPZ_RS_BC,
    __COPZ_RS_RESERVED_9,
    __COPZ_RS_RESERVED_10,
    __COPZ_RS_RESERVED_11,
    __COPZ_RS_RESERVED_12,
    __COPZ_RS_RESERVED_13,
    __COPZ_RS_RESERVED_14,
    __COPZ_RS_RESERVED_15,
} vr4300_copz_rs_t;

typedef enum {
    __C0_FUNCT_RESERVED_0,
    C0_FUNCT_TLBR,
    C0_FUNCT_TLBWI,
    __C0_FUNCT_RESERVED_3,
    __C0_FUNCT_RESERVED_4,
    __C0_FUNCT_RESERVED_5,
    C0_FUNCT_TLBWR,
    __C0_FUNCT_RESERVED_7,
    C0_FUNCT_TLBP,
    __C0_FUNCT_RESERVED_9,
    __C0_FUNCT_RESERVED_10,
    __C0_FUNCT_RESERVED_11,
    __C0_FUNCT_RESERVED_12,
    __C0_FUNCT_RESERVED_13,
    __C0_FUNCT_RESERVED_14,
    __C0_FUNCT_RESERVED_15,
    __C0_FUNCT_RESERVED_16,
    __C0_FUNCT_RESERVED_17,
    __C0_FUNCT_RESERVED_18,
    __C0_FUNCT_RESERVED_19,
    __C0_FUNCT_RESERVED_20,
    __C0_FUNCT_RESERVED_21,
    __C0_FUNCT_RESERVED_22,
    __C0_FUNCT_RESERVED_23,
    C0_FUNCT_ERET,
    __C0_FUNCT_RESERVED_25,
    __C0_FUNCT_RESERVED_26,
    __C0_FUNCT_RESERVED_27,
    __C0_FUNCT_RESERVED_28,
    __C0_FUNCT_RESERVED_29,
    __C0_FUNCT_RESERVED_30,
    __C0_FUNCT_RESERVED_31,
    __C0_FUNCT_RESERVED_32,
    __C0_FUNCT_RESERVED_33,
    __C0_FUNCT_RESERVED_34,
    __C0_FUNCT_RESERVED_35,
    __C0_FUNCT_RESERVED_36,
    __C0_FUNCT_RESERVED_37,
    __C0_FUNCT_RESERVED_38,
    __C0_FUNCT_RESERVED_39,
    __C0_FUNCT_RESERVED_40,
    __C0_FUNCT_RESERVED_41,
    __C0_FUNCT_RESERVED_42,
    __C0_FUNCT_RESERVED_43,
    __C0_FUNCT_RESERVED_44,
    __C0_FUNCT_RESERVED_45,
    __C0_FUNCT_RESERVED_46,
    __C0_FUNCT_RESERVED_47,
    __C0_FUNCT_RESERVED_48,
    __C0_FUNCT_RESERVED_49,
    __C0_FUNCT_RESERVED_50,
    __C0_FUNCT_RESERVED_51,
    __C0_FUNCT_RESERVED_52,
    __C0_FUNCT_RESERVED_53,
    __C0_FUNCT_RESERVED_54,
    __C0_FUNCT_RESERVED_55,
    __C0_FUNCT_RESERVED_56,
    __C0_FUNCT_RESERVED_57,
    __C0_FUNCT_RESERVED_58,
    __C0_FUNCT_RESERVED_59,
    __C0_FUNCT_RESERVED_60,
    __C0_FUNCT_RESERVED_61,
    __C0_FUNCT_RESERVED_62,
    __C0_FUNCT_RESERVED_63,
} vr4300_c0_funct;

#define __ASM_I_INST(o, s, t, i) \
    (((vr4300_instruction_t){.i_type = {.op = (o), .rs = (s), .rt = (t), .imm = (i)&0xFFFF}}).raw)
#define __ASM_J_INST(o, t) (((vr4300_instruction_t){.j_type = {.op = (o), .target = (t)&0x3FFFFFF}}).raw)
#define __ASM_R_INST(o, s, t, d, a, f) \
    (((vr4300_instruction_t){.r_type = {.op = (o), .rs = (s), .rt = (t), .rd = (d), .sa = (a), .funct = (f)}}).raw)
#define __ASM_C_INST(o, c, f) (((vr4300_instruction_t){.c_type = {.op = (o), .co = (c), .funct = (f)}}).raw)

#define A_OFFSET(a) ((int16_t)((a)&0xFFFF))
#define A_BASE(a) ((uint16_t)((((a) >> 16) & 0xFFFF) + (A_OFFSET(a) < 0 ? 1 : 0)))

#define I_ADDIU(rt, rs, immediate) __ASM_I_INST(OP_ADDIU, rs, rt, immediate)
#define I_AND(rd, rs, rt) __ASM_R_INST(OP_SPECIAL, rs, rt, rd, 0, FUNCT_AND)
#define I_ANDI(rt, rs, immediate) __ASM_I_INST(OP_ANDI, rs, rt, immediate)
#define I_BEQ(rs, rt, offset) __ASM_I_INST(OP_BEQ, rs, rt, offset)
#define I_BGTZ(rs, offset) __ASM_I_INST(OP_BGTZ, rs, 0, offset)
#define I_BNE(rs, rt, offset) __ASM_I_INST(OP_BNE, rs, rt, offset)
#define I_BNEL(rs, rt, offset) __ASM_I_INST(OP_BNEL, rs, rt, offset)
#define I_CACHE(op, offset, base) __ASM_I_INST(OP_CACHE, base, op, offset)
#define I_ERET() __ASM_C_INST(OP_COP0, 1, C0_FUNCT_ERET)
#define I_J(target) __ASM_J_INST(OP_J, (target >> 2))
#define I_JR(rs) __ASM_R_INST(OP_SPECIAL, rs, REG_ZERO, REG_ZERO, 0, FUNCT_JR)
#define I_LBU(rt, offset, base) __ASM_I_INST(OP_LBU, base, rt, offset)
#define I_LHU(rt, offset, base) __ASM_I_INST(OP_LHU, base, rt, offset)
#define I_LUI(rt, immediate) __ASM_I_INST(OP_LUI, 0, rt, immediate)
#define I_LW(rt, offset, base) __ASM_I_INST(OP_LW, base, rt, offset)
#define I_MFC0(rt, rd) __ASM_R_INST(OP_COP0, COPZ_RS_MF, rt, rd, 0, 0)
#define I_MTC0(rt, rd) __ASM_R_INST(OP_COP0, COPZ_RS_MT, rt, rd, 0, 0)
#define I_NOP() __ASM_R_INST(OP_SPECIAL, REG_ZERO, REG_ZERO, REG_ZERO, 0, FUNCT_SSL)
#define I_OR(rd, rs, rt) __ASM_R_INST(OP_SPECIAL, rs, rt, rd, 0, FUNCT_OR)
#define I_ORI(rt, rs, immediate) __ASM_I_INST(OP_ORI, rs, rt, immediate)
#define I_SB(rt, offset, base) __ASM_I_INST(OP_SB, base, rt, offset)
#define I_SH(rt, offset, base) __ASM_I_INST(OP_SH, base, rt, offset)
#define I_SRL(rd, rt, sa) __ASM_R_INST(OP_SPECIAL, 0, rt, rd, sa, FUNCT_SRL)
#define I_SW(rt, offset, base) __ASM_I_INST(OP_SW, base, rt, offset)

#endif /* VR4300_ASM_H__ */
