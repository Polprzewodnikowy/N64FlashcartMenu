/**
 * @file rom_database.h
 * @brief N64 ROM Database.
 * @note Only works with N64 ROM's by checking the first 1024 bytes of the file.
 * @ingroup menu 
 */

#ifndef ROM_DATABASE_H__
#define ROM_DATABASE_H__

#include <stdint.h>


/**
 * @brief ROM database save type enumeration.
 * @note These values are independent of flashcart / OS
 * but by default align to SC64.
*/
typedef enum {
    /** @brief The ROM has no save type. */
    DB_SAVE_TYPE_NONE =          0x00,
    /** @brief The ROM uses EEPROM 4K saves. */
    DB_SAVE_TYPE_EEPROM_4K =     0x01,
    /** @brief The ROM uses EEPROM 16K saves. */
    DB_SAVE_TYPE_EEPROM_16K =    0x02,
    /** @brief The ROM uses SRAM saves. */
    DB_SAVE_TYPE_SRAM =          0x03,
    /** @brief The ROM uses SRAM Banked saves. */
    DB_SAVE_TYPE_SRAM_BANKED =   0x04,
    /** @brief The ROM uses SRAM 128K saves @note This is not supported by all flashcarts. */
    DB_SAVE_TYPE_SRAM_128K =     0x05,
    /** @brief The ROM uses FLASHRAM saves. */
    DB_SAVE_TYPE_FLASHRAM =      0x06,
    /** @brief The ROM uses CPAK saves @note This must be handled by user code. */
    DB_SAVE_TYPE_CPAK =          0x10,
    /** @brief The ROM uses Disk Drive saves @note This is not supported by all flashcarts. */
    DB_SAVE_TYPE_DD =            0x20,
    /** @brief The ROM uses Disk Drive conversion saves @note This must be handled by user code. */
    DB_SAVE_TYPE_DD_CONVERSION = 0x30,
    /** @brief The ROM uses a save type that was not recognised. */
    DB_SAVE_TYPE_INVALID =       0xFF,
 } db_savetype_t;


/** @brief ROM system memory requirements enumeration. */
typedef enum {
    /** @brief The ROM is happy with 4MB of memory. */
    DB_MEMORY_EXPANSION_NONE =        0x00,
    /** @brief The ROM requires 8MB of memory. */
    DB_MEMORY_EXPANSION_REQUIRED =    0x01,
    /** @brief The ROM recommends 8MB of memory. */
    DB_MEMORY_EXPANSION_RECOMMENDED = 0x02,
    /** @brief The ROM suggests 8MB of memory. */
    DB_MEMORY_EXPANSION_SUGGESTED =   0x03,
    /** @brief The ROM is faulty when using 8MB of memory. */
    DB_MEMORY_EXPANSION_FAULTY =      0x04,
} rom_memorytype_t;


/**
 * @brief ROM homebrew save type enumeration.
 * @note These align to the Krikzz ED64 save types and are generally accepted
 * by all emulators.
 * 
*/
typedef enum {
    /** @brief The ROM has no save type. */
    HB_SAVE_TYPE_NONE =         0x00,
    /** @brief The ROM uses EEPROM 4K saves. */
    HB_SAVE_TYPE_EEPROM_4K =    0x01,
    /** @brief The ROM uses EEPROM 16K saves. */
    HB_SAVE_TYPE_EEPROM_16K =   0x02,
    /** @brief The ROM uses SRAM saves. */
    HB_SAVE_TYPE_SRAM =         0x03,
    /** @brief The ROM uses SRAM Banked saves. */
    HB_SAVE_TYPE_SRAM_BANKED =  0x04,
    /** @brief The ROM uses FLASHRAM saves. */
    HB_SAVE_TYPE_FLASHRAM =     0x05,
    /** @brief The ROM uses SRAM 128K saves @note This is not supported by all flashcarts. */
    HB_SAVE_TYPE_SRAM_128K =    0x06,
} homebrew_savetype_t;

/**
 * @brief ROM file endian enumeration.
 * 
 * @note this is a hack used for checking ROM's against expected Big Endian
 * when reading from the file system.
 */
typedef enum {
    /** @brief Big Endian ROM */
    ROM_BIG_ENDIAN =        0x80371240,
    /** @brief Little Endian ROM */
    ROM_LITTLE_ENDIAN =     0x40123780,
    /** @brief Mid Big Endian ROM */
    ROM_MID_BIG_ENDIAN =    0x37804012,
    /** @brief Mid Little Endian ROM */
    ROM_MID_LITTLE_ENDIAN = 0x12408037,
    /** @brief Big Endian IPL ROM */
    IPL_BIG_ENDIAN =        0x80270740,
} rom_endian_type_t;


/** @brief ROM media type enumeration. */
typedef enum {
    /** @brief Is a stand alone Cartridge program. */
    N64_CART = 'N',
    /** @brief Is a stand alone Disk Drive program. */
    N64_DISK  = 'D',
    /** @brief Is a Cartridge program that could use an extra Disk Drive program to expand its capabilities. */
    N64_CART_EXPANDABLE = 'C',
    /** @brief Is a Disk Drive program that could use an extra Cartridge program to expand its capabilities. */
    N64_DISK_EXPANDABLE = 'E',
    /** @brief Is an Aleck64 program. */
    N64_ALECK64  = 'Z'
} rom_media_type_t;

