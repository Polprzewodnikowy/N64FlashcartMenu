#ifndef ROM_DATABASE_H__
#define ROM_DATABASE_H__

#include <stdint.h>

// NOTE: these values are independent of flashcart / OS
// But by default align to SC64.
typedef enum {
    DB_SAVE_TYPE_NONE =          0x00,
    DB_SAVE_TYPE_EEPROM_4K =     0x01,
    DB_SAVE_TYPE_EEPROM_16K =    0x02,
    DB_SAVE_TYPE_SRAM =          0x03,
    DB_SAVE_TYPE_SRAM_BANKED =   0x04,
    DB_SAVE_TYPE_SRAM_128K =     0x05,
    DB_SAVE_TYPE_FLASHRAM =      0x06,
    DB_SAVE_TYPE_CPAK =          0x10,
    DB_SAVE_TYPE_DD =            0x20,
    DB_SAVE_TYPE_INVALID =       0xff,
 } db_savetype_t;

typedef enum {
    DB_MEMORY_EXPANSION_NONE =        0x00,
    DB_MEMORY_EXPANSION_REQUIRED =    0x01,
    DB_MEMORY_EXPANSION_RECOMMENDED = 0x02,
    DB_MEMORY_EXPANSION_SUGGESTED =   0x03,
    DB_MEMORY_EXPANSION_FAULTY =      0x04,
} rom_memorytype_t;


typedef enum {
    HB_SAVE_TYPE_NONE =         0x00,
    HB_SAVE_TYPE_EEPROM_4K =    0x01,
    HB_SAVE_TYPE_EEPROM_16K =   0x02,
    HB_SAVE_TYPE_SRAM =         0x03,
    HB_SAVE_TYPE_SRAM_BANKED =  0x04,
    HB_SAVE_TYPE_FLASHRAM =     0x05,
    HB_SAVE_TYPE_SRAM_128K =    0x06,
} homebrew_savetype_t;

typedef enum {
    ROM_BIG_ENDIAN =        0x80371240,
    ROM_LITTLE_ENDIAN =     0x40123780,
    ROM_MID_BIG_ENDIAN =    0x37804012,
    ROM_MID_LITTLE_ENDIAN = 0x12408037,
    IPL_BIG_ENDIAN =        0x80270740,
} rom_endian_type_t;

//Rom Info
// CheckCode 0x10, 8 bytes (sometimes refered to as CRC Hi and CRC Lo)
// GameTitle 0x20, 20 bytes
// GameCode ->
//    CategoryCode 0x3b
//    UniqueCode 0x3c and 0x3d
//    DestinationCode 0x3e
// RomVersion 0x3f

typedef enum {
    N64_CART = 'N',
    N64_DISK  = 'D',
    N64_CART_EXPANDABLE = 'C',
    N64_DISK_EXPANDABLE = 'E',
    N64_ALECK64  = 'Z'
} rom_media_type_t;

typedef enum {
    MARKET_ALL = 'A',
    MARKET_BRAZIL  = 'B',
    MARKET_CHINA = 'C',
    MARKET_GERMANY = 'D',
    MARKET_USA  = 'E',
    MARKET_FRANCE = 'F',
    MARKET_GATEWAY64_NTSC = 'G',
    MARKET_NETHERLANDS = 'H',
    MARKET_ITALY = 'I',
    MARKET_JAPAN = 'J',
    MARKET_KOREA = 'K',
    MARKET_GATEWAY64_PAL = 'L',
    // MARKET_UNKNOWN_M = 'M',
    MARKET_CANADA = 'N',
    // MARKET_UNKNOWN_O = 'O',
    MARKET_EUROPE_P = 'P',
    // MARKET_UNKNOWN_Q = 'Q',
    // MARKET_UNKNOWN_R = 'R',
    MARKET_SPAIN = 'S',
    // MARKET_UNKNOWN_T = 'T',
    MARKET_AUSTRAILA = 'U',
    // MARKET_UNKNOWN_V = 'V',
    MARKET_SCANDINAVAIA = 'W',
    MARKET_EUROPE_X = 'X',
    MARKET_EUROPE_Y = 'Y',
    MARKET_EUROPE_Z = 'Z'
} rom_destination_market_t;


typedef struct {
    uint8_t media_type; // rom_media_type_t
    uint16_t unique_identifier;
    uint8_t destination_market; // rom_destination_market_t
} rom_metadata_t;

typedef struct {
    uint32_t endian; // rom_endian_type_t
    uint64_t checksum;
    char title[21]; // 20 chars + null
    rom_metadata_t metadata;
    uint8_t version;
} rom_header_t;

rom_header_t file_read_rom_header(char *path);
uint8_t rom_db_match_save_type(rom_header_t rom_header);
uint8_t rom_db_match_expansion_pak(rom_header_t rom_header);

#endif
