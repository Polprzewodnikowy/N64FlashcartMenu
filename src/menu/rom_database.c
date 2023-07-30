#include <libdragon.h>
#include "rom_database.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t extract_homebrew_setting(uint8_t setting, uint8_t bit_position) {
    return (setting & (1 << bit_position)) ? 1 : 0;
}

/**
 * @brief Normalize the Homebrew save type.
 */
uint8_t extract_homebrew_save_type(uint8_t save_type) {
    switch (save_type) {
        case HB_SAVE_TYPE_NONE:
            return DB_SAVE_TYPE_NONE;
        case HB_SAVE_TYPE_EEPROM_4K:
            return DB_SAVE_TYPE_EEPROM_4K;
        case HB_SAVE_TYPE_EEPROM_16K:
            return DB_SAVE_TYPE_EEPROM_16K;
        case HB_SAVE_TYPE_SRAM:
            return DB_SAVE_TYPE_SRAM;
        case HB_SAVE_TYPE_SRAM_BANKED:
            return DB_SAVE_TYPE_SRAM_BANKED;
        case HB_SAVE_TYPE_FLASHRAM:
            return DB_SAVE_TYPE_FLASHRAM;
        case HB_SAVE_TYPE_SRAM_128K:
            return DB_SAVE_TYPE_SRAM_128K;
        default:
            return DB_SAVE_TYPE_INVALID; // Invalid save type, handle accordingly
    }
}


/**
 * @brief Reads the N64 ROM header from a file. @see https://n64brew.dev/wiki/ROM_Header
 */
rom_header_t file_read_rom_header(char *path) {
    char *sd_path = calloc(4 + strlen(path) + 1, sizeof(char));
    sprintf(sd_path, "sd:/%s", path);

    FILE *fp = fopen(sd_path, "rb");

    debugf("loading path: %s\n", sd_path);
	if (!fp) {
        debugf("Error loading rom file header\n");
    }

    rom_header_t *rom_header = malloc(sizeof(rom_header_t));

    fseek(fp, 0x00, SEEK_SET);
    fread(&(rom_header->config_flags), sizeof(rom_header->config_flags), 1, fp);

    // FIXME: handle endian appropriately, perhaps: cart_card_byteswap

    fseek(fp, 0x04, SEEK_SET);
    fread(&(rom_header->clock_rate), sizeof(rom_header->clock_rate), 1, fp);
    fseek(fp, 0x08, SEEK_SET);
    fread(&(rom_header->boot_address), sizeof(rom_header->boot_address), 1, fp);
    fseek(fp, 0x0C, SEEK_SET);
    fread(&(rom_header->sdk_version), sizeof(rom_header->sdk_version), 1, fp);
    fseek(fp, 0x10, SEEK_SET);
    fread(&(rom_header->checksum), sizeof(rom_header->checksum), 1, fp);
    fseek(fp, 0x18, SEEK_SET);
    fread(&(rom_header->unknown_reserved_1), sizeof(rom_header->unknown_reserved_1), 1, fp);
    fseek(fp, 0x20, SEEK_SET);
    fgets(rom_header->title, sizeof(rom_header->title), fp);
    fseek(fp, 0x34, SEEK_SET);
    fread(&(rom_header->unknown_reserved_2), sizeof(rom_header->unknown_reserved_2), 1, fp);
    fseek(fp, 0x3B, SEEK_SET);
    fread(&(rom_header->metadata.media_type), sizeof(rom_header->metadata.media_type), 1, fp);
    fseek(fp, 0x3C, SEEK_SET);
    fread(&(rom_header->metadata.unique_identifier), sizeof(rom_header->metadata.unique_identifier), 1, fp);
    fseek(fp, 0x3E, SEEK_SET);
    fread(&(rom_header->metadata.destination_market), sizeof(rom_header->metadata.destination_market), 1, fp);
    fseek(fp, 0x3F, SEEK_SET);
    fread(&(rom_header->version), sizeof(rom_header->version), 1, fp);
    fseek(fp, 0x40, SEEK_SET);
    fread(&(rom_header->ipl3_boot_code), sizeof(rom_header->ipl3_boot_code), 1, fp);

    fclose(fp);

    free(sd_path);

    return *rom_header;
}

// TODO: make sure this can also handle an external input file.

