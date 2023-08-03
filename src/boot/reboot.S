#define RI_ADDRESS          0xA4700000

#define RI_MODE             0x00
#define RI_CONFIG           0x04
#define RI_CURRENT_LOAD     0x08
#define RI_SELECT           0x0C
#define RI_REFRESH          0x10
#define RI_LATENCY          0x14
#define RI_RERROR           0x18
#define RI_WERROR           0x1C

#define RI_MODE_RESET       0x0000000E

#define IPL3_ENTRY          0xA4000040


.section .text.reboot, "ax", %progbits
.type reboot, %object
reboot_start:
    .global reboot_start

# NOTE: CIC x105 requirement
ipl2:
.set noat
.set noreorder
    lui $t5, 0xBFC0
1:
    lw $t0, 0x7FC($t5)
    addiu $t5, $t5, 0x7C0
    andi $t0, $t0, 0x80
    bnel $t0, $zero, 1b
    lui $t5, 0xBFC0
    lw $t0, 0x24($t5)
    lui $t3, 0xB000
.set reorder
.set at

reboot_entry:
    .equ reboot_entry_offset, ((. - reboot_start) / 4)
    .global reboot_entry_offset

reset_rdram:
    li $t0, RI_ADDRESS

    li $t1, RI_MODE_RESET
    sw $t1, RI_MODE($t0)
    sw $zero, RI_CONFIG($t0)
    sw $zero, RI_CURRENT_LOAD($t0)
    sw $zero, RI_SELECT($t0)
    sw $zero, RI_REFRESH($t0)

run_ipl3:
    li $t3, IPL3_ENTRY
    jr $t3

    .equ reboot_size, (. - reboot_start)
    .global reboot_size
