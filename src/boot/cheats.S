# Original code written by Jay Oster 'Parasyte'
# https://github.com/parasyte/alt64/blob/master/utils.c#L891-L1153


#include "cheats.h"

#define D_CACHE_LINE_SIZE               (16)

#define HIT_INVALIDATE_I                ((4 << 2) | 0)
#define HIT_WRITEBACK_D                 ((6 << 2) | 1)

#define C0_CAUSE                        $13
#define C0_EPC                          $14
#define C0_WATCHLO                      $18
#define C0_WATCHHI                      $19

#define CAUSE_IRQ_PRE_NMI               (1 << 12)
#define CAUSE_EXC_CODE_MASK             (0x7C)
#define CAUSE_EXC_CODE_WATCH            (0x5C)

#define WATCHLO_W                       (1 << 0)

#define GEH_RELOCATE_OFFSET             (0x120)
#define GEH_JUMP_OFFSET                 (0x180)
#define GEH_DELAY_SLOT_OFFSET           (0x184)
#define GEH_ENGINE_ADDRESS_OFFSET       (0x188)

#define RDRAM_CACHED_BASE               0x80000000
#define RDRAM_UNCACHED_BASE             0xA0000000
#define DEFAULT_CHEAT_ENGINE_ADDRESS    0x807C5C00
#define BOOT_CHEAT_CODE_ADDRESS_MASK    0x007FFFFF


.section .text.cheat_payload, "ax", %progbits
cheat_payload:
    .global cheat_payload


# ----- CHEAT PATCHER -----

# This code is hooked at the end of the IPL3 replacing jump to the game entry point

cheat_patcher:
    li $t6, CHEAT_PAYLOAD_ADDRESS                   # t6 = Payload address
    li $t7, DEFAULT_CHEAT_ENGINE_ADDRESS            # t7 = Final cheat engine address
    addiu $t8, $t6, (cheat_payload_size - 8)        # t8 = Cheat list location, sorted by boot cheats first
    li $t9, RDRAM_CACHED_BASE                       # t9 = RDRAM cached address base

patcher_get_boot_cheat:
    addiu $t8, 8                                    # Increment cheat list pointer
    lw $v0, 0($t8)                                  # v0 = Cheat type and address
    lw $v1, 4($t8)                                  # v1 = Cheat value
    srl $t2, $v0, 24                                # t2 = Cheat type
    li $t3, BOOT_CHEAT_CODE_ADDRESS_MASK
    li $t4, RDRAM_UNCACHED_BASE
    and $t3, $v0                                    # Sanitize address
    or $t4, $t3                                     # t4 = Uncached address
    or $t5, $t3, $t9                                # t5 = Cached address

patcher_handle_boot_EE_code:                        # "Disable" expansion pak
    li $t3, 0xEE
    bne $t2, $t3, 1f
    li $t3, 0x04000000                              # Load 4 MB value
    sw $t3, 0x318($t4)                              # Store it at default osMemSize address (0xA0000318)
    sw $t3, 0x3F0($t4)                              # Store it at CIC x105 osMemSize address (0xA00003F0)
    b patcher_get_boot_cheat
1:

patcher_handle_boot_F0_code:                        # Write byte to an uncached address
    li $t3, 0xF0
    bne $t2, $t3, 1f
    sb $v1, ($t4)
    b patcher_get_boot_cheat
1:

patcher_handle_boot_F1_code:                        # Write half word to an uncached address
    li $t3, 0xF1
    bne $t2, $t3, 1f
    sh $v1, ($t4)
    b patcher_get_boot_cheat
1:

patcher_handle_boot_FF_code:                        # Set final cheat engine address
    li $t3, 0xFF
    bne $t2, $t3, 1f
    li $t3, 0xFFFFFFFC                              # Address mask
    and $t7, $t5, $t3                               # Sanitize address and update final cheat engine address
    b patcher_get_boot_cheat
1:

patcher_install_exception:
    li $t2, 0x0FFFFFFC                              # Mask bits [31:28] and [1:0] of the address
    and $t2, $t7                                    # Apply mask to the final cheat engine address
    srl $t2, 2                                      # Move address to the correct bits in the 'j' instruction
    li $t3, (0b000010 << 26)                        # 'j' instruction opcode
    or $t2, $t3                                     # Assemble the 'j' instruction
    sw $t2, GEH_JUMP_OFFSET($t9)                    # Write assembled instruction to the beginning of the exception handler
    sw $zero, GEH_DELAY_SLOT_OFFSET($t9)            # Write nop in the delay slot

patcher_install_engine:
    sw $t7, GEH_ENGINE_ADDRESS_OFFSET($t9)          # Store final cheat engine address
    addiu $t2, $t6, cheat_patcher_size              # Calculate cheat engine load start address
    addiu $t3, $t7, cheat_engine_size               # Calculate cheat engine store end address
1:
    lw $t4, ($t2)                                   # Copy cheat engine code to the final location
    sw $t4, ($t7)
    addiu $t2, 4
    addiu $t7, 4
    bne $t3, $t7, 1b

patcher_install_codes:
    lw $v0, 0($t8)                                  # Copy cheat list to the final location
    lw $v1, 4($t8)
    sw $v0, 0($t7)
    sw $v1, 4($t7)
    addiu $t8, 8
    addiu $t7, 8
    or $v0, $v1
    bnez $v0, patcher_install_codes                 # Break when cheat type, address and value is equal to zero

patcher_cache_writeback_invalidate:
    cache HIT_WRITEBACK_D, GEH_JUMP_OFFSET($t9)     # Writeback exception handler to the physical memory
    cache HIT_INVALIDATE_I, GEH_JUMP_OFFSET($t9)    # Invalidate existing exception handler in the cache
    lw $t2, GEH_ENGINE_ADDRESS_OFFSET($t9)          # Load back stored cheat engine address
