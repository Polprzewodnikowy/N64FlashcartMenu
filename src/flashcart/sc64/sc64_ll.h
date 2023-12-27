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
    uint8_t BUFFER[8192];
    uint8_t EEPROM[2048];
    uint8_t DD_SECTOR[256];
    uint8_t FLASHRAM[128];
} sc64_buffers_t;

#define SC64_BUFFERS_BASE       (0x1FFE0000UL)
#define SC64_BUFFERS            ((sc64_buffers_t *) SC64_BUFFERS_BASE)

/** @brief The SC64 State Enumeration. */
typedef enum {
    SC64_OK,
    SC64_ERROR_BAD_ARGUMENT,
    SC64_ERROR_BAD_ADDRESS,
    SC64_ERROR_BAD_CONFIG_ID,
    SC64_ERROR_TIMEOUT,
    SC64_ERROR_SD_CARD,
    SC64_ERROR_UNKNOWN_CMD = -1
} sc64_error_t;

typedef enum {
    CFG_ID_BOOTLOADER_SWITCH,
    CFG_ID_ROM_WRITE_ENABLE,
    CFG_ID_ROM_SHADOW_ENABLE,
    CFG_ID_DD_MODE,
    CFG_ID_ISV_ADDRESS,
    CFG_ID_BOOT_MODE,
    CFG_ID_SAVE_TYPE,
    CFG_ID_CIC_SEED,
    CFG_ID_TV_TYPE,
    CFG_ID_DD_SD_ENABLE,
    CFG_ID_DD_DRIVE_TYPE,
    CFG_ID_DD_DISK_STATE,
    CFG_ID_BUTTON_STATE,
    CFG_ID_BUTTON_MODE,
    CFG_ID_ROM_EXTENDED_ENABLE,
} sc64_cfg_id_t;

typedef enum {
    DD_MODE_DISABLED = 0,
    DD_MODE_REGS = 1,
    DD_MODE_IPL = 2,
    DD_MODE_FULL = 3
} sc64_dd_mode_t;

/** @brief The SC64 Boot Mode Enumeration. */
typedef enum {
    BOOT_MODE_MENU = 0,
    BOOT_MODE_ROM = 1,
    BOOT_MODE_DDIPL = 2,
    BOOT_MODE_DIRECT_ROM = 3,
    BOOT_MODE_DIRECT_DDIPL = 4,
} sc64_boot_mode_t;

/** @brief The SC64 Save Type Enumeration. */
typedef enum {
    SAVE_TYPE_NONE,
    SAVE_TYPE_EEPROM_4K,
    SAVE_TYPE_EEPROM_16K,
    SAVE_TYPE_SRAM,
    SAVE_TYPE_FLASHRAM,
    SAVE_TYPE_SRAM_BANKED,
    SAVE_TYPE_SRAM_128K,
} sc64_save_type_t;

typedef enum {
    CIC_SEED_AUTO = 0xFFFF
} sc64_cic_seed_t;

typedef enum {
    TV_TYPE_PAL = 0,
    TV_TYPE_NTSC = 1,
    TV_TYPE_MPAL = 2,
    TV_TYPE_PASSTHROUGH = 3
} sc64_tv_type_t;

typedef enum {
    DRIVE_TYPE_RETAIL,
    DRIVE_TYPE_DEVELOPMENT,
} sc64_drive_type_t;

typedef enum {
    DISK_STATE_EJECTED,
    DISK_STATE_INSERTED,
    DISK_STATE_CHANGED,
} sc64_disk_state_t;

typedef enum {
    BUTTON_MODE_NONE,
    BUTTON_MODE_N64_IRQ,
    BUTTON_MODE_USB_PACKET,
    BUTTON_MODE_DD_DISK_SWAP,
} sc64_button_mode_t;

typedef struct {
    int count;
    struct {
        uint32_t thb_table;
        uint32_t sector_table;
    } disks[4];
} sc64_disk_mapping_t;


void sc64_ll_lock (void);
sc64_error_t sc64_ll_get_version (uint16_t *major, uint16_t *minor, uint32_t *revision);
sc64_error_t sc64_ll_get_config (sc64_cfg_id_t cfg, uint32_t *value);
sc64_error_t sc64_ll_set_config (sc64_cfg_id_t cfg, uint32_t value);
sc64_error_t sc64_ll_set_disk_mapping (sc64_disk_mapping_t *disk_mapping);
sc64_error_t sc64_ll_writeback_pending (bool *pending);
sc64_error_t sc64_ll_writeback_enable (void *address);
sc64_error_t sc64_ll_flash_wait_busy (void);
sc64_error_t sc64_ll_flash_get_erase_block_size (size_t *erase_block_size);
sc64_error_t sc64_ll_flash_erase_block (void *address);

/** @} */ /* sc64 */


#endif
