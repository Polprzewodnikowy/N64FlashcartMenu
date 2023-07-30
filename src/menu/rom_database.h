/**
 * @file rom_database.h
 * @brief N64 ROM Database
 * @note Only works with N64 ROM's by checking the first 1024 bytes
 * @ingroup menu 
 */

#ifndef ROM_DATABASE_H__
#define ROM_DATABASE_H__

#include <stdint.h>


/**
 * @brief ROM Database save type enumeration 
 * @note these values are independent of flashcart / OS
 * But by default align to SC64.
*/
typedef enum {
    /** @brief The ROM has no save type */
    DB_SAVE_TYPE_NONE =          0x00,
    /** @brief The ROM uses EEPROM 4K saves */
    DB_SAVE_TYPE_EEPROM_4K =     0x01,
    /** @brief The ROM uses EEPROM 16K saves */
    DB_SAVE_TYPE_EEPROM_16K =    0x02,
    /** @brief The ROM uses SRAM saves */
    DB_SAVE_TYPE_SRAM =          0x03,
    /** @brief The ROM uses SRAM Banked saves */
    DB_SAVE_TYPE_SRAM_BANKED =   0x04,
    /** @brief The ROM uses SRAM 128K saves @note This is not supported by all flashcarts */
    DB_SAVE_TYPE_SRAM_128K =     0x05,
    /** @brief The ROM uses FLASHRAM saves */
    DB_SAVE_TYPE_FLASHRAM =      0x06,
    /** @brief The ROM uses CPAK saves @note This must be handled by user code. */
    DB_SAVE_TYPE_CPAK =          0x10,
    /** @brief The ROM uses Disk Drive saves @note This is not supported by all flashcarts */
    DB_SAVE_TYPE_DD =            0x20,
    /** @brief The ROM uses Disk Drive conversion saves @note This must be handled by user code. */
    DB_SAVE_TYPE_DD_CONVERSION = 0x30,
    /** @brief The ROM uses a save type that was not recognised */
    DB_SAVE_TYPE_INVALID =       0xff,
 } db_savetype_t;


/** @brief ROM System Memory requirements enumeration */
typedef enum {
    /** @brief The ROM is happy with 4MB of memory */
    DB_MEMORY_EXPANSION_NONE =        0x00,
    /** @brief The ROM requires 8MB of memory */
    DB_MEMORY_EXPANSION_REQUIRED =    0x01,
    /** @brief The ROM recommends 8MB of memory */
    DB_MEMORY_EXPANSION_RECOMMENDED = 0x02,
    /** @brief The ROM suggests 8MB of memory */
    DB_MEMORY_EXPANSION_SUGGESTED =   0x03,
    /** @brief The ROM is faulty when using 8MB of memory */
    DB_MEMORY_EXPANSION_FAULTY =      0x04,
} rom_memorytype_t;


/**
 * @brief ROM Homebrew save type enumeration
 * @note these align to the Krikzz ED64 save types and are generally accepted
 * by all emulators.
 * 
*/
typedef enum {
    /** @brief The ROM has no save type */
    HB_SAVE_TYPE_NONE =         0x00,
    /** @brief The ROM uses EEPROM 4K saves */
    HB_SAVE_TYPE_EEPROM_4K =    0x01,
    /** @brief The ROM uses EEPROM 16K saves */
    HB_SAVE_TYPE_EEPROM_16K =   0x02,
    /** @brief The ROM uses SRAM saves */
    HB_SAVE_TYPE_SRAM =         0x03,
    /** @brief The ROM uses SRAM Banked saves */
    HB_SAVE_TYPE_SRAM_BANKED =  0x04,
    /** @brief The ROM uses FLASHRAM saves */
    HB_SAVE_TYPE_FLASHRAM =     0x05,
    /** @brief The ROM uses SRAM 128K saves @note This is not supported by all flashcarts */
    HB_SAVE_TYPE_SRAM_128K =    0x06,
} homebrew_savetype_t;

/**
 * @brief ROM file endian enumeration
 * 
 * @note this is a hack used for checking ROM's against expected Big Endian
 * when reading from the file system.
 */
typedef enum {
    ROM_BIG_ENDIAN =        0x80371240,
    ROM_LITTLE_ENDIAN =     0x40123780,
    ROM_MID_BIG_ENDIAN =    0x37804012,
    ROM_MID_LITTLE_ENDIAN = 0x12408037,
    IPL_BIG_ENDIAN =        0x80270740,
} rom_endian_type_t;


