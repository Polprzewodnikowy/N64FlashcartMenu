#include <libdragon.h>
#include "rom_database.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t extract_homebrew_setting(uint8_t setting, uint8_t bit_position) {
    return (setting & (1 << bit_position)) ? 1 : 0;
}

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
    fread(&(rom_header->endian), sizeof(uint32_t), 1, fp);
    // FIXME: handle endian appropriately, perhaps: cart_card_byteswap
    fseek(fp, 0x10, SEEK_SET);
    fread(&(rom_header->checksum), sizeof(uint64_t), 1, fp);
    fseek(fp, 0x20, SEEK_SET);
	fgets(rom_header->title, sizeof(rom_header->title), fp);
    fseek(fp, 0x3b, SEEK_SET);
    fread(&(rom_header->metadata.media_type), sizeof(rom_header->metadata.media_type), 1, fp);
    //fseek(fp, 0x3c, SEEK_SET);     // Consecutive read (no need to seek).
    fread(&(rom_header->metadata.unique_identifier), sizeof(rom_header->metadata.unique_identifier), 1, fp);
    //fseek(fp, 0x3e, SEEK_SET);     // Consecutive read (no need to seek).
    fread(&(rom_header->metadata.destination_market), sizeof(rom_header->metadata.destination_market), 1, fp);
    //fseek(fp, 0x3f, SEEK_SET);     // Consecutive read (no need to seek).
    fread(&(rom_header->version), sizeof(rom_header->version), 1, fp);

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
    // donkey kong f2
    // if (rom_header.checksum == 0xce84793d) return DB_SAVE_TYPE_SRAM;
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

        // Controller Pak only
        "BX", "BQ", "NS",

        // Last entry
        "!!"

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

        // Controller Pak only
        0x10, 0x10, 0x10,

        // Last entry.
        0xff
    };

    for (int i = 0; save_types[i] != 0xff; i++) {

        if (rom_header.metadata.unique_identifier == *(uint16_t *) cart_ids[i]) {
            return save_types[i];
        }

    }

    return DB_SAVE_TYPE_NONE; //Nothing matched.
}

uint8_t rom_db_match_expansion_pak(rom_header_t rom_header) {

    static char *cart_ids[] = {

        // Expansion Pak Required
        "DO", "DP", "ZS", // Donkey Kong, Dino Planet, Majoras Mask

        // Expansion Pak Suggested
         "IJ", "PD", "SQ", // Indiana Jones, Perfect Dark, Starcraft

        // Expansion Pak Enhanced
        "32", "F2", "MX", "NA", "Q2", "RE", "SD", "P3", "Y2", "TQ", // F-1 World Grand Prix II, Shadow Man, Excitebike 64, Rogue Squadron, Battle for Naboo, Quake 2, Resident Evil 2, etc.

        // Expansion Pak Known Faulty ( NTSC only )
        "SV",

        // Last entry
        "!!"

    };
    static int exp_types[] = {

        // Expansion Pak Required
        0x01, 0x01, 0x01,

        // Expansion Pak Suggested
        0x02, 0x02, 0x02,

        // Expansion Pak Enhanced
        0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,

        // Expansion Pak Known Faulty ( NTSC only )
        0x04,

        // Last entry.
        0xff
    };

    for (int i = 0; exp_types[i] != 0xff; i++) {

        if (rom_header.metadata.unique_identifier == *(uint16_t *) cart_ids[i]) {
            return exp_types[i];
        }

    }

    return DB_MEMORY_EXPANSION_NONE;
}