1:
    cache HIT_WRITEBACK_D, ($t2)                    # Writeback cheat engine and cheat list to the physical memory
    cache HIT_INVALIDATE_I, ($t2)                   # Invalidate instruction cache for cheat engine
    addiu $t2, D_CACHE_LINE_SIZE                    # Increment pointer by D cache line size
    blt $t2, $t7, 1b                                # Loop until done

patcher_set_watchpoint:
    li $t2, (GEH_JUMP_OFFSET | WATCHLO_W)           # Set write watchpoint to the physical address 0x00000180 (exception handler)
    mtc0 $t2, C0_WATCHLO
    nop
    mtc0 $zero, C0_WATCHHI
    nop

patcher_return_to_game_code:
    .set noreorder
    jr $t1                                          # Stock IPL3 always use 't1' register as entry point
    nop                                             # Manually place nop in the slot delay to prevent compiler from being "too smart"
    .set reorder

    .set cheat_patcher_size, (. - cheat_patcher)


# ----- CHEAT ENGINE -----

# This code is hooked to the general exeption handler and is run on every exception/interrupt

cheat_engine:
    .set noat

    mfc0 $k0, C0_CAUSE                              # Get exception cause register

engine_check_pre_nmi:
    andi $k1, $k0, CAUSE_IRQ_PRE_NMI                # Check for pending pre NMI interrupt
    beqz $k1, engine_check_exception_code           # If not, continue
    mtc0 $zero, C0_WATCHLO                          # Disable watchpoint if pre NMI is pending
    nop

engine_check_exception_code:
    andi $k0, CAUSE_EXC_CODE_MASK                   # Extract exception code
    li $k1, CAUSE_EXC_CODE_WATCH                    # Check for watch exception
    bne $k0, $k1, engine_run                        # If not, run the cheat engine

engine_handle_watch_exception:                      # Protect engine from being replaced by the game code
    mfc0 $k1, C0_EPC                                # Get exception instruction address
    lw $k1, ($k1)                                   # Load instruction at exception address (should be store instruction)
    li $k0, 0x03E00000                              # Base register mask
    and $k1, $k0                                    # Apply mask
    srl $k1, 5                                      # Move register number to the rt field of the 'ori' instruction

    li $k0, (0b001101 << 26) | (26 << 21)           # Assemble 'ori $zero, $k0, 0' instruction
    ori $k0, GEH_RELOCATE_OFFSET                    # Apply relocation offset
    or $k0, $k1                                     # Update rt field of the assembled instruction

    li $k1, RDRAM_CACHED_BASE
    lw $k1, GEH_ENGINE_ADDRESS_OFFSET($k1)          # Load cheat engine address
    addiu $k1, placeholder_instruction_offset       # Calculate placeholder instruction address
    sw $k0, ($k1)                                   # Write assembled instruction to the placeholder
    cache HIT_WRITEBACK_D, ($k1)                    # Writeback assembled instruction
    cache HIT_INVALIDATE_I, ($k1)                   # Force update instruction cache

    .set noreorder
    li $k0, RDRAM_CACHED_BASE                       # Next instruction use k0 register in the 'rs' field
    .set placeholder_instruction_offset, (. - cheat_engine)
    nop                                             # Placeholder instruction to use by the code above
    .set reorder

    eret                                            # Return from the exception

engine_run:
    li $k0, RDRAM_CACHED_BASE
    lw $k0, GEH_ENGINE_ADDRESS_OFFSET($k0)

engine_backup_registers:
    sd $v1, -8($k0)
    sd $v0, -16($k0)
    sd $t9, -24($k0)
    sd $t8, -32($k0)
    sd $t7, -40($k0)
    sd $t6, -48($k0)
    sd $t5, -56($k0)

    addiu $t8, $k0, cheat_engine_size

engine_load_cheat:
    lw $v0, 0($t8)
    lw $v1, 4($t8)
    addiu $t8, 8
    or $t9, $v0, $v1
    beqz $t9, engine_restore_registers              # Stop executing when there are no more cheats to process
    srl $t7, $v0, 24
    li $t5, 0x007FFFFF
    and $v0, $t5
    li $t5, 0x80000000
    or $v0, $t5

engine_handle_80_code:
    li $t9, 0x80
    bne $t7, $t9, 1f
    sb $v1, ($v0)
1:

engine_handle_81_code:
    li $t9, 0x81
    bne $t7, $t9, 1f
    sh $v1, ($v0)
1:

engine_handle_D0_code:
    li $t9, 0xD0
    bne $t7, $t9, 1f
    lbu $t9, ($v0)
    andi $v1, 0xFF
    beq $t9, $v1, 1f
    addiu $t8, 8
1:

engine_handle_D1_code:
    li $t9, 0xD1
    bne $t7, $t9, 1f
    lhu $t9, ($v0)
    andi $v1, 0xFFFF
    beq $t9, $v1, 1f
    addiu $t8, 8
1:

engine_handle_unknown_code:
    b engine_load_cheat

engine_restore_registers:
    ld $t5, -56($k0)
    ld $t6, -48($k0)
    ld $t7, -40($k0)
    ld $t8, -32($k0)
    ld $t9, -24($k0)
    ld $v0, -16($k0)
    ld $v1, -8($k0)

engine_return_to_original_exception_handler:
    .set noreorder
    j (RDRAM_CACHED_BASE | GEH_RELOCATE_OFFSET)
    nop
    .set reorder

    .set at

    .set cheat_engine_size, (. - cheat_engine)

    .set cheat_payload_size, (. - cheat_payload)
    .global cheat_payload_size