uint8_t rom_db_match_save_type(rom_header_t rom_header) {

    // These are ordered to ensure they are handled correctly... (presumes big endian)

    // First: Match by the `ED` or `HB` Developer ID
    if (rom_header.metadata.unique_identifier == *(uint16_t *)"ED" || rom_header.metadata.unique_identifier == *(uint16_t *)"HB") {
// #ifdef ED64_COMPATIBLE        
//         uint8_t low_nibble = rom_header.version & 0x0F;
//         uint8_t rtc_enabled = extract_homebrew_setting(low_nibble, 0); // Bit 0
//         uint8_t region_free_enabled = extract_homebrew_setting(low_nibble, 1); // Bit 1
// #endif

        uint8_t high_nibble = (rom_header.version >> 4) & 0x0F;

        return extract_homebrew_save_type(high_nibble);
    }

    // Second: Match the default entries for crc.
    // DOUBUTSU BANCHOU (ANIMAL LEADER, Cubivore) - Contains no game ID
    if (rom_header.checksum == 0xEB85EBC9596682AF) return DB_SAVE_TYPE_FLASHRAM;
    // DK Retail kiosk demo (shares ID with Dinosaur planet, but hacks are unlikely)!
    if (rom_header.checksum == 0x0DD4ABABB5A2A91E) return DB_SAVE_TYPE_EEPROM_16K;
    // DMTJ 64DD game
    // if (rom_header.checksum == 0x4cbc3b56) return DB_SAVE_TYPE_SRAM;
    

    // FIXME: we need to take into account the Category (first char) and the Region code (last char) before a general match of the ID.

    // Finally: Match the default entries for "unique" ROM ID.
    // It is useful to use this rather than CRC's so that ROM hacks continue to work.
    static char *cart_ids[] = {
        // EEP4K
        "AB", "AD", "AG", "BC", "BD", "BH", "BK", "BM", "BN", "BV", "B6", "CG", "CH", "CR", "CT", "CU",
        "CX", "DR", "DQ", "DU", "DY", "D3", "D4", "EA", "ER", "FG", "FH", "FW", "FX", "FY", "GC", "GE",
        "GF", "GU", "GV", "HA", "HF", "HP", "IC", "IJ", "IR", "JM", "K2", "KA", "KI", "KT", "LB", "LR",
        "MG", "MI", "ML", "MO", "MR", "MS", "MU", "MW", "N6", "NA", "OH", "PG", "PW", "PY", "RC", "RS",
        "S6", "SA", "SC", "SM", "SU", "SV", "SW", "TB", "TC", "TJ", "TM", "TN", "TP", "TR", "TX", "T6",
        "VL", "VY", "WC", "WL", "WQ", "WR", "WU", "XO", "4W", "GL", "O2", "OS", "PM", "PT", "SN", "SB",
        "SS",  
        // EEP16K
        "B7", "CW", "CZ", "DO", "D2", "D6", "EP", "EV", "FU", "F2", "IM", "M8", "MV", "MX", "NB", "NX",
        "PD", "RZ", "UB", "X7", "YS", "3D", "R7",
        // SRAM
        "AL", "AY", "A2", "DA", "FZ", "GP", "G6", "K4", "KG", "MF", "OB", "RE", "RI", "TE", "VB", "VP",
        "WI", "W2", "WX", "WZ", "YW", "ZL", "B5", "IB", "JG", "UT", "UM", "T3",
        // SRAM_BANKED
        "DZ",
        // SRAM_128K

        // FLASHRAM
        "AF", "CC", "CK", "DL", "DP", "JD", "JF", "KJ", "M6", "MQ", "P2", "P3", "PF", "PH", "PN", "PO",
        "PS", "RH", "SI", "SQ", "T9", "W4", "ZS", "DA",
        // Controller Pak required
        "2V", "32", "3P", "3T", "8W", "AC", "AM", "AR", "B4", "BF", "BL", "BO", "BP", "BQ", "BR", "BX",
        "BY", "CD", "CE", "CL", "CO", "CS", "DE", "DH", "DM", "DN", "DT", "EG", "ET", "F9", "FD", "FF",
        "FO", "FQ", "G2", "G5", "GA", "GM", "GN", "GR", "GX", "HC", "HT", "HX", "HW", "HV", "IV", "KK",
        "L2", "LG", "M4", "MB", "MY", "NC", "NS", "O7", "OF", "2P", "PR", "PU", "PX", "R2", "R3", "R6",
        "RK", "RO", "RP", "RR", "RT", "RU", "RV", "RX", "SD", "SF", "SG", "SK", "SL", "SP", "SX", "SY",
        "T4", "TA", "TF", "TH", "TQ", "WB", "X2", "X3", "Y2", "V3", "V8", "VC", "VG", "VR", "WD", "WO",
        "WP", "WT", "XF", "YP", "ZO",

        // Definitely none
        // "JQ", // Batman Beyond: Return of the Joker 
        // "CB", // Charlie Blast's Territory (Password)
        // "CF", // Clayfighter 63 1/3 
        // "C2", // Clayfighter: Sculptor's Cut
        // "DK", // Dark Rift
        // "EL", // Elmo's Letter Adventure
        // Elmo's Number Journey
        // "JO", // Jeopardy!
        // "KE", // Knife Edge: Nosegunner
        // "ME", // Mace: The Dark Age
        // "MT", //Magical Tetris Challenge
        // "M3", //Monster Truck Madness (Password)
        // "MK", //Mortal Kombat Trilogy
        // Powerpuff Girls: Chemical X Traction (Password)
        // "RG", //Rugrats: Scavenger Hunt
        // "CY", //South Park: Chef's Luv Shack
        // "OH", //Transformers Beast Wars: Transmetals
        // "WA", //War Gods
        // "WF", //Wheel of Fortune

        // To work out
        // "AH", "BW", "GB", "LC", "2V", "DW", "M3", "MK", "NC", "WA",

        // Last entry
        "\0\0"

    };
    static int save_types[] = {
        // EEP4K
        0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
        0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
        0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
        0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
        0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
        0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
        0x01,
        // EEP16K
        0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
        0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
        // SRAM
        0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
        0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
        // SRAM_BANKED
        0x04,
        // SRAM_128K

        // FLASHRAM
        0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
        0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
        // Controller Pak required
        0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
        0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
        0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
        0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
        0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
        0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,
        0x10, 0x10, 0x10, 0x10, 0x10,

        // Last entry.
        0xFF
    };

    for (int i = 0; save_types[i] != 0xFF; i++) {

        if (rom_header.metadata.unique_identifier == *(uint16_t *) cart_ids[i]) {
            return save_types[i];
        }

    }

    return DB_SAVE_TYPE_NONE; //Nothing matched.
}