/** @brief ROM media type enumeration */
typedef enum {
    /** @brief Is a stand alone Cartridge program */
    N64_CART = 'N',
    /** @brief Is a stand alone Disk Drive program */
    N64_DISK  = 'D',
    /** @brief Is a Cartridge program that could use an extra Disk Drive program to expand its capabilities */
    N64_CART_EXPANDABLE = 'C',
    /** @brief Is a Disk Drive program that could use an extra Cartridge program to expand its capabilities */
    N64_DISK_EXPANDABLE = 'E',
    /** @brief Is an Aleck64 program */
    N64_ALECK64  = 'Z'
} rom_media_type_t;

/** @brief ROM market type enumeration */
typedef enum {
    /** @brief The ROM is designed for all regions */
    MARKET_ALL = 'A',
    /** @brief The ROM is designed for Brazil */
    MARKET_BRAZIL  = 'B',
    /** @brief The ROM is designed for China */
    MARKET_CHINA = 'C',
    /** @brief The ROM is designed for Germany */
    MARKET_GERMANY = 'D',
    /** @brief The ROM is designed for USA */
    MARKET_USA  = 'E',
    /** @brief The ROM is designed for France */
    MARKET_FRANCE = 'F',
    MARKET_GATEWAY64_NTSC = 'G',
    /** @brief The ROM is designed for Netherlands */
    MARKET_NETHERLANDS = 'H',
    /** @brief The ROM is designed for Italy */
    MARKET_ITALY = 'I',
    /** @brief The ROM is designed for Japan */
    MARKET_JAPAN = 'J',
    /** @brief The ROM is designed for Korea */
    MARKET_KOREA = 'K',
    MARKET_GATEWAY64_PAL = 'L',
    // MARKET_UNKNOWN_M = 'M',
    /** @brief The ROM is designed for Canada */
    MARKET_CANADA = 'N',
    // MARKET_UNKNOWN_O = 'O',
    MARKET_PAL_GENERIC = 'P',
    // MARKET_UNKNOWN_Q = 'Q',
    // MARKET_UNKNOWN_R = 'R',
    /** @brief The ROM is designed for Spain */
    MARKET_SPAIN = 'S',
    // MARKET_UNKNOWN_T = 'T',
    /** @brief The ROM is designed for Australia */
    MARKET_AUSTRAILA = 'U',
    // MARKET_UNKNOWN_V = 'V',
    /** @brief The ROM is designed for Scandinavia */
    MARKET_SCANDINAVAIA = 'W',
    MARKET_PAL_X = 'X',
    MARKET_PAL_Y = 'Y',
    MARKET_PAL_Z = 'Z'
} rom_destination_market_t;


/**
 * @brief ROM Metadata Structure
 * @note This information is derived from the ROM header. i.e.
 * 0x3B = Media Type
 * 0x3C and 0x3D = Unique Identifier
 * 0x3E = Destination Market
 */
typedef struct {
    uint8_t media_type; // rom_media_type_t
    uint16_t unique_identifier;
    uint8_t destination_market; // rom_destination_market_t
} rom_metadata_t;

/** 
 * @brief ROM Header Structure
 * @note This information is derived from the ROM header. @see https://n64brew.dev/wiki/ROM_Header
 */
typedef struct {
    /** @brief The ROM configuration flags @note we currently use this to work out the endian @see rom_endian_type_t. */
    uint32_t config_flags;

    // uint32_t clock_rate;
    // uint32_t boot_address;
    // uint32_t sdk_version;

    /** @brief The ROM file checksum */
    uint64_t checksum;

    // uint64_t unknown_reserved_1;

    /** @brief The ROM file title */
    char title[21]; // 20 chars + null
    /** @brief The ROM file metadata */

    // char unknown_reserved_2[7];

    /** @brief The ROM file metadata @see rom_metadata_t */
    rom_metadata_t metadata;
    /** @brief The ROM file release version */
    uint8_t version;

    // char ipl3_boot_code[0xFC0];

} rom_header_t;

#ifdef __cplusplus
extern "C" {
#endif

rom_header_t file_read_rom_header(char *path);
uint8_t rom_db_match_save_type(rom_header_t rom_header);
uint8_t rom_db_match_expansion_pak(rom_header_t rom_header);

#ifdef __cplusplus
}
#endif

#endif
