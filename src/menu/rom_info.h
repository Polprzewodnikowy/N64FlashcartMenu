/**
 * @file rom_info.h
 * @brief N64 ROM Database.
 * @note Only works with N64 ROM's by checking the first 4096 bytes of the file.
 * @ingroup menu 
 */

#ifndef ROM_INFO_H__
#define ROM_INFO_H__


#include <stdbool.h>
#include <stdint.h>

#include "path.h"


/** @brief ROM error enumeration. */
typedef enum {
    ROM_OK,
    ROM_ERR_LOAD_IO,
    ROM_ERR_SAVE_IO,
    ROM_ERR_NO_FILE,
} rom_err_t;

/** @brief ROM endian enumeration. */
typedef enum {
    /** @brief Is Big Endian. */
    ENDIANNESS_BIG,
    /** @brief Is Little Endian. */
    ENDIANNESS_LITTLE,
    /** @brief Is Byte Swapped Endian. */
    ENDIANNESS_BYTE_SWAP,
} rom_endianness_t;

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
} rom_category_type_t;

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
} rom_destination_type_t;

/** @brief ROM CIC type enumeration. */
typedef enum {
    ROM_CIC_TYPE_UNKNOWN = 0,       // No known CIC type detected
    ROM_CIC_TYPE_5101 = 5101,       // Aleck64 CIC-5101
    ROM_CIC_TYPE_5167 = 5167,       // 64DD ROM conversion CIC-5167
    ROM_CIC_TYPE_6101 = 6101,       // NTSC CIC-6101
    ROM_CIC_TYPE_7102 = 7102,       // PAL CIC-7102
    ROM_CIC_TYPE_x102 = 6102,       // NTSC CIC-6102 / PAL CIC-7101
    ROM_CIC_TYPE_x103 = 6103,       // NTSC CIC-6103 / PAL CIC-7103
    ROM_CIC_TYPE_x105 = 6105,       // NTSC CIC-6105 / PAL CIC-7105
    ROM_CIC_TYPE_x106 = 6106,       // NTSC CIC-6106 / PAL CIC-7106
    ROM_CIC_TYPE_8301 = 8301,       // NDDJ0 64DD IPL
    ROM_CIC_TYPE_8302 = 8302,       // NDDJ1 64DD IPL
    ROM_CIC_TYPE_8303 = 8303,       // NDDJ2 64DD IPL
    ROM_CIC_TYPE_8401 = 8401,       // NDXJ0 64DD IPL
    ROM_CIC_TYPE_8501 = 8501,       // NDDE0 64DD IPL
    ROM_CIC_TYPE_AUTOMATIC = -1,    // Guess CIC from IPL3
} rom_cic_type_t;

/** @brief ROM save type enumeration. */
typedef enum {
    /** @brief There is no expected save type. */
    SAVE_TYPE_NONE = 0,
    SAVE_TYPE_EEPROM_4KBIT = 1,
    SAVE_TYPE_EEPROM_16KBIT = 2,
    SAVE_TYPE_SRAM_256KBIT = 3,
    SAVE_TYPE_SRAM_BANKED = 4,
    SAVE_TYPE_SRAM_1MBIT = 5,
    SAVE_TYPE_FLASHRAM_1MBIT = 6,
    SAVE_TYPE_FLASHRAM_PKST2 = 7,
    SAVE_TYPE_AUTOMATIC = -1,
} rom_save_type_t;

typedef enum {
    ROM_TV_TYPE_PAL = 0,
    ROM_TV_TYPE_NTSC = 1,
    ROM_TV_TYPE_MPAL = 2,
    ROM_TV_TYPE_UNKNOWN = 3,
    ROM_TV_TYPE_AUTOMATIC = -1,
} rom_tv_type_t;

/** @brief ROM memory requirements enumeration. */
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
} rom_expansion_pak_t;

/** @brief ROM Information Structure. */
typedef struct {
    /** @brief The file endian. */
    rom_endianness_t endianness;

    /** @brief The clock rate defined in the ROM's header. */
    float clock_rate;

    /** @brief The boot address defined in the ROM's header. */
    uint32_t boot_address;

    struct {
        /** @brief The SDK version defined in the ROM's header. */
        uint8_t version;
        /** @brief The SDK revision defined in the ROM's header. */
        char revision;
    } libultra;

    /** @brief The check code defined in the ROM's header. */
    uint64_t check_code;

    /** @brief The title defined in the ROM's header. */
    char title[20];

    union {
        /** @brief The game code defined in the ROM's header. */
        char game_code[4];
        struct {
            /** @brief The game media type. */
            rom_category_type_t category_code : 8;
            /** @brief The game unique identifier. */
            char unique_code[2];
            /** @brief The game region and or market. */
            rom_destination_type_t destination_code : 8;
        };
    };

    /** @brief The ROM version defined in the ROM's header. */
    uint8_t version;

    /** @brief The CIC type required by the ROM. */
    rom_cic_type_t cic_type;

    /** @brief The save type required by the ROM. */
    rom_save_type_t save_type;

    /** @brief The TV type required by the ROM. */
    rom_tv_type_t tv_type;

    /** @brief Overrides of auto-detected CIC/save/TV types. */
    struct {
        bool cic;
        rom_cic_type_t cic_type;

        bool save;
        rom_save_type_t save_type;

        bool tv;
        rom_tv_type_t tv_type;
    } override;

    /** @brief The supported ROM accessories. */
    struct {
        bool controller_pak;
        bool rumble_pak;
        bool transfer_pak;
        bool voice_recognition_unit;
        bool real_time_clock;
        bool disk_conversion;
        bool combo_rom_disk_game;
        rom_expansion_pak_t expansion_pak;
    } features;
} rom_info_t;


rom_cic_type_t rom_info_get_cic_type (rom_info_t *rom_info);
bool rom_info_get_cic_seed (rom_info_t *rom_info, uint8_t *seed);
rom_err_t rom_info_override_cic_type (path_t *path, rom_info_t *rom_info, rom_cic_type_t cic_type);

rom_save_type_t rom_info_get_save_type (rom_info_t *rom_info);
rom_err_t rom_info_override_save_type (path_t *path, rom_info_t *rom_info, rom_save_type_t save_type);

rom_tv_type_t rom_info_get_tv_type (rom_info_t *rom_info);
rom_err_t rom_info_override_tv_type (path_t *path, rom_info_t *rom_info, rom_tv_type_t tv_type);

rom_err_t rom_info_load (path_t *path, rom_info_t *rom_info);

void rom_info_use_config_folder(bool state);


#endif
