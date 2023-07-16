/**
 * @file flashcart.h
 * @brief SC64 Flashcart Internal Utilities
 * @ingroup flashcart 
 */

#ifndef FLASHCART_SC64_INTERNAL_H__
#define FLASHCART_SC64_INTERNAL_H__


#include <stddef.h>
#include <stdint.h>

/**
 * @addtogroup sc64
 * @{
 */

typedef struct {
    uint8_t BUFFER[8192];
    uint8_t EEPROM[2048];
    uint8_t DD_SECTOR[256];
    uint8_t FLASHRAM[128];
} sc64_buffers_t;

#define SC64_BUFFERS_BASE       (0x1FFE0000UL)
#define SC64_BUFFERS            ((sc64_buffers_t *) SC64_BUFFERS_BASE)


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
    CFG_BOOTLOADER_SWITCH,
    CFG_ROM_WRITE_ENABLE,
    CFG_ROM_SHADOW_ENABLE,
    CFG_DD_MODE,
    CFG_ISV_ADDRESS,
    CFG_BOOT_MODE,
    CFG_SAVE_TYPE,
    CFG_CIC_SEED,
    CFG_TV_TYPE,
    CFG_DD_SD_ENABLE,
    CFG_DD_DRIVE_TYPE,
    CFG_DD_DISK_STATE,
    CFG_BUTTON_STATE,
    CFG_BUTTON_MODE,
    CFG_ROM_EXTENDED_ENABLE,
} sc64_cfg_t;

typedef enum {
    DD_MODE_DISABLED = 0,
    DD_MODE_REGS = 1,
    DD_MODE_IPL = 2,
    DD_MODE_FULL = 3
} sc64_dd_mode_t;

typedef enum {
    BOOT_MODE_MENU = 0,
    BOOT_MODE_ROM = 1,
    BOOT_MODE_DDIPL = 2,
    BOOT_MODE_DIRECT_ROM = 3,
    BOOT_MODE_DIRECT_DDIPL = 4,
} sc64_boot_mode_t;

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


void sc64_unlock (void);
void sc64_lock (void);
bool sc64_check_presence (void);
void sc64_read_data (void *src, void *dst, size_t length);
void sc64_write_data (void *src, void *dst, size_t length);
sc64_error_t sc64_get_version (uint16_t *major, uint16_t *minor);
sc64_error_t sc64_get_config (sc64_cfg_t cfg, void *value);
sc64_error_t sc64_set_config (sc64_cfg_t cfg, uint32_t value);
sc64_error_t sc64_sd_set_byte_swap (bool enabled);
sc64_error_t sc64_writeback_pending (bool *pending);
sc64_error_t sc64_writeback_enable (void *address);
sc64_error_t sc64_flash_program (void *address, size_t length);
sc64_error_t sc64_flash_wait_busy (void);
sc64_error_t sc64_flash_get_erase_block_size (size_t *erase_block_size);
sc64_error_t sc64_flash_erase_block (void *address);

/** @} */ /* sc64 */

#endif
