/**
 * @file sc64_ll.h
 * @brief SC64 flashcart low level access
 * @ingroup flashcart
 */

#ifndef FLASHCART_SC64_LL_H__
#define FLASHCART_SC64_LL_H__

#include <stddef.h>
#include <stdint.h>

/**
 * @addtogroup sc64
 * @{
 */

/** @brief The SC64 buffers structure. */
typedef struct {
    uint8_t BUFFER[8192]; /**< General buffer */
    uint8_t EEPROM[2048]; /**< EEPROM buffer */
    uint8_t DD_SECTOR[256]; /**< Disk Drive sector buffer */
    uint8_t FLASHRAM[128]; /**< FlashRAM buffer */
} sc64_buffers_t;

#define SC64_BUFFERS_BASE       (0x1FFE0000UL)
#define SC64_BUFFERS            ((sc64_buffers_t *) SC64_BUFFERS_BASE)

/** @brief The SC64 State Enumeration. */
typedef enum {
    SC64_OK, /**< No error */
    SC64_ERROR_BAD_ARGUMENT, /**< Bad argument error */
    SC64_ERROR_BAD_ADDRESS, /**< Bad address error */
    SC64_ERROR_BAD_CONFIG_ID, /**< Bad config ID error */
    SC64_ERROR_TIMEOUT, /**< Timeout error */
    SC64_ERROR_SD_CARD, /**< SD card error */
    SC64_ERROR_UNKNOWN_CMD = -1 /**< Unknown command error */
} sc64_error_t;

/** @brief The SC64 Configuration ID Enumeration. */
typedef enum {
    CFG_ID_BOOTLOADER_SWITCH, /**< Bootloader switch config */
    CFG_ID_ROM_WRITE_ENABLE, /**< ROM write enable config */
    CFG_ID_ROM_SHADOW_ENABLE, /**< ROM shadow enable config */
    CFG_ID_DD_MODE, /**< Disk Drive mode config */
    CFG_ID_ISV_ADDRESS, /**< ISV address config */
    CFG_ID_BOOT_MODE, /**< Boot mode config */
    CFG_ID_SAVE_TYPE, /**< Save type config */
    CFG_ID_CIC_SEED, /**< CIC seed config */
    CFG_ID_TV_TYPE, /**< TV type config */
    CFG_ID_DD_SD_ENABLE, /**< Disk Drive SD enable config */
    CFG_ID_DD_DRIVE_TYPE, /**< Disk Drive type config */
    CFG_ID_DD_DISK_STATE, /**< Disk Drive disk state config */
    CFG_ID_BUTTON_STATE, /**< Button state config */
    CFG_ID_BUTTON_MODE, /**< Button mode config */
    CFG_ID_ROM_EXTENDED_ENABLE /**< ROM extended enable config */
} sc64_cfg_id_t;

/** @brief The SC64 Disk Drive Mode Enumeration. */
typedef enum {
    DD_MODE_DISABLED = 0, /**< Disk Drive disabled */
    DD_MODE_REGS = 1, /**< Disk Drive registers mode */
    DD_MODE_IPL = 2, /**< Disk Drive IPL mode */
    DD_MODE_FULL = 3 /**< Disk Drive full mode */
} sc64_dd_mode_t;

/** @brief The SC64 Boot Mode Enumeration. */
typedef enum {
    BOOT_MODE_MENU = 0, /**< Boot to menu */
    BOOT_MODE_ROM = 1, /**< Boot to ROM */
    BOOT_MODE_DDIPL = 2, /**< Boot to Disk Drive IPL */
    BOOT_MODE_DIRECT_ROM = 3, /**< Direct boot to ROM */
    BOOT_MODE_DIRECT_DDIPL = 4 /**< Direct boot to Disk Drive IPL */
} sc64_boot_mode_t;

/** @brief The SC64 Save Type Enumeration. */
typedef enum {
    SAVE_TYPE_NONE, /**< No save type */
    SAVE_TYPE_EEPROM_4KBIT, /**< EEPROM 4Kbit */
    SAVE_TYPE_EEPROM_16KBIT, /**< EEPROM 16Kbit */
    SAVE_TYPE_SRAM_256KBIT, /**< SRAM 256Kbit */
    SAVE_TYPE_FLASHRAM_1MBIT, /**< FlashRAM 1Mbit */
    SAVE_TYPE_SRAM_BANKED, /**< SRAM Banked */
    SAVE_TYPE_SRAM_1MBIT /**< SRAM 1Mbit */
} sc64_save_type_t;

