#include "rom_database.h"
#include <stdint.h>

// TODO: make sure this can also handle an external input file.

uint8_t rom_db_match_save_type(rom_header_t rom_header) {

    // These are ordered to ensure they are handled correctly...

    // First: Match by the `ED` Developer ID
    // TODO: if appropriate this can be improved with other unused codes... e.g. | `AA` | `ZZ` 
    if (rom_header.metadata.unique_identifier == *(uint16_t *)"ED") {
        // FIXME: should probably just return the specified save type.
        return DB_SAVE_TYPE_CART_SPECIFIED;
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
            i = save_types[i];
            // i = i == 1 ? DB_SAVE_TYPE_EEPROM_4K : 
            //     i == 2 ? DB_SAVE_TYPE_EEPROM_16K :
            //     i == 3 ? DB_SAVE_TYPE_SRAM : 
            //     i == 4 ? DB_SAVE_TYPE_SRAM_BANKED :
            //     i == 5 ? DB_SAVE_TYPE_SRAM_128K : 
            //     i == 6 ? DB_SAVE_TYPE_FLASHRAM : 
            // //     i == 15 ? DB_SAVE_TYPE_CART_SPECIFIED : 
            //     DB_SAVE_TYPE_NONE;
            return i;
        }

    }

    return DB_SAVE_TYPE_NONE; //Nothing matched.
}
