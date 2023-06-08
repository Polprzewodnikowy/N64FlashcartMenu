#include "rom_database.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint8_t extract_homebrew_setting(uint8_t setting, uint8_t bit_position) {
    return (setting & (1 << bit_position)) ? 1 : 0;
}

uint8_t extract_homebrew_save_type(uint8_t save_type) {
    switch (save_type) {
        case 0x00:
            return DB_SAVE_TYPE_NONE;
        case 0x01:
            return DB_SAVE_TYPE_EEPROM_4K;
        case 0x02:
            return DB_SAVE_TYPE_EEPROM_16K;
        case 0x03:
            return DB_SAVE_TYPE_SRAM;
        case 0x04:
            return DB_SAVE_TYPE_SRAM_BANKED;
        case 0x05:
            return DB_SAVE_TYPE_FLASHRAM;
        case 0x06:
            return DB_SAVE_TYPE_SRAM_128K;
        default:
            return DB_SAVE_TYPE_CART_SPECIFIED; // Invalid save type, handle accordingly
    }
}

rom_header_t file_read_rom_header(char *path) {
    FILE *fp = fopen(path, "rb");
    printf("loading path: %s\n", path);
	if (!fp) {
        printf("Error loading rom file header\n");
    }

    rom_header_t *rom_header = malloc(sizeof(rom_header_t));

    fseek(fp, 0x10, SEEK_SET);
    fread(&(rom_header->checksum), sizeof(uint64_t), 1, fp);
    fseek(fp, 0x20, SEEK_SET);
	fread(&(rom_header->title), sizeof(rom_header->title), 1, fp);
    rom_header->title[20] = '\0';
    fseek(fp, 0x3b, SEEK_SET);
    fread(&(rom_header->metadata.media_type), sizeof(rom_header->metadata.media_type), 1, fp);
    //fseek(fp, 0x3c, SEEK_SET);     // Consecutive read (no need to seek).
    fread(&(rom_header->metadata.unique_identifier), sizeof(rom_header->metadata.unique_identifier), 1, fp);
    //fseek(fp, 0x3e, SEEK_SET);     // Consecutive read (no need to seek).
    fread(&(rom_header->metadata.destination_market), sizeof(rom_header->metadata.destination_market), 1, fp);
    //fseek(fp, 0x3f, SEEK_SET);     // Consecutive read (no need to seek).
    fread(&(rom_header->version), sizeof(rom_header->version), 1, fp);

    fclose(fp);

    return *rom_header;
}

// TODO: make sure this can also handle an external input file.

uint8_t rom_db_match_save_type(rom_header_t rom_header) {

    // These are ordered to ensure they are handled correctly...

    // First: Match by the `ED` Developer ID
    // TODO: if appropriate this can be improved with other unused codes... e.g. | `AA` | `ZZ` 
    if (rom_header.metadata.unique_identifier == *(uint16_t *)"ED") {
        
        // uint8_t low_nibble = rom_header.version & 0x0F;
        // uint8_t rtc_enabled = extract_homebrew_setting(low_nibble, 0); // Bit 0
        // uint8_t region_free_enabled = extract_homebrew_setting(low_nibble, 1); // Bit 1

        uint8_t high_nibble = (rom_header.version >> 4) & 0x0F;

        return extract_homebrew_save_type(high_nibble);
    }

    // Second: Match the default entries for crc_high.
    // FIXME: use full check code, or pad.
    if (rom_header.checksum == 0xbcb1f89f)return DB_SAVE_TYPE_EEPROM_4K;    // kirby v1.3
    if (rom_header.checksum == 0x46039fb4)return DB_SAVE_TYPE_EEPROM_16K;   // kirby U
    if (rom_header.checksum == 0x0d93ba11)return DB_SAVE_TYPE_EEPROM_16K;   // kirby U
    if (rom_header.checksum == 0xce84793d)return DB_SAVE_TYPE_SRAM;         // donkey kong f2
    if (rom_header.checksum == 0x4cbc3b56)return DB_SAVE_TYPE_SRAM;         // DMTJ 64DD game
    if (rom_header.checksum == 0x0dd4abab)return DB_SAVE_TYPE_EEPROM_16K;   // DK Retail kiosk demo (shares ID with Dinosaur planet, but hacks are unlikely)!
    if (rom_header.checksum == 0xeb85ebc9)return DB_SAVE_TYPE_FLASHRAM;     // DOUBUTSU BANCHOU (ANIMAL LEADER, Cubivore) - Contains no game ID
    

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
        "VL", "VY", "WC", "WL", "WQ", "WR", "WU", "XO", "4W",        
        // EEP16K
        "B7", "CW", "CZ", "DO", "D2", "D6", "EP", "EV", "FU", "F2", "IM", "M8", "MV", "MX", "NB", "NX",
        "PD", "RZ", "UB", "X7", "YS", "3D",
        // SRAM
        "AL", "AY", "A2", "DA", "FZ", "GP", "G6", "K4", "KG", "MF", "OB", "RE", "RI", "TE", "VB", "VP",
        "WI", "W2", "WX", "WZ", "YW", "ZL",
        // SRAM_BANKED
        "DZ",
        // SRAM_128K

        // FLASHRAM
        "AF", "CC", "CK", "DL", "DP", "JD", "JF", "KJ", "M6", "MQ", "P2", "P3", "PF", "PH", "PN", "PO",
        "PS", "RH", "SI", "SQ", "T9", "W4", "ZS",
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
        0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
        // EEP16K
        0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
        0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
        // SRAM
        0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
        0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
        // SRAM_BANKED
        0x04,
        // SRAM_128K

        // FLASHRAM
        0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
        0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06,
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
