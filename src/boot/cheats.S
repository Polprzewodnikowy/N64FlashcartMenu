# Original code written by Jay Oster 'Parasyte'
# https://github.com/parasyte/alt64/blob/master/utils.c#L891-L1153
#
# This cheat engine works only with the games that utilize libultra toolchain
# Libdragon based software is unsupported due to completely different approach to the boot process
#
# Supported codes:
#  - 50 - Repeater (patcher)
#  - 80/81/A0/A1 - Continous 8/16 bit write to a cached/uncached address
#  - D0/D1/D2/D3 - Conditional 8/16 bit equal/different to specified value
#  - EE - Disable expansion pak
#  - F0/F1 - Single 8/16 bit write to an uncached address at boot
#  - FF - Set cheat engine and cheat list address
#
# Unsupported codes (for now):
#  - 88/89/A8/A9 - Continous 8/16 bit write to a cached/uncached address (triggered by GS button)
#  - D8/D9/DA/DB - Conditional 8/16 bit equal/different to specified value (triggered by GS button)
#
# TODO/ideas list:
#  - Support "GS button" cheats
#  - Implement USB support


#include "cheats.h"


#define D_CACHE_LINE_SIZE               (16)

#define HIT_INVALIDATE_I                ((4 << 2) | 0)
#define HIT_WRITE_BACK_D                ((6 << 2) | 1)

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
#define ENGINE_CHEAT_CODE_ADDRESS_MASK  0xA07FFFFF

#define CHEAT_ENGINE_BACKUP_REGISTERS   (6)


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
    lh $v1, 6($t8)                                  # v1 = Cheat value
    srl $t2, $v0, 24                                # t2 = Cheat type
    li $t3, BOOT_CHEAT_CODE_ADDRESS_MASK
    li $t4, RDRAM_UNCACHED_BASE
    and $t3, $v0                                    # Sanitize address
    or $t4, $t3                                     # t4 = Uncached address
    or $t5, $t3, $t9                                # t5 = Cached address

patcher_handle_disable_expansion_pak_code:
    li $t3, 0xEE
    bne $t2, $t3, 1f
    li $t3, 0x04000000                              # Load 4 MB value
    sw $t3, 0x318($t4)                              # Store it at default osMemSize address (0xA0000318)
    sw $t3, 0x3F0($t4)                              # Store it at CIC x105 osMemSize address (0xA00003F0)
    b patcher_get_boot_cheat
1:

patcher_handle_write_byte_on_boot_code:
    li $t3, 0xF0
    bne $t2, $t3, 1f
    sb $v1, ($t4)
    b patcher_get_boot_cheat
1:

patcher_handle_write_halfword_on_boot_code:
    li $t3, 0xF1
    bne $t2, $t3, 1f
    sh $v1, ($t4)
    b patcher_get_boot_cheat
1:

patcher_handle_set_cheat_engine_location_code:
    li $t3, 0xFF
    bne $t2, $t3, 1f
    li $t3, 0xFFFFFFFC                              # Address mask
    and $t7, $t5, $t3                               # Sanitize address and update final cheat engine address
    b patcher_get_boot_cheat
1:

patcher_install_exception_handler:
    addiu $t7, (8 * CHEAT_ENGINE_BACKUP_REGISTERS)  # Reserve space for registers backup
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
    addiu $t7, 4
    addiu $t2, 4
    bne $t3, $t7, 1b

patcher_install_codes:
    lw $v0, 0($t8)                                  # Copy cheat list to the final location
    lhu $v1, 6($t8)                                 # Care only about lower 16 bits of the cheat value
    sw $v0, 0($t7)
    sw $v1, 4($t7)
    or $v0, $v1
    addiu $t8, 8
    addiu $t7, 8
    bnez $v0, patcher_install_codes                 # Break when cheat type, address and value is equal to zero

patcher_update_cache:
    cache HIT_WRITE_BACK_D, GEH_JUMP_OFFSET($t9)    # Writeback exception handler to the physical memory
    cache HIT_INVALIDATE_I, GEH_JUMP_OFFSET($t9)    # Invalidate existing exception handler in the cache
    lw $t2, GEH_ENGINE_ADDRESS_OFFSET($t9)          # Load back stored cheat engine address
1:
    cache HIT_WRITE_BACK_D, ($t2)                   # Writeback cheat engine and cheat list to the physical memory
    cache HIT_INVALIDATE_I, ($t2)                   # Invalidate instruction cache for the cheat engine
    addiu $t2, D_CACHE_LINE_SIZE                    # Increment pointer by the D cache line size
    blt $t2, $t7, 1b                                # Loop until done

