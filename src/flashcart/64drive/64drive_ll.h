/**
 * @file 64drive_ll.h
 * @brief 64drive flashcart low level access
 * @ingroup flashcart
 */

#ifndef FLASHCART_64DRIVE_LL_H__
#define FLASHCART_64DRIVE_LL_H__


#include <stdbool.h>
#include <stdint.h>


/**
 * @addtogroup 64drive
 * @{
 */

typedef struct {
    uint8_t BUFFER[512];
    uint32_t STATUS;
    uint32_t __unused_1;
    uint32_t COMMAND;
    uint32_t __unused_2;
    uint32_t LBA;
    uint32_t __unused_3;
    uint32_t LENGTH;
    uint32_t __unused_4;
    uint32_t RESULT;

    uint32_t __unused_5[49];

    uint32_t SDRAM_SIZE;
    uint32_t MAGIC;
    uint32_t VARIANT;
    uint32_t PERSISTENT;
    uint32_t BUTTON_UPGRADE;
    uint32_t REVISION;

    uint32_t __unused_6[64];

    uint32_t USB_COMMAND_STATUS;
    uint32_t USB_PARAM_RESULT[2];

    uint32_t __unused_7[5];

    uint32_t WIFI_COMMAND_STATUS;
    uint32_t WIFI_PARAM_RESULT[2];

    uint32_t __unused_8[757];

    uint8_t EEPROM[2048];
    uint32_t WRITEBACK[256];
} d64_regs_t;

#define D64_REGS_BASE       (0x18000000UL)
#define D64_REGS            ((d64_regs_t *) D64_REGS_BASE)


typedef enum {
    DEVICE_VARIANT_UNKNOWN  = 0x0000,
    DEVICE_VARIANT_A        = 0x4100,
    DEVICE_VARIANT_B        = 0x4200,
} d64_device_variant_t;


typedef enum {
    SAVE_TYPE_NONE,
    SAVE_TYPE_EEPROM_4K,
    SAVE_TYPE_EEPROM_16K,
    SAVE_TYPE_SRAM,
    SAVE_TYPE_FLASHRAM,
    SAVE_TYPE_SRAM_BANKED,
    SAVE_TYPE_FLASHRAM_PKST2,
} d64_save_type_t;


size_t d64_ll_get_sdram_size (void);
void d64_ll_get_version (d64_device_variant_t *device_variant, uint16_t *fpga_revision, uint32_t *bootloader_version);
bool d64_ll_set_save_type (d64_save_type_t save_type);
bool d64_ll_enable_save_writeback (bool enabled);
bool d64_ll_enable_cartrom_writes (bool enabled);
bool d64_ll_enable_extended_mode (bool enabled);

/** @} */ /* 64drive */


#endif
