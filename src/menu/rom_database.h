#include <stdint.h>

// NOTE: these values are independent of flashcart / OS
#define DB_SAVE_TYPE_NONE           0x00
#define DB_SAVE_TYPE_EEPROM_4K      0x01
#define DB_SAVE_TYPE_EEPROM_16K     0x02
#define DB_SAVE_TYPE_SRAM           0x03
#define DB_SAVE_TYPE_FLASHRAM       0x04
#define DB_SAVE_TYPE_SRAM_BANKED    0x05
#define DB_SAVE_TYPE_CART_SPECIFIED 0x0f

uint8_t rom_db_match_save_type(uint16_t id, uint32_t crc);
