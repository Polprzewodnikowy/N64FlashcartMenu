/**
 * @file ed64_ll.h
 * @brief ED64 V-Series Flashcart Internal Utilities
 * @ingroup flashcart 
 */

#ifndef FLASHCART_ED64_LL_H__
#define FLASHCART_ED64_LL_H__


#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @addtogroup ed64
 * @{
 */


typedef enum {
    SAVE_TYPE_NONE = 0,
    SAVE_TYPE_SRAM = 1,
    SAVE_TYPE_SRAM_128K = 2,
    SAVE_TYPE_EEPROM_4K = 3,
    SAVE_TYPE_EEPROM_16K = 4,
    SAVE_TYPE_FLASHRAM = 5,
    SAVE_TYPE_CPAK = 8,
    SAVE_TYPE_DD64 = 16,
} ed64_save_type_t;


// typedef struct {
//     uint16_t assembily_date;  /* The Date that the ED64 was manufactured */
//     uint16_t assembily_time;  /* The Time that the ED64 was manufactured */
//     uint16_t serial_number;   /* The ED64 serial number (unique for each ED64) */
//     bool cic_6105_compatible; /* This returns whether the the cart CIC is 6105 compatible
//                                  as many were produced before Ultra CIC existed!
//                                  Carts that are not compatible will be unable to run certain ROMs */

// } ed64_cart_metadata_s;


#define SRAM_ADDRESS (0xA8000000)
#define ROM_ADDRESS  (0xB0000000)

// #define USB_ERROR_FIFO_TIMEOUT 0x90
// #define EVD_ERROR_MMC_TIMEOUT 0x91

// #define BOOT_UPD_ERR_WRONG_SIZE 0x95
// #define BOOT_UPD_ERR_HDR 0x96
// #define BOOT_UPD_ERR_CMP 0x97
// #define BOOT_UPD_ERR_CIC_DTCT 0x98


/* Initialization functions */
int ed64_ll_init();
void ed64_ll_init_v2();
void ed64_ll_init_v3();

/* Device information functions */
uint16_t ed64_ll_get_fpga_version();
uint16_t ed64_ll_get_cpld_version();

/* Firmware update functions */
void ed64_ll_load_firmware(uint8_t *firmware);

// /* USB functions */
// uint8_t ed64_ll_usb_read_busy();
// uint8_t ed64_ll_usb_read(uint32_t start_address, uint32_t slen);
// uint8_t ed64_ll_usb_write(uint32_t start_address, uint32_t slen);

/* Save functions */
void ed64_ll_set_ram_bank(uint8_t bank);
ed64_save_type_t ed64_ll_get_save_type();
void ed64_ll_set_save_type(ed64_save_type_t type);

/* reads metadata related to the assembily date and cart capabilities */
void ed64_ll_read_cart_metadata(void *dest);

uint16_t ed64_ll_msg_rd();
void ed64_ll_msg_wr(uint16_t val);

void ed64_ll_lock_regs();
void ed64_ll_unlock_regs();

void ed64_ll_reset_spx();

/* GPIO functions */
void ed64_ll_gpio_mode_rtc();
void ed64_ll_gpio_mode_io();
void ed64_ll_gpio_mode_off();
uint8_t ed64_ll_gpio_read();

// /* 64DD cart conversion save functions */
// void ed64_ll_64dd_ram_oe();
// void ed64_ll_64dd_ram_we();
// void ed64_ll_64dd_ram_off();
// void ed64_ll_64dd_ram_clear();
// uint8_t ed64_ll_get_64dd_ram_supported();

/** @} */ /* ed64 */

#endif


