/**
 * @file rom_database.h
 * @brief ROM Database
 * @ingroup menu 
 */

#ifndef ROM_DATABASE_H__
#define ROM_DATABASE_H__

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
#define DB_SAVE_TYPE_CPAK           0x10
#define DB_SAVE_TYPE_DD             0x20
#define DB_SAVE_TYPE_INVALID        0xff

/** @brief ROM System Memory requirements enumeration */
typedef enum {
    /** @brief The ROM is happy with 4K of memory */
    DB_MEMORY_EXPANSION_NONE =      0x00,
    /** @brief The ROM requires 8K of memory */
    DB_MEMORY_EXPANSION_REQUIRED =  0x01,
    DB_MEMORY_EXPANSION_SUGGESTED = 0x02,
    DB_MEMORY_EXPANSION_ENHANCED =  0x03,
    /** @brief The ROM is faulty when using 8K of memory */
    DB_MEMORY_EXPANSION_FAULTY =    0x04,
} rom_memorytype_t;


/** @brief N64 ROM Homebrew save type enumeration */
typedef enum {
    HB_SAVE_TYPE_NONE =         0x00,
    HB_SAVE_TYPE_EEPROM_4K =    0x01,
    HB_SAVE_TYPE_EEPROM_16K =   0x02,
    HB_SAVE_TYPE_SRAM =         0x03,
    HB_SAVE_TYPE_SRAM_BANKED =  0x04,
    HB_SAVE_TYPE_FLASHRAM =     0x05,
    HB_SAVE_TYPE_SRAM_128K =    0x06,
} homebrew_savetype_t;

/** @brief N64 ROM endian enumeration */
typedef enum {
    ROM_BIG_ENDIAN =        0x80371240,
    ROM_LITTLE_ENDIAN =     0x40123780,
    ROM_MID_BIG_ENDIAN =    0x37804012,
    ROM_MID_LITTLE_ENDIAN = 0x12408037,
    IPL_BIG_ENDIAN =        0x80270740,
} rom_endian_type_t;


/** @brief N64 ROM media type enumeration */
typedef enum {
    N64_CART = 'N',
    N64_DISK  = 'D',
    N64_CART_EXPANDABLE = 'C',
    N64_DISK_EXPANDABLE = 'E',
    N64_ALECK64  = 'Z'
} rom_media_type_t;

/** @brief N64 ROM market type enumeration */
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


/** @brief N64 ROM Metadata Structure */
typedef struct {
    uint8_t media_type; // rom_media_type_t
    uint16_t unique_identifier;
    uint8_t destination_market; // rom_destination_market_t
} rom_metadata_t;

/** @brief N64 ROM Header Structure */
typedef struct {
    /** @brief The N64 ROM file endian */
    uint32_t endian; // rom_endian_type_t
    /** @brief The N64 ROM file checksum */
    uint64_t checksum;
    /** @brief The N64 ROM file title */
    char title[21]; // 20 chars + null
    /** @brief The N64 ROM file metadata */
    rom_metadata_t metadata;
    /** @brief The N64 ROM file version */
    uint8_t version;
} rom_header_t;

rom_header_t file_read_rom_header(char *path);
uint8_t rom_db_match_save_type(rom_header_t rom_header);
uint8_t rom_db_match_expansion_pak(rom_header_t rom_header);

#endif
