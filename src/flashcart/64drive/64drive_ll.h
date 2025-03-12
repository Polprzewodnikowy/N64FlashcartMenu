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
    uint8_t BUFFER[512]; /**< General buffer */
    uint32_t STATUS; /**< Status register */
    uint32_t __unused_1; /**< Unused */
    uint32_t COMMAND; /**< Command register */
    uint32_t __unused_2; /**< Unused */
    uint32_t LBA; /**< Logical Block Address register */
    uint32_t __unused_3; /**< Unused */
    uint32_t LENGTH; /**< Length register */
    uint32_t __unused_4; /**< Unused */
    uint32_t RESULT; /**< Result register */

    uint32_t __unused_5[49]; /**< Unused */

    uint32_t SDRAM_SIZE; /**< SDRAM size register */
    uint32_t MAGIC; /**< Magic register */
    uint32_t VARIANT; /**< Variant register */
    uint32_t PERSISTENT; /**< Persistent register */
    uint32_t BUTTON_UPGRADE; /**< Button upgrade register */
    uint32_t REVISION; /**< Revision register */

    uint32_t __unused_6[64]; /**< Unused */

    uint32_t USB_COMMAND_STATUS; /**< USB command status register */
    uint32_t USB_PARAM_RESULT[2]; /**< USB parameter result registers */

    uint32_t __unused_7[5]; /**< Unused */

    uint32_t WIFI_COMMAND_STATUS; /**< WiFi command status register */
    uint32_t WIFI_PARAM_RESULT[2]; /**< WiFi parameter result registers */

    uint32_t __unused_8[757]; /**< Unused */

    uint8_t EEPROM[2048]; /**< EEPROM buffer */
    uint32_t WRITEBACK[256]; /**< Writeback buffer */
} d64_regs_t;

/** @brief Registers Base Address. */
#define D64_REGS_BASE       (0x18000000UL)
#define D64_REGS_BASE_EXT   (0x1F800000UL)
#define D64_REGS            ((d64_regs_t *) D64_REGS_BASE)
#define D64_REGS_EXT        ((d64_regs_t *) D64_REGS_BASE_EXT)

/** @brief Device Variant Enumeration. */
typedef enum {
    DEVICE_VARIANT_UNKNOWN  = 0x0000, /**< Unknown device variant */
    DEVICE_VARIANT_A        = 0x4100, /**< Device variant A */
    DEVICE_VARIANT_B        = 0x4200, /**< Device variant B */
} d64_device_variant_t;

/** @brief TV Type Enumeration. */
typedef enum {
    TV_TYPE_PAL = 0, /**< PAL TV type */
    TV_TYPE_NTSC = 1, /**< NTSC TV type */
    TV_TYPE_MPAL = 2, /**< MPAL TV type */
    TV_TYPE_UNKNOWN = 3, /**< Unknown TV type */
} d64_tv_type_t;

/** @brief Save Type Enumeration. */
typedef enum {
    SAVE_TYPE_NONE, /**< No save type */
    SAVE_TYPE_EEPROM_4KBIT, /**< EEPROM 4Kbit */
    SAVE_TYPE_EEPROM_16KBIT, /**< EEPROM 16Kbit */
    SAVE_TYPE_SRAM_256KBIT, /**< SRAM 256Kbit */
    SAVE_TYPE_FLASHRAM_1MBIT, /**< FlashRAM 1Mbit */
    SAVE_TYPE_SRAM_BANKED, /**< SRAM Banked */
    SAVE_TYPE_FLASHRAM_PKST2, /**< FlashRAM PKST2 */
} d64_save_type_t;

/**
 * @brief Get the 64drive version.
 * 
 * @param device_variant Pointer to store the device variant.
 * @param fpga_revision Pointer to store the FPGA revision.
 * @param bootloader_version Pointer to store the bootloader version.
 * @return true if successful, false otherwise.
 */
bool d64_ll_get_version(d64_device_variant_t *device_variant, uint16_t *fpga_revision, uint32_t *bootloader_version);

/**
 * @brief Set the persistent variable storage.
 * 
 * @param quick_reboot Enable or disable quick reboot.
 * @param force_tv_type TV type to force.
 * @param cic_seed CIC seed value.
 * @return true if successful, false otherwise.
 */
bool d64_ll_set_persistent_variable_storage(bool quick_reboot, d64_tv_type_t force_tv_type, uint8_t cic_seed);

/**
 * @brief Set the save type.
 * 
 * @param save_type The save type to set.
 * @return true if successful, false otherwise.
 */
bool d64_ll_set_save_type(d64_save_type_t save_type);

/**
 * @brief Enable or disable save writeback.
 * 
 * @param enabled True to enable, false to disable.
 * @return true if successful, false otherwise.
 */
bool d64_ll_enable_save_writeback(bool enabled);

/**
 * @brief Enable or disable cart ROM writes.
 * 
 * @param enabled True to enable, false to disable.
 * @return true if successful, false otherwise.
 */
bool d64_ll_enable_cartrom_writes(bool enabled);

/**
 * @brief Enable or disable extended mode.
 * 
 * @param enabled True to enable, false to disable.
 * @return true if successful, false otherwise.
 */
bool d64_ll_enable_extended_mode(bool enabled);

/**
 * @brief Write EEPROM contents.
 * 
 * @param contents Pointer to the EEPROM contents.
 * @return true if successful, false otherwise.
 */
bool d64_ll_write_eeprom_contents(void *contents);

/**
 * @brief Write save writeback LBA list.
 * 
 * @param list Pointer to the LBA list.
 * @return true if successful, false otherwise.
 */
bool d64_ll_write_save_writeback_lba_list(void *list);

/** @} */ /* 64drive */

#endif /* FLASHCART_64DRIVE_LL_H__ */
