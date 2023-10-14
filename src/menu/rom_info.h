/**
 * @file rom_info.h
 * @brief N64 ROM Database.
 * @note Only works with N64 ROM's by checking the first 1024 bytes of the file.
 * @ingroup menu 
 */

#ifndef ROM_INFO_H__
#define ROM_INFO_H__


#include <stdbool.h>
#include <stdint.h>

#include "boot/cic.h"


typedef enum {
    ROM_OK,
    ROM_ERR_IO,
    ROM_ERR_NO_FILE,
} rom_err_t;

typedef enum {
    ENDIANNESS_BIG,
    ENDIANNESS_LITTLE,
    ENDIANNESS_BYTE_SWAP,
} endianness_t;

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
} category_type_t;

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
} destination_type_t;

typedef enum {
    SAVE_TYPE_NONE,
    SAVE_TYPE_EEPROM_4K,
    SAVE_TYPE_EEPROM_16K,
    SAVE_TYPE_SRAM,
    SAVE_TYPE_SRAM_BANKED,
    SAVE_TYPE_SRAM_128K,
    SAVE_TYPE_FLASHRAM,
    SAVE_TYPE_FLASHRAM_PKST2,
} save_type_t;

typedef enum {
    /** @brief The ROM is happy with 4MB of memory. */
    EXPANSION_PAK_NONE,

    /** @brief The ROM requires 8MB of memory. */
    EXPANSION_PAK_REQUIRED,

    /** @brief The ROM recommends 8MB of memory. */
    EXPANSION_PAK_RECOMMENDED,

    /** @brief The ROM suggests 8MB of memory. */
    EXPANSION_PAK_SUGGESTED,

    /** @brief The ROM is faulty when using 8MB of memory. */
    EXPANSION_PAK_FAULTY,
} expansion_pak_t;

typedef struct {
    endianness_t endianness;
    float clock_rate;
    uint32_t boot_address;
    struct {
        uint8_t version;
        char revision;
    } libultra;
    uint64_t check_code;
    char title[20];
    union {
        char game_code[4];
        struct {
            category_type_t category_code : 8;
            char unique_code[2];
            destination_type_t destination_code : 8;
        };
    };
    uint8_t version;

    cic_type_t cic_type;

    save_type_t save_type;

    struct {
        bool controller_pak;
        bool rumble_pak;
        bool transfer_pak;
        bool voice_recognition_unit;
        bool real_time_clock;
        bool disk_conversion;
        bool combo_rom_disk_game;
        expansion_pak_t expansion_pak;
    } features;
} rom_info_t;


rom_err_t rom_info_load (char *path, rom_info_t *rom_info);


#endif