/** @brief ROM market region & language type enumeration. */
typedef enum {
    /** @brief The ROM is designed for Japanese and "English" languages. */
    MARKET_JAPANESE_MULTI = 'A', // 1080 Snowboarding JPN is the only ROM that uses this? possibily a mistake, or the fact it also includes American English!.
    /** @brief The ROM is designed for Brazil (Portuguese) language. */
    MARKET_BRAZILIAN  = 'B',
    /** @brief The ROM is designed for Chinese language. */
    MARKET_CHINESE = 'C',
    /** @brief The ROM is designed for German language. */
    MARKET_GERMAN = 'D',
    /** @brief The ROM is designed for North American "English" language. */
    MARKET_NORTH_AMERICA  = 'E',
    /** @brief The ROM is designed for French language. */
    MARKET_FRENCH = 'F',
    /** @brief The ROM is designed for a NTSC Gateway 64. */
    MARKET_GATEWAY64_NTSC = 'G',
    /** @brief The ROM is designed for Dutch language. */
    MARKET_DUTCH = 'H',
    /** @brief The ROM is designed for Italian language. */
    MARKET_ITALIAN = 'I',
    /** @brief The ROM is designed for Japanese language. */
    MARKET_JAPANESE = 'J',
    /** @brief The ROM is designed for Korean language. */
    MARKET_KOREAN = 'K',
    /** @brief The ROM is designed for a PAL Gateway 64. */
    MARKET_GATEWAY64_PAL = 'L',
    /** @brief The ROM is designed for Canada region (English and French) language. */
    MARKET_CANADIAN = 'N',
    /** @brief The ROM is designed for European market and languages (must at minimum include English). */
    MARKET_EUROPEAN_BASIC = 'P', // Sometimes used for Australian region ROMs as well.
    /** @brief The ROM is designed for Spanish language */
    MARKET_SPANISH = 'S',
    /** @brief The ROM is designed for Australia (English) language. */
    MARKET_AUSTRALIAN = 'U',
    /** @brief The ROM is designed for Scandinavian (Swedish, Norwegian, Finnish, etc.) languages. */
    MARKET_SCANDINAVIAN = 'W',
    /** @brief The ROM is designed for an undefined region and TBD language(s). */
    MARKET_OTHER_X = 'X', // many EU ROM's, Top Gear Rally (Asia) and HSV Racing (AUS) ROM uses this.
    /** @brief The ROM is designed for a European region and language(s). */
    MARKET_OTHER_Y = 'Y', // many EU ROM's uses this.
    /** @brief The ROM is designed for an undefined region and TBD language(s). */
    MARKET_OTHER_Z = 'Z' // no known ROM's use this.
} rom_destination_market_t;


/**
 * @brief ROM Config Flags Structure
 * @note This information is derived from the ROM header. 
 * @see https://n64brew.dev/wiki/Peripheral_Interface#Domains
 * i.e.
 * 0x00 = PI BSD Domain 1 Release register
 * 0x01 = PI BSD Domain 1 Page Size register
 * 0x02 = PI BSD Domain 1 Pulse Width register
 * 0x03 = PI BSD Domain 1 Latch register
 */
typedef struct {
    /* PI BSD Domain 1 Release register value */
    uint8_t domain1_release;
    /* PI BSD Domain 1 Page Size register value */
    uint8_t domain1_page_size;
    /* PI BSD Domain 1 Pulse Width register value */
    uint8_t domain1_latency;
    /* PI BSD Domain 1 Latch register value */
    uint8_t domain1_pulse_width;
} rom_config_flags_t;

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
    uint8_t version;
} rom_metadata_t;

/** 
 * @brief ROM Header Structure
 * @note This information is derived from the ROM header. @see https://n64brew.dev/wiki/ROM_Header
 */
typedef struct {
    /** @brief The ROM configuration flags @note we currently use this to work out the endian @see rom_endian_type_t. */
    uint32_t config_flags; // TODO: use rom_config_flags_t

    /** @brief The ROM file clock rate. */
    uint32_t clock_rate;
    /** @brief The ROM file boot address. */
    uint32_t boot_address;
    /** @brief The ROM file SDK version. */
    uint32_t sdk_version;

    /** @brief The ROM file checksum. */
    uint64_t checksum;

    /** @brief The ROM file unknown reserved region at 0x18. for 8 bytes */
    uint64_t unknown_reserved_1;

    /** @brief The ROM file title */
    char title[21]; // 20 chars + null char

    /** @brief The ROM file unknown reserved region at 0x34. for 7 bytes */
    char unknown_reserved_2[7];

    /** @brief The ROM file metadata @see rom_metadata_t. */
    rom_metadata_t metadata;
    /** @brief The ROM file release version. */

    char ipl_boot_code[0x0FC0];

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