/** @brief The SC64 CIC Seed Enumeration. */
typedef enum {
    CIC_SEED_AUTO = 0xFFFF /**< Automatic CIC seed */
} sc64_cic_seed_t;

/** @brief The SC64 TV Type Enumeration. */
typedef enum {
    TV_TYPE_PAL = 0, /**< PAL TV type */
    TV_TYPE_NTSC = 1, /**< NTSC TV type */
    TV_TYPE_MPAL = 2, /**< MPAL TV type */
    TV_TYPE_PASSTHROUGH = 3 /**< Passthrough TV type */
} sc64_tv_type_t;

/** @brief The SC64 Drive Type Enumeration. */
typedef enum {
    DRIVE_TYPE_RETAIL, /**< Retail drive type */
    DRIVE_TYPE_DEVELOPMENT /**< Development drive type */
} sc64_drive_type_t;

/** @brief The SC64 Disk State Enumeration. */
typedef enum {
    DISK_STATE_EJECTED, /**< Disk ejected */
    DISK_STATE_INSERTED, /**< Disk inserted */
    DISK_STATE_CHANGED /**< Disk state changed */
} sc64_disk_state_t;

/** @brief The SC64 Button Mode Enumeration. */
typedef enum {
    BUTTON_MODE_NONE, /**< No button mode */
    BUTTON_MODE_N64_IRQ, /**< N64 IRQ button mode */
    BUTTON_MODE_USB_PACKET, /**< USB packet button mode */
    BUTTON_MODE_DD_DISK_SWAP /**< Disk Drive disk swap button mode */
} sc64_button_mode_t;

/** @brief The SC64 Disk Mapping Structure. */
typedef struct {
    int count; /**< Number of disks */
    struct {
        uint32_t thb_table; /**< THB table */
        uint32_t sector_table; /**< Sector table */
    } disks[4]; /**< Array of disks */
} sc64_disk_mapping_t;

/**
 * @brief Lock the SC64.
 */
void sc64_ll_lock(void);

/**
 * @brief Get the SC64 version.
 * 
 * @param major Pointer to store the major version.
 * @param minor Pointer to store the minor version.
 * @param revision Pointer to store the revision.
 * @return sc64_error_t Error code.
 */
sc64_error_t sc64_ll_get_version(uint16_t *major, uint16_t *minor, uint32_t *revision);

/**
 * @brief Get the SC64 configuration.
 * 
 * @param cfg Configuration ID.
 * @param value Pointer to store the configuration value.
 * @return sc64_error_t Error code.
 */
sc64_error_t sc64_ll_get_config(sc64_cfg_id_t cfg, uint32_t *value);

/**
 * @brief Set the SC64 configuration.
 * 
 * @param cfg Configuration ID.
 * @param value Configuration value.
 * @return sc64_error_t Error code.
 */
sc64_error_t sc64_ll_set_config(sc64_cfg_id_t cfg, uint32_t value);

/**
 * @brief Set the SC64 disk mapping.
 * 
 * @param disk_mapping Pointer to the disk mapping structure.
 * @return sc64_error_t Error code.
 */
sc64_error_t sc64_ll_set_disk_mapping(sc64_disk_mapping_t *disk_mapping);

/**
 * @brief Check if writeback is pending.
 * 
 * @param pending Pointer to store the pending status.
 * @return sc64_error_t Error code.
 */
sc64_error_t sc64_ll_writeback_pending(bool *pending);

/**
 * @brief Enable writeback.
 * 
 * @param address Address to enable writeback.
 * @return sc64_error_t Error code.
 */
sc64_error_t sc64_ll_writeback_enable(void *address);

/**
 * @brief Wait for flash to be not busy.
 * 
 * @return sc64_error_t Error code.
 */
sc64_error_t sc64_ll_flash_wait_busy(void);

/**
 * @brief Get the flash erase block size.
 * 
 * @param erase_block_size Pointer to store the erase block size.
 * @return sc64_error_t Error code.
 */
sc64_error_t sc64_ll_flash_get_erase_block_size(size_t *erase_block_size);

/**
 * @brief Erase a flash block.
 * 
 * @param address Address of the block to erase.
 * @return sc64_error_t Error code.
 */
sc64_error_t sc64_ll_flash_erase_block(void *address);

/** @} */ /* sc64 */

#endif // FLASHCART_SC64_LL_H__