patcher_set_watchpoint:
    li $t2, (GEH_JUMP_OFFSET | WATCHLO_W)           # Set write watchpoint to the physical address 0x00000180 (exception handler)
    mtc0 $t2, C0_WATCHLO
    mtc0 $zero, C0_WATCHHI

patcher_return_to_game_code:
    jr $t1                                          # Stock IPL3 always use 't1' register as entry point

    .set cheat_patcher_size, (. - cheat_patcher)


# ----- CHEAT ENGINE -----

# This code is hooked to the general exeption handler and is run on every exception/interrupt

cheat_engine:
    .set noat

    mfc0 $k0, C0_CAUSE                              # Get exception cause register

engine_check_pre_nmi:
    andi $k1, $k0, CAUSE_IRQ_PRE_NMI                # Check for pending pre NMI interrupt
    .set noreorder
    bnezl $k1, 1f                                   # If not, continue
    mtc0 $zero, C0_WATCHLO                          # Disable watchpoint if pre NMI is pending
1:
    .set reorder

engine_check_exception_code:
    andi $k0, CAUSE_EXC_CODE_MASK                   # Extract exception code
    li $k1, CAUSE_EXC_CODE_WATCH                    # Check for watch exception
    bne $k0, $k1, engine_run                        # If not, run the cheat engine

engine_handle_watch_exception:                      # Protect engine from being replaced by the game code
    mfc0 $k1, C0_EPC                                # Get exception instruction address
    lw $k1, ($k1)                                   # Load instruction at exception address (should be store instruction)
    li $k0, 0x03E00000                              # Base register mask
    and $k1, $k0                                    # Apply mask
    srl $k1, 5                                      # Move register number to the rt field in the 'ori' instruction

    li $k0, (0b001101 << 26) | (26 << 21)           # Assemble 'ori $zero, $k0, 0' instruction
    ori $k0, GEH_RELOCATE_OFFSET                    # Apply relocation offset
    or $k0, $k1                                     # Update rt field in the assembled instruction

    li $k1, RDRAM_CACHED_BASE
    lw $k1, GEH_ENGINE_ADDRESS_OFFSET($k1)          # Load cheat engine address
    addiu $k1, placeholder_instruction_offset       # Calculate placeholder instruction address
    sw $k0, ($k1)                                   # Write assembled instruction to the placeholder
    cache HIT_WRITE_BACK_D, ($k1)                   # Writeback assembled instruction
    cache HIT_INVALIDATE_I, ($k1)                   # Force update instruction cache

    .set noreorder
    li $k0, RDRAM_CACHED_BASE                       # Next instruction use k0 register in the 'rs' field
    .set placeholder_instruction_offset, (. - cheat_engine)
    nop                                             # Placeholder instruction to use by the code above
    .set reorder

    eret                                            # Return from the exception

engine_run:
    li $k0, RDRAM_CACHED_BASE
    lw $k0, GEH_ENGINE_ADDRESS_OFFSET($k0)          # Load cheat engine address

engine_backup_registers:
    sd $v1, -8($k0)
    sd $v0, -16($k0)
    sd $t3, -24($k0)
    sd $t2, -32($k0)
    sd $t1, -40($k0)
    sd $t0, -48($k0)

    addiu $k1, $k0, cheat_engine_size               # k1 = Cheat list pointer

engine_load_cheat:
    lw $v0, 0($k1)                                  # v0 = Cheat type and address
    lh $v1, 6($k1)                                  # v1 = Cheat value
    or $t0, $v0, $v1                                # Check if both fields are zeroed
    addiu $k1, 8                                    # Increment cheat list pointer
    beqz $t0, engine_restore_registers              # Break when cheat type, address and value is equal to zero
    srl $t0, $v0, 24                                # t0 = Cheat type
    li $t1, ENGINE_CHEAT_CODE_ADDRESS_MASK          # Address mask
    and $v0, $t1                                    # Sanitize cheat address

engine_check_for_repeater_code:
    li $t1, 0x50
    beq $t1, $t0, engine_handle_repeater_code       # 0x50xxxxxx == Repeater code

engine_check_for_conditional_code:
    li $t1, 0xD0
    andi $t2, $t0, 0xF0
    beq $t1, $t2, engine_handle_conditional_code    # 0xDxxxxxxx == Conditional code

