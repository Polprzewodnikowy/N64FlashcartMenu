#ifndef FLASHCART_SC64_INTERNAL_H__
#define FLASHCART_SC64_INTERNAL_H__


#include <stddef.h>
#include <stdint.h>


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
    CFG_ISV_ENABLE,
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
    SAVE_TYPE_NONE,
    SAVE_TYPE_EEPROM_4K,
    SAVE_TYPE_EEPROM_16K,
    SAVE_TYPE_SRAM,
    SAVE_TYPE_FLASHRAM,
    SAVE_TYPE_SRAM_BANKED,
} sc64_save_type_t;


void sc64_unlock (void);
void sc64_lock (void);
bool sc64_check_presence (void);
void sc64_read_data (void *src, void *dst, size_t length);
void sc64_write_data (void *src, void *dst, size_t length);
sc64_error_t sc64_get_api_version (uint32_t *api_version);
sc64_error_t sc64_get_config (sc64_cfg_t cfg, void *value);
sc64_error_t sc64_set_config (sc64_cfg_t cfg, uint32_t value);
sc64_error_t sc64_writeback_enable (void *address);
sc64_error_t sc64_flash_program (void *address, size_t length);
sc64_error_t sc64_flash_wait_busy (void);
sc64_error_t sc64_flash_get_erase_block_size (size_t *erase_block_size);
sc64_error_t sc64_flash_erase_block (void *address);


#endif
