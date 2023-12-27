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

/** @brief Registers Structure. */
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

/** @brief Registers Base Address. */
#define D64_REGS_BASE       (0x18000000UL)
#define D64_REGS_BASE_EXT   (0x1F800000UL)
#define D64_REGS            ((d64_regs_t *) D64_REGS_BASE)
#define D64_REGS_EXT        ((d64_regs_t *) D64_REGS_BASE_EXT)

/** @brief Device Variant Enumeration. */
typedef enum {
    DEVICE_VARIANT_UNKNOWN  = 0x0000,
    DEVICE_VARIANT_A        = 0x4100,
    DEVICE_VARIANT_B        = 0x4200,
} d64_device_variant_t;

/** @brief TV Type Enumeration. */
typedef enum {
    TV_TYPE_PAL = 0,
    TV_TYPE_NTSC = 1,
    TV_TYPE_MPAL = 2,
    TV_TYPE_UNKNOWN = 3,
} d64_tv_type_t;

/** @brief Save Type Enumeration. */
typedef enum {
    SAVE_TYPE_NONE,
    SAVE_TYPE_EEPROM_4K,
    SAVE_TYPE_EEPROM_16K,
    SAVE_TYPE_SRAM,
    SAVE_TYPE_FLASHRAM,
    SAVE_TYPE_SRAM_BANKED,
    SAVE_TYPE_FLASHRAM_PKST2,
} d64_save_type_t;


bool d64_ll_get_version (d64_device_variant_t *device_variant, uint16_t *fpga_revision, uint32_t *bootloader_version);
bool d64_ll_set_persistent_variable_storage (bool quick_reboot, d64_tv_type_t force_tv_type, uint8_t cic_seed);
bool d64_ll_set_save_type (d64_save_type_t save_type);
bool d64_ll_enable_save_writeback (bool enabled);
bool d64_ll_enable_cartrom_writes (bool enabled);
bool d64_ll_enable_extended_mode (bool enabled);
bool d64_ll_write_eeprom_contents (void *contents);
bool d64_ll_write_save_writeback_lba_list (void *list);

/** @} */ /* 64drive */


#endif