engine_handle_write_code:                           # All other cases == Write code
    sltiu $t1, $t0, 0x80                            # Check for invalid addresses below 0x80000000
    andi $t2, $t0, (1 << 3)                         # Extract "GS button" flag
    or $t1, $t2                                     # OR previous two conditions
    andi $t2, $t0, (1 << 0)                         # Extract "16 bit" flag
    bnez $t1, engine_load_cheat                     # Skip execution on invalid address or "GS button" cheat (for now)
    .set noreorder
    bnezl $t2, 1f                                   # Check for 16 bit write
    sh $v1, ($v0)                                   # Write 16 bit value
    sb $v1, ($v0)                                   # Write 8 bit value
1:
    .set reorder
    b engine_load_cheat                             # Process more cheats

engine_handle_conditional_code:
    andi $t1, $t0, (1 << 3)                         # Extract "GS button" flag
    andi $t2, $t0, (1 << 0)                         # Extract "16 bit" flag
    bnez $t1, engine_load_cheat                     # "GS button" cheats are unsupported (for now)
    .set noreorder
    bnezl $t2, 1f                                   # Check for 16 bit read
    lh $t1, ($v0)                                   # Read 16 bit value
    lbu $t1, ($v0)                                  # Read 8 bit value
    andi $v1, 0xFF                                  # Sanitize cheat value for 8 bit check
1:
    .set reorder
    lw $v0, ($k1)                                   # Load next cheat type and address
    srl $v0, 24                                     # Extract cheat type
    li $t2, 0x50
    .set noreorder
    beql $v0, $t2, 1f                               # Check if next cheat is a repeater code
    li $v0, 16                                      # Increment by 2 codes if next cheat is a repeater code
    li $v0, 8                                       # Increment by 1 code for all other cheat codes
1:
    .set reorder
    andi $t0, (1 << 1)                              # Extract "Different to" flag
    xor $t1, $v1                                    # Check for difference between read value and expected value
    bnez $t0, 2f                                    # Handle "Different to" cases
    .set noreorder
    bnezl $t1, 1f                                   # Check if the values are different
    addu $k1, $v0                                   # Increment cheat list pointer if the condition is met
1:
    .set reorder
    b engine_load_cheat                             # Process more cheats
2:
    .set noreorder
    beqzl $t1, 1f                                   # Check if the values are the same
    addu $k1, $v0                                   # Increment cheat list pointer if the condition is met
1:
    .set reorder
    b engine_load_cheat                             # Process more cheats

engine_handle_repeater_code:
    lw $t0, 0($k1)                                  # Load address
    srl $t0, 24                                     # Extract cheat type
    sltiu $t1, $t0, 0x80                            # Check for invalid addresses below 0x80000000
    andi $t2, $t0, (1 << 3)                         # Extract "GS button" flag
    or $t1, $t2                                     # OR previous two conditions
    addiu $k1, 8                                    # Increment cheat list pointer
    bnez $t1, engine_load_cheat                     # Skip execution on invalid address or "GS button" cheat (for now)
    andi $t2, $t0, (1 << 0)                         # Extract "16 bit" flag
    lw $t0, -8($k1)                                 # Load address
    li $t1, ENGINE_CHEAT_CODE_ADDRESS_MASK          # Address mask
    and $t0, $t1                                    # Sanitize cheat address
    lh $t1, -2($k1)                                 # Load value
    andi $t3, $v0, 0xFF                             # Extract address increment
    srl $v0, 8
    andi $v0, 0xFF                                  # Extract address count
1:
    .set noreorder
    bnezl $t2, 2f                                   # Check for 16 bit write
    sh $t1, ($t0)                                   # Write 16 bit value
    sb $t1, ($t0)                                   # Write 8 bit value
2:
    .set reorder
    addiu $v0, -1                                   # Decrement counter
    addu $t1, $v1                                   # Calculate next value
    addu $t0, $t3                                   # Increment address by offset
    bnez $v0, 1b                                    # Loop until done
    b engine_load_cheat                             # Process more cheats

engine_restore_registers:
    ld $t0, -48($k0)
    ld $t1, -40($k0)
    ld $t2, -32($k0)
    ld $t3, -24($k0)
    ld $v0, -16($k0)
    ld $v1, -8($k0)

engine_return_to_the_original_exception_handler:
    j (RDRAM_CACHED_BASE | GEH_RELOCATE_OFFSET)

    .set at

    .set cheat_engine_size, (. - cheat_engine)

    .set cheat_payload_size, (. - cheat_payload)
    .global cheat_payload_size
