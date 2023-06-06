#include <stdint.h>

// NOTE: these values are independent of flashcart / OS
// But by default align to SC64.
#define DB_SAVE_TYPE_NONE           0x00
#define DB_SAVE_TYPE_EEPROM_4K      0x01
#define DB_SAVE_TYPE_EEPROM_16K     0x02
#define DB_SAVE_TYPE_SRAM           0x03
#define DB_SAVE_TYPE_SRAM_BANKED    0x04
#define DB_SAVE_TYPE_SRAM_128K      0x05
#define DB_SAVE_TYPE_FLASHRAM       0x06
#define DB_SAVE_TYPE_CART_SPECIFIED 0x0f


//Rom Info
// CheckCode 0x10, 8 bytes (sometimes refered to as CRC Hi and CRC Lo)
// GameTitle 0x20, 14 bytes
// GameCode ->
//    CategoryCode 0x3b
//    UniqueCode 0x3c and 0x3d
//    DestinationCode 0x3e
// RomVersion 0x3f

typedef struct {
    char category_code;
    uint16_t unique_code;
    char destination_code;
} game_code_t;

typedef struct {
    uint64_t check_code;
    char game_title[14];
    game_code_t game_code;
    char version;
} rom_header_t;


uint8_t rom_db_match_save_type(rom_header_t rom_header);
