# Original watch exception handler code written by Jay Oster 'Parasyte'
# https://github.com/parasyte/alt64/blob/master/utils.c#L891-L1153

#include "cheats.h"

#define HIT_INVALIDATE_I ((4 << 2) | 0)
#define HIT_WRITE_BACK_D ((6 << 2) | 1)

#define C0_CAUSE $13
#define C0_EPC $14
#define C0_WATCHLO $18

#define CAUSE_IRQ_PRE_NMI (1 << 12)
#define CAUSE_EXC_CODE_MASK (0x7C)
#define CAUSE_EXC_CODE_WATCH (0x5C)

#define RDRAM_CACHED_BASE (0x80000000)
#define EXCEPTION_HANDLER_ADDRESS_OFFSET (0x188)

.section .text.cheat_payload, "ax", %progbits
cheat_payload:
    .global cheat_payload

exception_handler:
    .set noat
    .set noreorder

    mfc0 $k0, C0_CAUSE                                  # Get exception cause register

exception_check_pre_nmi:
    andi $k1, $k0, CAUSE_IRQ_PRE_NMI                    # Check for pending pre NMI interrupt
    bnezl $k1, 1f                                       # If not, continue
    mtc0 $zero, C0_WATCHLO                              # Disable watchpoint if pre NMI is pending
1:

exception_check_code:
    andi $k0, CAUSE_EXC_CODE_MASK                       # Extract exception code
    li $k1, CAUSE_EXC_CODE_WATCH                        # Check for watch exception
    bne $k0, $k1, exception_execute_cheats              # If not, run the cheat engine

exception_handle_watch:                                 # Protect exception handler from being replaced by the game code
    mfc0 $k1, C0_EPC                                    # Get exception instruction address
    lw $k1, ($k1)                                       # Load instruction at exception address (should be store instruction)
    li $k0, 0x03E00000                                  # Base register mask
    and $k1, $k0                                        # Apply mask
    srl $k1, 5                                          # Move register number to the rt field in the 'ori' instruction

    li $k0, (0b001101 << 26) | (26 << 21)               # Assemble 'ori $zero, $k0, 0' instruction
    ori $k0, %lo(RELOCATED_EXCEPTION_HANDLER_ADDRESS)   # Apply relocation offset
    or $k0, $k1                                         # Update rt field in the assembled instruction

    li $k1, RDRAM_CACHED_BASE
    lw $k1, EXCEPTION_HANDLER_ADDRESS_OFFSET($k1)       # Load cheat engine address
    addiu $k1, placeholder_offset                       # Calculate placeholder instruction address
    sw $k0, ($k1)                                       # Write assembled instruction to the placeholder
    cache HIT_WRITE_BACK_D, ($k1)                       # Writeback assembled instruction
    cache HIT_INVALIDATE_I, ($k1)                       # Force update instruction cache

    li $k0, RDRAM_CACHED_BASE                           # Next instruction use k0 register in the 'rs' field
    .set placeholder_offset, (. - exception_handler)
    nop                                                 # Placeholder instruction to use by the code above

    eret                                                # Return from the exception

exception_execute_cheats:

    # Assembled cheats will be appended here

    .set reorder
    .set at

    .set cheat_payload_size, (. - cheat_payload)
    .global cheat_payload_size