uint8_t rom_db_match_expansion_pak(rom_header_t rom_header) {

    // Space Station Silicon Valley has known issues on NTSC.
    // We only check for the known working market here.
    if ( (rom_header.metadata.unique_identifier == *(uint16_t *) "SV") && (rom_header.metadata.destination_market != MARKET_PAL_GENERIC)) {
        return DB_MEMORY_EXPANSION_FAULTY;
    }

    static char *cart_ids[] = {

        // See: https://nintendo.fandom.com/wiki/Nintendo_64_Expansion_Pak
        
        // Expansion Pak Required
        "DO", "DP", "ZS", // Donkey Kong, Dino Planet, Majoras Mask

        // Expansion Pak Recommended (games are pretty broken without it)
         "IJ", "PD", "SQ", // Indiana Jones, Perfect Dark, Starcraft

        // Expansion Pak Enhanced (box art suggest improvements in some form)
        "3T", "2M", "32", "4W", "9F", "AC", "AM", "AR", "AS", "AY", "BE", "CC", "CE", "CO", "D4", "DT",
        "DQ", "DZ", "F2", "FL", "GB", "GX", "HT", "HV", "IC", "IS", "JA", "L2", "MD", "MX", "NA", "O7",
        "Q2", "Q9", "QB", "RC", "RE", "RO", "RU", "RS", "RV", "RW", "SD", "SL", "P3", "T2", "T4", "TF",
        "TK", "Y2", "TQ", "V8", "VG", "XF", "ZO",

        // Last entry
        "\0\0"

    };
    static int exp_types[] = {

        // Expansion Pak Required
        0x01, 0x01, 0x01,

        // Expansion Pak Recommended
        0x02, 0x02, 0x02,

        // Expansion Pak Enhanced
        0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
        0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
        0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
        0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,

        // Last entry.
        0xFF
    };

    for (int i = 0; exp_types[i] != 0xff; i++) {

        if (rom_header.metadata.unique_identifier == *(uint16_t *) cart_ids[i]) {
            return exp_types[i];
        }

    }

    return DB_MEMORY_EXPANSION_NONE;
}
