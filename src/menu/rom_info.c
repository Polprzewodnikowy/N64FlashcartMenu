#include <string.h>

#include <fatfs/ff.h>

#include "rom_info.h"
#include "utils/fs.h"


#define SWAP_VARS(x0, x1)       { typeof(x0) tmp = (x0); (x0) = (x1); (x1) = (tmp); }

#define PI_CONFIG_BIG_ENDIAN    (0x80371240)
#define PI_CONFIG_LITTLE_ENDIAN (0x40123780)
#define PI_CONFIG_BYTE_SWAPPED  (0x37804012)
#define PI_CONFIG_64DD_IPL      (0x80270740)

#define CLOCK_RATE_DEFAULT      (0x0000000F)

/** @brief ROM File Information Structure. */
typedef struct  __attribute__((packed)) {
    uint32_t pi_dom1_config;
    uint32_t clock_rate;
    uint32_t boot_address;
    struct {
        uint8_t __unused_1[2];
        uint8_t version;
        char revision;
    } libultra;
    uint64_t check_code;
    uint8_t __unused_1[8];
    char title[20];
    uint8_t __unused_2[7];
    union {
        char game_code[4];
        struct {
            char category_code;
            char unique_code[2];
            char destination_code;
        };
    };
    uint8_t version;
    uint8_t ipl3[IPL3_LENGTH];
} rom_header_t;

/** @brief ROM Information Match Type Enumeration. */
typedef enum {
    // Check only game code
    MATCH_TYPE_ID,

    // Check game code and region
    MATCH_TYPE_ID_REGION,

    // Check game code, region and version
    MATCH_TYPE_ID_REGION_VERSION,

    // Check game check code
    MATCH_TYPE_CHECK_CODE,

    // Check for homebrew header ID
    MATCH_TYPE_HOMEBREW_HEADER,

    // List end marker
    MATCH_TYPE_END
} match_type_t;

typedef enum {
    // No features supported
    FEAT_NONE = 0,

    // Controller Pak
    FEAT_CPAK = (1 << 0),

    // Rumble Pak
    FEAT_RPAK = (1 << 1),

    // Transfer Pak
    FEAT_TPAK = (1 << 2),

    // Voice Recognition Unit
    FEAT_VRU = (1 << 3),

    // Real Time Clock
    FEAT_RTC = (1 << 4),

    // Expansion Pak (for games that will not work without it inserted into the console)
    FEAT_EXP_PAK_REQUIRED = (1 << 5),

    // Expansion Pak (for games with game play enhancements)
    FEAT_EXP_PAK_RECOMMENDED = (1 << 6),

    // Expansion Pak (for games with visual (or other) enhancements)
    FEAT_EXP_PAK_ENHANCED = (1 << 7),

    // No Expansion Pak (for games "broken" with it inserted into the console)
    FEAT_EXP_PAK_BROKEN = (1 << 8),

    // 64DD disk to ROM conversion
    FEAT_64DD_CONVERSION = (1 << 9),

    // Combo ROM + Disk games
    FEAT_64DD_ENHANCED = (1 << 10),
} feat_t;

typedef struct {
    // Which fields to check
    match_type_t type;

    // Fields to check for matching
    union {
        struct {
            // Game code (with media type and optional region) or unique ID
            const char *id;

            // Game version
            uint8_t version;
        };

        // Game check code
        uint64_t check_code;
    } fields;

    // Matched game metadata
    struct {
        // Save type (only cartridge save types)
        save_type_t save;

        // Supported features
        feat_t feat;
    } data;
} match_t;


#define MATCH_ID(i, s, f)                       { .type = MATCH_TYPE_ID, .fields = { .id = i }, .data = { .save = s, .feat = f } }
#define MATCH_ID_REGION(i, s, f)                { .type = MATCH_TYPE_ID_REGION, .fields = { .id = i }, .data = { .save = s, .feat = f } }
#define MATCH_ID_REGION_VERSION(i, v, s, f)     { .type = MATCH_TYPE_ID_REGION_VERSION, .fields = { .id = i, .version = v }, .data = { .save = s, .feat = f } }
#define MATCH_CHECK_CODE(c, s, f)               { .type = MATCH_TYPE_CHECK_CODE, .fields = { .check_code = c }, .data = { .save = s, .feat = f } }
#define MATCH_HOMEBREW_HEADER(i)                { .type = MATCH_TYPE_HOMEBREW_HEADER, .fields = { .id = i }, .data = { .feat = FEAT_NONE } }
#define MATCH_END                               { .type = MATCH_TYPE_END, .data = { .save = SAVE_TYPE_NONE, .feat = FEAT_NONE } }


// List shamelessly stolen from https://github.com/ares-emulator/ares/blob/master/mia/medium/nintendo-64.cpp
static const match_t database[] = {
    MATCH_HOMEBREW_HEADER("ED"),                                                                            // Homebrew header (ED)

    MATCH_CHECK_CODE(0x000000004CBC3B56, SAVE_TYPE_SRAM, FEAT_EXP_PAK_REQUIRED | FEAT_64DD_CONVERSION),     // DMTJ 64DD cartridge conversion
    MATCH_CHECK_CODE(0x0DD4ABABB5A2A91E, SAVE_TYPE_EEPROM_16K, FEAT_EXP_PAK_REQUIRED),                      // DK Retail kiosk demo
    MATCH_CHECK_CODE(0xEB85EBC9596682AF, SAVE_TYPE_FLASHRAM, FEAT_NONE),                                    // Doubutsu Banchou

    MATCH_ID_REGION_VERSION("NK4J", 0, SAVE_TYPE_SRAM, FEAT_RPAK),                                          // Kirby 64: The Crystal Shards [Hoshi no Kirby 64 (J)]
    MATCH_ID_REGION_VERSION("NK4J", 1, SAVE_TYPE_SRAM, FEAT_RPAK),                                          // Kirby 64: The Crystal Shards [Hoshi no Kirby 64 (J)]
    MATCH_ID("NK4", SAVE_TYPE_EEPROM_16K, FEAT_RPAK),                                                       // Kirby 64: The Crystal Shards [Hoshi no Kirby 64 (J)]

    MATCH_ID_REGION_VERSION("NSMJ", 3, SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                     // Super Mario 64 Shindou Edition
    MATCH_ID("NSM", SAVE_TYPE_EEPROM_4K, FEAT_NONE),                                                        // Super Mario 64

    MATCH_ID_REGION_VERSION("NWRJ", 2, SAVE_TYPE_EEPROM_4K, FEAT_CPAK | FEAT_RPAK),                         // Wave Race 64 Shindou Edition
    MATCH_ID("NWR", SAVE_TYPE_EEPROM_4K, FEAT_CPAK),                                                        // Wave Race 64

    MATCH_ID_REGION("N3HJ", SAVE_TYPE_SRAM, FEAT_NONE),                                                     // Ganbare! Nippon! Olympics 2000
    MATCH_ID("N3H", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // International Track & Field 2000

    MATCH_ID_REGION("ND3J", SAVE_TYPE_EEPROM_16K, FEAT_RPAK),                                               // Akumajou Dracula Mokushiroku (J)
    MATCH_ID("ND3", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Castlevania

    MATCH_ID_REGION("ND4J", SAVE_TYPE_EEPROM_16K, FEAT_RPAK),                                               // Akumajou Dracula Mokushiroku Gaiden: Legend of Cornell (J)
    MATCH_ID("ND4", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Castlevania - Legacy of Darkness

    MATCH_ID_REGION("NDKJ", SAVE_TYPE_EEPROM_4K, FEAT_NONE),                                                // Dark Rift [Space Dynamites (J)]

    MATCH_ID_REGION("NSVE", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                // Space Station Silicon Valley
    MATCH_ID("NSV", SAVE_TYPE_EEPROM_4K, FEAT_RPAK | FEAT_EXP_PAK_BROKEN),                                  // Space Station Silicon Valley

    MATCH_ID_REGION("NWTJ", SAVE_TYPE_EEPROM_4K, FEAT_NONE),                                                // Wetrix
    MATCH_ID("NWT", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Wetrix

    MATCH_ID("CLB", SAVE_TYPE_EEPROM_4K, FEAT_RPAK | FEAT_64DD_ENHANCED),                                   // Mario Party (NTSC)
    MATCH_ID("NAB", SAVE_TYPE_EEPROM_4K, FEAT_CPAK | FEAT_RPAK),                                            // Air Boarder 64
    MATCH_ID("NAD", SAVE_TYPE_EEPROM_4K, FEAT_NONE),                                                        // Worms Armageddon (U)
    MATCH_ID("NAG", SAVE_TYPE_EEPROM_4K, FEAT_CPAK),                                                        // AeroGauge
    MATCH_ID("NB6", SAVE_TYPE_EEPROM_4K, FEAT_CPAK | FEAT_TPAK),                                            // Super B-Daman: Battle Phoenix 64
    MATCH_ID("NBC", SAVE_TYPE_EEPROM_4K, FEAT_CPAK),                                                        // Blast Corps
    MATCH_ID("NBD", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Bomberman Hero [Mirian Ojo o Sukue! (J)]
    MATCH_ID("NBH", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Body Harvest
    MATCH_ID("NBK", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Banjo-Kazooie [Banjo to Kazooie no Daiboken (J)]
    MATCH_ID("NBM", SAVE_TYPE_EEPROM_4K, FEAT_CPAK),                                                        // Bomberman 64 [Baku Bomberman (J)]
    MATCH_ID("NBN", SAVE_TYPE_EEPROM_4K, FEAT_CPAK),                                                        // Bakuretsu Muteki Bangaioh
    MATCH_ID("NBV", SAVE_TYPE_EEPROM_4K, FEAT_CPAK | FEAT_RPAK),                                            // Bomberman 64: The Second Attack! [Baku Bomberman 2 (J)]
    MATCH_ID("NCG", SAVE_TYPE_EEPROM_4K, FEAT_CPAK | FEAT_RPAK | FEAT_TPAK),                                // Choro Q 64 II - Hacha Mecha Grand Prix Race (J)
    MATCH_ID("NCH", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Chopper Attack
    MATCH_ID("NCR", SAVE_TYPE_EEPROM_4K, FEAT_CPAK),                                                        // Penny Racers [Choro Q 64 (J)]
    MATCH_ID("NCT", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Chameleon Twist
    MATCH_ID("NCU", SAVE_TYPE_EEPROM_4K, FEAT_CPAK),                                                        // Cruis'n USA
    MATCH_ID("NCX", SAVE_TYPE_EEPROM_4K, FEAT_NONE),                                                        // Custom Robo
    MATCH_ID("NDQ", SAVE_TYPE_EEPROM_4K, FEAT_CPAK),                                                        // Disney's Donald Duck - Goin' Quackers [Quack Attack (E)]
    MATCH_ID("NDR", SAVE_TYPE_EEPROM_4K, FEAT_NONE),                                                        // Doraemon: Nobita to 3tsu no Seireiseki
    MATCH_ID("NDU", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Duck Dodgers starring Daffy Duck
    MATCH_ID("NDY", SAVE_TYPE_EEPROM_4K, FEAT_CPAK | FEAT_RPAK),                                            // Diddy Kong Racing
    MATCH_ID("NEA", SAVE_TYPE_EEPROM_4K, FEAT_NONE),                                                        // PGA European Tour
    MATCH_ID("NER", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Aero Fighters Assault [Sonic Wings Assault (J)]
    MATCH_ID("NF2", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // F-1 World Grand Prix II
    MATCH_ID("NFG", SAVE_TYPE_EEPROM_4K, FEAT_CPAK | FEAT_RPAK),                                            // Fighter Destiny 2
    MATCH_ID("NFH", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // In-Fisherman Bass Hunter 64 
    MATCH_ID("NFW", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // F-1 World Grand Prix
    MATCH_ID("NFX", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Star Fox 64 [Lylat Wars (E)]
    MATCH_ID("NFY", SAVE_TYPE_EEPROM_4K, FEAT_CPAK | FEAT_RPAK),                                            // Kakutou Denshou: F-Cup Maniax
    MATCH_ID("NGE", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // GoldenEye 007
    MATCH_ID("NGL", SAVE_TYPE_EEPROM_4K, FEAT_CPAK | FEAT_RPAK),                                            // Getter Love!!
    MATCH_ID("NGU", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Tsumi to Batsu: Hoshi no Keishousha (Sin and Punishment)
    MATCH_ID("NGV", SAVE_TYPE_EEPROM_4K, FEAT_NONE),                                                        // Glover
    MATCH_ID("NHA", SAVE_TYPE_EEPROM_4K, FEAT_CPAK),                                                        // Bomberman 64: Arcade Edition (J)
    MATCH_ID("NHF", SAVE_TYPE_EEPROM_4K, FEAT_NONE),                                                        // 64 Hanafuda: Tenshi no Yakusoku
    MATCH_ID("NHP", SAVE_TYPE_EEPROM_4K, FEAT_NONE),                                                        // Heiwa Pachinko World 64
    MATCH_ID("NIC", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Indy Racing 2000
    MATCH_ID("NIJ", SAVE_TYPE_EEPROM_4K, FEAT_RPAK | FEAT_EXP_PAK_RECOMMENDED),                             // Indiana Jones and the Infernal Machine
    MATCH_ID("NIR", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Utchan Nanchan no Hono no Challenger: Denryuu Ira Ira Bou
    MATCH_ID("NJM", SAVE_TYPE_EEPROM_4K, FEAT_NONE),                                                        // Earthworm Jim 3D
    MATCH_ID("NK2", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Snowboard Kids 2 [Chou Snobow Kids (J)]
    MATCH_ID("NKA", SAVE_TYPE_EEPROM_4K, FEAT_CPAK | FEAT_RPAK),                                            // Fighters Destiny [Fighting Cup (J)]
    MATCH_ID("NKI", SAVE_TYPE_EEPROM_4K, FEAT_CPAK),                                                        // Killer Instinct Gold
    MATCH_ID("NKT", SAVE_TYPE_EEPROM_4K, FEAT_CPAK),                                                        // Mario Kart 64
    MATCH_ID("NLB", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Mario Party (PAL)
    MATCH_ID("NLL", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Last Legion UX
    MATCH_ID("NLR", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Lode Runner 3-D
    MATCH_ID("NMG", SAVE_TYPE_EEPROM_4K, FEAT_CPAK | FEAT_RPAK),                                            // Monaco Grand Prix [Racing Simulation 2 (G)]
    MATCH_ID("NMI", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Mission: Impossible
    MATCH_ID("NML", SAVE_TYPE_EEPROM_4K, FEAT_RPAK | FEAT_TPAK),                                            // Mickey's Speedway USA [Mickey no Racing Challenge USA (J)]
    MATCH_ID("NMO", SAVE_TYPE_EEPROM_4K, FEAT_NONE),                                                        // Monopoly
    MATCH_ID("NMR", SAVE_TYPE_EEPROM_4K, FEAT_CPAK | FEAT_RPAK),                                            // Multi-Racing Championship
    MATCH_ID("NMS", SAVE_TYPE_EEPROM_4K, FEAT_CPAK),                                                        // Morita Shougi 64
    MATCH_ID("NMU", SAVE_TYPE_EEPROM_4K, FEAT_CPAK | FEAT_RPAK),                                            // Big Mountain 2000
    MATCH_ID("NMW", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Mario Party 2
    MATCH_ID("NMZ", SAVE_TYPE_EEPROM_4K, FEAT_NONE),                                                        // Zool - Majou Tsukai Densetsu (J)
    MATCH_ID("NN6", SAVE_TYPE_EEPROM_4K, FEAT_NONE),                                                        // Dr. Mario 64
    MATCH_ID("NNA", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Star Wars Episode I: Battle for Naboo
    MATCH_ID("NOS", SAVE_TYPE_EEPROM_4K, FEAT_CPAK | FEAT_RPAK),                                            // 64 Oozumou
    MATCH_ID("NP2", SAVE_TYPE_EEPROM_4K, FEAT_CPAK | FEAT_RPAK),                                            // Chou Kuukan Night Pro Yakyuu King 2 (J)
    MATCH_ID("NPG", SAVE_TYPE_EEPROM_4K, FEAT_RPAK | FEAT_VRU),                                             // Hey You, Pikachu! [Pikachu Genki Dechu (J)]
    MATCH_ID("NPT", SAVE_TYPE_EEPROM_4K, FEAT_RPAK | FEAT_TPAK),                                            // Puyo Puyon Party
    MATCH_ID("NPW", SAVE_TYPE_EEPROM_4K, FEAT_NONE),                                                        // Pilotwings 64
    MATCH_ID("NPY", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Puyo Puyo Sun 64
    MATCH_ID("NRA", SAVE_TYPE_EEPROM_4K, FEAT_CPAK | FEAT_RPAK),                                            // Rally '99 (J)
    MATCH_ID("NRC", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Top Gear Overdrive
    MATCH_ID("NRS", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Star Wars: Rogue Squadron [Shutsugeki! Rogue Chuutai (J)]
    MATCH_ID("NS3", SAVE_TYPE_EEPROM_4K, FEAT_CPAK),                                                        // AI Shougi 3
    MATCH_ID("NS6", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Star Soldier: Vanishing Earth
    MATCH_ID("NSA", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Sonic Wings Assault (J)
    MATCH_ID("NSC", SAVE_TYPE_EEPROM_4K, FEAT_NONE),                                                        // Starshot: Space Circus Fever
    MATCH_ID("NSN", SAVE_TYPE_EEPROM_4K, FEAT_CPAK | FEAT_RPAK),                                            // Snow Speeder (J)
    MATCH_ID("NSS", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Super Robot Spirits
    MATCH_ID("NSU", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Rocket: Robot on Wheels
    MATCH_ID("NSW", SAVE_TYPE_EEPROM_4K, FEAT_NONE),                                                        // Star Wars: Shadows of the Empire [Teikoku no Kage (J)]
    MATCH_ID("NT6", SAVE_TYPE_EEPROM_4K, FEAT_NONE),                                                        // Tetris 64
    MATCH_ID("NTB", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Transformers: Beast Wars Metals 64 (J)
    MATCH_ID("NTC", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // 64 Trump Collection
    MATCH_ID("NTJ", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Tom & Jerry in Fists of Fury
    MATCH_ID("NTM", SAVE_TYPE_EEPROM_4K, FEAT_NONE),                                                        // Mischief Makers [Yuke Yuke!! Trouble Makers (J)]
    MATCH_ID("NTN", SAVE_TYPE_EEPROM_4K, FEAT_NONE),                                                        // All Star Tennis '99
    MATCH_ID("NTP", SAVE_TYPE_EEPROM_4K, FEAT_NONE),                                                        // Tetrisphere
    MATCH_ID("NTR", SAVE_TYPE_EEPROM_4K, FEAT_CPAK | FEAT_RPAK),                                            // Top Gear Rally (J + E)
    MATCH_ID("NTW", SAVE_TYPE_EEPROM_4K, FEAT_CPAK),                                                        // 64 de Hakken!! Tamagotchi
    MATCH_ID("NTX", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Taz Express
    MATCH_ID("NVL", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // V-Rally Edition '99
    MATCH_ID("NVY", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // V-Rally Edition '99 (J)
    MATCH_ID("NWC", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Wild Choppers
    MATCH_ID("NWQ", SAVE_TYPE_EEPROM_4K, FEAT_CPAK | FEAT_RPAK),                                            // Rally Challenge 2000
    MATCH_ID("NWU", SAVE_TYPE_EEPROM_4K, FEAT_NONE),                                                        // Worms Armageddon (E)
    MATCH_ID("NXO", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Cruis'n Exotica
    MATCH_ID("NYK", SAVE_TYPE_EEPROM_4K, FEAT_RPAK),                                                        // Yakouchuu II: Satsujin Kouro

    MATCH_ID("N3D", SAVE_TYPE_EEPROM_16K, FEAT_RPAK),                                                       // Doraemon 3: Nobita no Machi SOS!
    MATCH_ID("NB7", SAVE_TYPE_EEPROM_16K, FEAT_RPAK),                                                       // Banjo-Tooie [Banjo to Kazooie no Daiboken 2 (J)]
    MATCH_ID("NCW", SAVE_TYPE_EEPROM_16K, FEAT_RPAK),                                                       // Cruis'n World
    MATCH_ID("NCZ", SAVE_TYPE_EEPROM_16K, FEAT_RPAK),                                                       // Custom Robo V2
    MATCH_ID("ND2", SAVE_TYPE_EEPROM_16K, FEAT_RPAK),                                                       // Doraemon 2: Nobita to Hikari no Shinden
    MATCH_ID("ND6", SAVE_TYPE_EEPROM_16K, FEAT_RPAK | FEAT_VRU),                                            // Densha de Go! 64
    MATCH_ID("NDO", SAVE_TYPE_EEPROM_16K, FEAT_RPAK | FEAT_EXP_PAK_REQUIRED),                               // Donkey Kong 64
    MATCH_ID("NEP", SAVE_TYPE_EEPROM_16K, FEAT_RPAK),                                                       // Star Wars Episode I: Racer
    MATCH_ID("NEV", SAVE_TYPE_EEPROM_16K, FEAT_RPAK),                                                       // Neon Genesis Evangelion
    MATCH_ID("NFU", SAVE_TYPE_EEPROM_16K, FEAT_RPAK),                                                       // Conker's Bad Fur Day
    MATCH_ID("NGC", SAVE_TYPE_EEPROM_16K, FEAT_CPAK | FEAT_RPAK),                                           // GT 64: Championship Edition
    MATCH_ID("NGT", SAVE_TYPE_EEPROM_16K, FEAT_CPAK | FEAT_RPAK),                                           // City Tour GrandPrix - Zen Nihon GT Senshuken
    MATCH_ID("NIM", SAVE_TYPE_EEPROM_16K, FEAT_NONE),                                                       // Ide Yosuke no Mahjong Juku
    MATCH_ID("NM8", SAVE_TYPE_EEPROM_16K, FEAT_RPAK | FEAT_TPAK),                                           // Mario Tennis
    MATCH_ID("NMV", SAVE_TYPE_EEPROM_16K, FEAT_RPAK),                                                       // Mario Party 3
    MATCH_ID("NMX", SAVE_TYPE_EEPROM_16K, FEAT_CPAK | FEAT_RPAK),                                           // Excitebike 64
    MATCH_ID("NNB", SAVE_TYPE_EEPROM_16K, FEAT_CPAK | FEAT_RPAK),                                           // Kobe Bryant in NBA Courtside
    MATCH_ID("NPD", SAVE_TYPE_EEPROM_16K, FEAT_CPAK | FEAT_RPAK | FEAT_TPAK | FEAT_EXP_PAK_RECOMMENDED),    // Perfect Dark
    MATCH_ID("NPP", SAVE_TYPE_EEPROM_16K, FEAT_CPAK),                                                       // Parlor! Pro 64: Pachinko Jikki Simulation Game
    MATCH_ID("NR7", SAVE_TYPE_EEPROM_16K, FEAT_TPAK),                                                       // Robot Poncots 64: 7tsu no Umi no Caramel
    MATCH_ID("NRZ", SAVE_TYPE_EEPROM_16K, FEAT_RPAK),                                                       // Ridge Racer 64
    MATCH_ID("NUB", SAVE_TYPE_EEPROM_16K, FEAT_CPAK | FEAT_TPAK),                                           // PD Ultraman Battle Collection 64
    MATCH_ID("NYS", SAVE_TYPE_EEPROM_16K, FEAT_RPAK),                                                       // Yoshi's Story

    MATCH_ID("CFZ", SAVE_TYPE_SRAM, FEAT_RPAK | FEAT_64DD_ENHANCED),                                        // F-Zero X (J)
    MATCH_ID("CPS", SAVE_TYPE_SRAM, FEAT_TPAK | FEAT_64DD_ENHANCED),                                        // Pocket Monsters Stadium (J)
    MATCH_ID("CZL", SAVE_TYPE_SRAM, FEAT_RPAK | FEAT_64DD_ENHANCED),                                        // Legend of Zelda: Ocarina of Time [Zelda no Densetsu - Toki no Ocarina (J)]
    MATCH_ID("NA2", SAVE_TYPE_SRAM, FEAT_CPAK | FEAT_RPAK),                                                 // Virtual Pro Wrestling 2
    MATCH_ID("NAL", SAVE_TYPE_SRAM, FEAT_RPAK),                                                             // Super Smash Bros. [Nintendo All-Star! Dairantou Smash Brothers (J)]
    MATCH_ID("NB5", SAVE_TYPE_SRAM, FEAT_RPAK),                                                             // Biohazard 2 (J)
    MATCH_ID("NFZ", SAVE_TYPE_SRAM, FEAT_RPAK),                                                             // F-Zero X (U + E)
    MATCH_ID("NG6", SAVE_TYPE_SRAM, FEAT_RPAK),                                                             // Ganmare Goemon: Dero Dero Douchuu Obake Tenkomori
    MATCH_ID("NGP", SAVE_TYPE_SRAM, FEAT_CPAK),                                                             // Goemon: Mononoke Sugoroku
    MATCH_ID("NHY", SAVE_TYPE_SRAM, FEAT_CPAK | FEAT_RPAK),                                                 // Hybrid Heaven (J)
    MATCH_ID("NIB", SAVE_TYPE_SRAM, FEAT_RPAK),                                                             // Itoi Shigesato no Bass Tsuri No. 1 Kettei Ban!
    MATCH_ID("NJ5", SAVE_TYPE_SRAM, FEAT_CPAK),                                                             // Jikkyou Powerful Pro Yakyuu 5
    MATCH_ID("NJG", SAVE_TYPE_SRAM, FEAT_RPAK),                                                             // Jinsei Game 64
    MATCH_ID("NKG", SAVE_TYPE_SRAM, FEAT_CPAK | FEAT_RPAK),                                                 // Major League Baseball featuring Ken Griffey Jr.
    MATCH_ID("NMF", SAVE_TYPE_SRAM, FEAT_RPAK | FEAT_TPAK),                                                 // Mario Golf 64
    MATCH_ID("NOB", SAVE_TYPE_SRAM, FEAT_NONE),                                                             // Ogre Battle 64: Person of Lordly Caliber
    MATCH_ID("NP4", SAVE_TYPE_SRAM, FEAT_CPAK),                                                             // Jikkyou Powerful Pro Yakyuu 4
    MATCH_ID("NP6", SAVE_TYPE_SRAM, FEAT_CPAK | FEAT_TPAK),                                                 // Jikkyou Powerful Pro Yakyuu 6
    MATCH_ID("NPA", SAVE_TYPE_SRAM, FEAT_CPAK | FEAT_TPAK),                                                 // Jikkyou Powerful Pro Yakyuu 2000
    MATCH_ID("NPE", SAVE_TYPE_SRAM, FEAT_CPAK),                                                             // Jikkyou Powerful Pro Yakyuu Basic Ban 2001
    MATCH_ID("NPM", SAVE_TYPE_SRAM, FEAT_CPAK),                                                             // Premier Manager 64
    MATCH_ID("NPS", SAVE_TYPE_SRAM, FEAT_CPAK | FEAT_RPAK),                                                 // Jikkyou J.League 1999: Perfect Striker 2
    MATCH_ID("NRE", SAVE_TYPE_SRAM, FEAT_RPAK),                                                             // Resident Evil 2
    MATCH_ID("NRI", SAVE_TYPE_SRAM, FEAT_CPAK),                                                             // New Tetris, The
    MATCH_ID("NS4", SAVE_TYPE_SRAM, FEAT_CPAK | FEAT_TPAK),                                                 // Super Robot Taisen 64
    MATCH_ID("NSI", SAVE_TYPE_SRAM, FEAT_CPAK),                                                             // Fushigi no Dungeon: Fuurai no Shiren 2
    MATCH_ID("NT3", SAVE_TYPE_SRAM, FEAT_CPAK),                                                             // Shin Nihon Pro Wrestling - Toukon Road 2 - The Next Generation (J)
    MATCH_ID("NTE", SAVE_TYPE_SRAM, FEAT_RPAK),                                                             // 1080 Snowboarding
    MATCH_ID("NUM", SAVE_TYPE_SRAM, FEAT_RPAK | FEAT_TPAK),                                                 // Nushi Zuri 64: Shiokaze ni Notte
    MATCH_ID("NUT", SAVE_TYPE_SRAM, FEAT_CPAK | FEAT_RPAK | FEAT_TPAK),                                     // Nushi Zuri 64
    MATCH_ID("NVB", SAVE_TYPE_SRAM, FEAT_RPAK),                                                             // Bass Rush - ECOGEAR PowerWorm Championship (J)
    MATCH_ID("NVP", SAVE_TYPE_SRAM, FEAT_CPAK | FEAT_RPAK),                                                 // Virtual Pro Wrestling 64
    MATCH_ID("NW2", SAVE_TYPE_SRAM, FEAT_RPAK),                                                             // WCW-nWo Revenge
    MATCH_ID("NWL", SAVE_TYPE_SRAM, FEAT_RPAK),                                                             // Waialae Country Club: True Golf Classics
    MATCH_ID("NWX", SAVE_TYPE_SRAM, FEAT_CPAK | FEAT_RPAK),                                                 // WWF WrestleMania 2000
    MATCH_ID("NYW", SAVE_TYPE_SRAM, FEAT_NONE),                                                             // Harvest Moon 64
    MATCH_ID("NZL", SAVE_TYPE_SRAM, FEAT_RPAK),                                                             // Legend of Zelda: Ocarina of Time (E)

    MATCH_ID("CDZ", SAVE_TYPE_SRAM_BANKED, FEAT_RPAK | FEAT_64DD_ENHANCED),                                 // Dezaemon 3D

    MATCH_ID("CP2", SAVE_TYPE_FLASHRAM, FEAT_TPAK | FEAT_64DD_ENHANCED),                                    // Pocket Monsters Stadium 2 (J)
    MATCH_ID("NAF", SAVE_TYPE_FLASHRAM, FEAT_CPAK | FEAT_RTC),                                              // Doubutsu no Mori
    MATCH_ID("NCC", SAVE_TYPE_FLASHRAM, FEAT_RPAK),                                                         // Command & Conquer
    MATCH_ID("NCK", SAVE_TYPE_FLASHRAM, FEAT_RPAK),                                                         // NBA Courtside 2 featuring Kobe Bryant
    MATCH_ID("NDA", SAVE_TYPE_FLASHRAM, FEAT_CPAK),                                                         // Derby Stallion 64
    MATCH_ID("NDP", SAVE_TYPE_FLASHRAM, FEAT_EXP_PAK_REQUIRED),                                             // Dinosaur Planet (Unlicensed)
    MATCH_ID("NJF", SAVE_TYPE_FLASHRAM, FEAT_RPAK),                                                         // Jet Force Gemini [Star Twins (J)]
    MATCH_ID("NKJ", SAVE_TYPE_FLASHRAM, FEAT_RPAK),                                                         // Ken Griffey Jr.'s Slugfest
    MATCH_ID("NM6", SAVE_TYPE_FLASHRAM, FEAT_RPAK),                                                         // Mega Man 64
    MATCH_ID("NMQ", SAVE_TYPE_FLASHRAM, FEAT_RPAK),                                                         // Paper Mario
    MATCH_ID("NPF", SAVE_TYPE_FLASHRAM, FEAT_NONE),                                                         // Pokemon Snap [Pocket Monsters Snap (J)]
    MATCH_ID("NPN", SAVE_TYPE_FLASHRAM, FEAT_NONE),                                                         // Pokemon Puzzle League
    MATCH_ID("NPO", SAVE_TYPE_FLASHRAM, FEAT_TPAK),                                                         // Pokemon Stadium
    MATCH_ID("NRH", SAVE_TYPE_FLASHRAM, FEAT_RPAK),                                                         // Rockman Dash - Hagane no Boukenshin (J)
    MATCH_ID("NSQ", SAVE_TYPE_FLASHRAM, FEAT_RPAK | FEAT_EXP_PAK_RECOMMENDED),                              // StarCraft 64
    MATCH_ID("NT9", SAVE_TYPE_FLASHRAM, FEAT_NONE),                                                         // Tigger's Honey Hunt
    MATCH_ID("NW4", SAVE_TYPE_FLASHRAM, FEAT_CPAK | FEAT_RPAK),                                             // WWF No Mercy
    MATCH_ID("NZS", SAVE_TYPE_FLASHRAM, FEAT_RPAK | FEAT_EXP_PAK_REQUIRED),                                 // Legend of Zelda: Majora's Mask [Zelda no Densetsu - Mujura no Kamen (J)]

    MATCH_ID("NP3", SAVE_TYPE_FLASHRAM_PKST2, FEAT_TPAK),                                                   // Pokemon Stadium 2 [Pocket Monsters Stadium - Kin Gin (J)]

    MATCH_ID("N22", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Ready 2 Rumble Boxing - Round 2
    MATCH_ID("N2M", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Madden Football 2002
    MATCH_ID("N32", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Army Men - Sarge's Heroes 2
    MATCH_ID("N3P", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Triple Play 2000
    MATCH_ID("N64", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Kira to Kaiketsu! 64 Tanteidan
    MATCH_ID("N7I", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // FIFA Soccer 64 [FIFA 64 (E)]
    MATCH_ID("N8I", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // FIFA - Road to World Cup 98 [World Cup e no Michi (J)]
    MATCH_ID("N8M", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Madden Football 64
    MATCH_ID("N8W", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // World Cup '98
    MATCH_ID("N9B", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // NBA Live '99
    MATCH_ID("N9C", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Nascar '99
    MATCH_ID("N9F", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // FIFA 99
    MATCH_ID("N9H", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // NHL '99
    MATCH_ID("N9M", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Madden Football '99
    MATCH_ID("NAC", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Army Men - Air Combat
    MATCH_ID("NAH", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Asteroids Hyper 64
    MATCH_ID("NAI", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Midway's Greatest Arcade Hits Volume 1
    MATCH_ID("NAM", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Army Men - Sarge's Heroes
    MATCH_ID("NAR", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Armorines - Project S.W.A.R.M.
    MATCH_ID("NAS", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // All-Star Baseball 2001
    MATCH_ID("NAY", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Aidyn Chronicles - The First Mage
    MATCH_ID("NB2", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // NBA In the Zone '99 [NBA Pro '99 (E)]
    MATCH_ID("NB3", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Bust-A-Move '99 [Bust-A-Move 3 DX (E)]
    MATCH_ID("NB4", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Bass Masters 2000
    MATCH_ID("NB8", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Beetle Adventure Racing (J)
    MATCH_ID("NB9", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // NBA Jam '99
    MATCH_ID("NBA", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // NBA In the Zone '98 [NBA Pro '98 (E)]
    MATCH_ID("NBE", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // All-Star Baseball 2000
    MATCH_ID("NBF", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Bio F.R.E.A.K.S.
    MATCH_ID("NBI", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // NFL Blitz 2000
    MATCH_ID("NBJ", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Bakushou Jinsei 64 - Mezase! Resort Ou
    MATCH_ID("NBL", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Buck Bumble
    MATCH_ID("NBO", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Bottom of the 9th
    MATCH_ID("NBP", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Blues Brothers 2000
    MATCH_ID("NBQ", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Battletanx - Global Assault
    MATCH_ID("NBR", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Milo's Astro Lanes
    MATCH_ID("NBS", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // All-Star Baseball '99
    MATCH_ID("NBU", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Bust-A-Move 2 - Arcade Edition
    MATCH_ID("NBW", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Super Bowling
    MATCH_ID("NBX", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Battletanx
    MATCH_ID("NBY", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Bug's Life, A
    MATCH_ID("NBZ", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // NFL Blitz
    MATCH_ID("NCB", SAVE_TYPE_NONE, FEAT_RPAK),                                                             // Charlie Blast's Territory
    MATCH_ID("NCD", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Carmageddon 64
    MATCH_ID("NCE", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Nuclear Strike 64
    MATCH_ID("NCL", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // California Speed
    MATCH_ID("NCO", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Jeremy McGrath Supercross 2000
    MATCH_ID("NCS", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // S.C.A.R.S.
    MATCH_ID("NDC", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // SD Hiryuu no Ken Densetsu (J)
    MATCH_ID("NDE", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Destruction Derby 64
    MATCH_ID("NDF", SAVE_TYPE_NONE, FEAT_RPAK),                                                             // Dance Dance Revolution - Disney Dancing Museum
    MATCH_ID("NDH", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Duel Heroes
    MATCH_ID("NDM", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Doom 64
    MATCH_ID("NDN", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Duke Nukem 64
    MATCH_ID("NDQ", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Disney's Donald Duck - Goin' Quackers [Quack Attack (E)]
    MATCH_ID("NDS", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // J.League Dynamite Soccer 64
    MATCH_ID("NDT", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // South Park
    MATCH_ID("NDW", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Daikatana, John Romero's
    MATCH_ID("NDZ", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Duke Nukem - Zero Hour
    MATCH_ID("NEG", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Extreme-G
    MATCH_ID("NET", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Quest 64 [Eltale Monsters (J) Holy Magic Century (E)]
    MATCH_ID("NF9", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Fox Sports College Hoops '99
    MATCH_ID("NFB", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // NFL Blitz 2001
    MATCH_ID("NFD", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Flying Dragon
    MATCH_ID("NFF", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Fighting Force 64
    MATCH_ID("NFL", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Madden Football 2001
    MATCH_ID("NFO", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Forsaken 64
    MATCH_ID("NFQ", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Razor Freestyle Scooter
    MATCH_ID("NFR", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // F-1 Racing Championship
    MATCH_ID("NFS", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Famista 64
    MATCH_ID("NG2", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Extreme-G XG2
    MATCH_ID("NG5", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Ganbare Goemon - Neo Momoyama Bakufu no Odori [Mystical Ninja Starring Goemon]
    MATCH_ID("NGA", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Deadly Arts [G.A.S.P!! Fighter's NEXTream (E-J)]
    MATCH_ID("NGB", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Top Gear Hyper Bike
    MATCH_ID("NGD", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Gauntlet Legends (J)
    MATCH_ID("NGM", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Goemon's Great Adventure [Mystical Ninja 2 Starring Goemon]
    MATCH_ID("NGN", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Golden Nugget 64
    MATCH_ID("NGR", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Top Gear Rally (U)
    MATCH_ID("NGS", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Jikkyou G1 Stable
    MATCH_ID("NGX", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Gauntlet Legends
    MATCH_ID("NH5", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Nagano Winter Olympics '98 [Hyper Olympics in Nagano (J)]
    MATCH_ID("NH9", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // NHL Breakaway '99
    MATCH_ID("NHC", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Hercules - The Legendary Journeys
    MATCH_ID("NHG", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // F-1 Pole Position 64
    MATCH_ID("NHG", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Human Grand Prix - New Generation 
    MATCH_ID("NHK", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Hiryuu no Ken Twin
    MATCH_ID("NHL", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // NHL Breakaway '98
    MATCH_ID("NHM", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Mia Hamm Soccer 64
    MATCH_ID("NHN", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Olympic Hockey Nagano '98
    MATCH_ID("NHO", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // NHL Blades of Steel '99 [NHL Pro '99 (E)]
    MATCH_ID("NHS", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Hamster Monogatari 64
    MATCH_ID("NHT", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Hydro Thunder
    MATCH_ID("NHV", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Hybrid Heaven (U + E)
    MATCH_ID("NHW", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Hot Wheels Turbo Racing
    MATCH_ID("NHX", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Hexen
    MATCH_ID("NIS", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // International Superstar Soccer 2000 
    MATCH_ID("NIV", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Space Invaders
    MATCH_ID("NJ2", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Wonder Project J2 - Koruro no Mori no Jozet (J)
    MATCH_ID("NJ3", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Jikkyou World Soccer 3
    MATCH_ID("NJA", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // NBA Jam 2000
    MATCH_ID("NJE", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // J.League Eleven Beat 1997
    MATCH_ID("NJL", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // J.League Live 64
    MATCH_ID("NJP", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // International Superstar Soccer 64 [Jikkyo J-League Perfect Striker (J)]
    MATCH_ID("NJQ", SAVE_TYPE_NONE, FEAT_RPAK),                                                             // Batman Beyond - Return of the Joker [Batman of the Future - Return of the Joker (E)]
    MATCH_ID("NKE", SAVE_TYPE_NONE, FEAT_RPAK),                                                             // Knife Edge - Nose Gunner
    MATCH_ID("NKK", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Knockout Kings 2000
    MATCH_ID("NKM", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Pro Mahjong Kiwame 64 (J)
    MATCH_ID("NKR", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Rakuga Kids (E)
    MATCH_ID("NL2", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Top Gear Rally 2
    MATCH_ID("NLC", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Automobili Lamborghini [Super Speed Race 64 (J)]
    MATCH_ID("NLG", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // LEGO Racers
    MATCH_ID("NM3", SAVE_TYPE_NONE, FEAT_RPAK),                                                             // Monster Truck Madness 64
    MATCH_ID("NM4", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Mortal Kombat 4
    MATCH_ID("NM9", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Harukanaru Augusta Masters 98
    MATCH_ID("NMA", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Jangou Simulation Mahjong Do 64
    MATCH_ID("NMB", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Mike Piazza's Strike Zone
    MATCH_ID("NMD", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Madden Football 2000
    MATCH_ID("NMJ", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Mahjong 64
    MATCH_ID("NMM", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Mahjong Master
    MATCH_ID("NMT", SAVE_TYPE_NONE, FEAT_RPAK),                                                             // Magical Tetris Challenge
    MATCH_ID("NMY", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Mortal Kombat Mythologies - Sub-Zero
    MATCH_ID("NN2", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Nascar 2000
    MATCH_ID("NNC", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Nightmare Creatures
    MATCH_ID("NNL", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // NBA Live 2000
    MATCH_ID("NNM", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Namco Museum 64
    MATCH_ID("NNR", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Pro Mahjong Tsuwamono 64 - Jansou Battle ni Chousen (J)
    MATCH_ID("NNS", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Beetle Adventure Racing
    MATCH_ID("NO7", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // The World Is Not Enough
    MATCH_ID("NOF", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Offroad Challenge
    MATCH_ID("NOH", SAVE_TYPE_NONE, FEAT_RPAK | FEAT_TPAK),                                                 // Transformers Beast Wars - Transmetals
    MATCH_ID("NOM", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Onegai Monsters
    MATCH_ID("NOW", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Brunswick Circuit Pro Bowling
    MATCH_ID("NP9", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Ms. Pac-Man - Maze Madness
    MATCH_ID("NPB", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Puzzle Bobble 64 (J)
    MATCH_ID("NPC", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Pachinko 365 Nichi (J)
    MATCH_ID("NPK", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Chou Kuukan Night Pro Yakyuu King (J)
    MATCH_ID("NPL", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Power League 64 (J)
    MATCH_ID("NPR", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // South Park Rally
    MATCH_ID("NPU", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Power Rangers - Lightspeed Rescue
    MATCH_ID("NPX", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Polaris SnoCross
    MATCH_ID("NPZ", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Susume! Taisen Puzzle Dama Toukon! Marumata Chou (J)
    MATCH_ID("NQ2", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Quake 2
    MATCH_ID("NQ8", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // NFL Quarterback Club '98
    MATCH_ID("NQ9", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // NFL Quarterback Club '99
    MATCH_ID("NQB", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // NFL Quarterback Club 2000
    MATCH_ID("NQC", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // NFL Quarterback Club 2001
    MATCH_ID("NQK", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Quake 64
    MATCH_ID("NR2", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Rush 2 - Extreme Racing USA
    MATCH_ID("NR3", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Stunt Racer 64
    MATCH_ID("NR6", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Tom Clancy's Rainbow Six
    MATCH_ID("NRD", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Ready 2 Rumble Boxing
    MATCH_ID("NRG", SAVE_TYPE_NONE, FEAT_RPAK),                                                             // Rugrats - Scavenger Hunt [Treasure Hunt (E)]
    MATCH_ID("NRK", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Rugrats in Paris - The Movie
    MATCH_ID("NRO", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Road Rash 64
    MATCH_ID("NRP", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Rampage - World Tour
    MATCH_ID("NRP", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Rampage 2 - Universal Tour
    MATCH_ID("NRR", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Roadster's Trophy
    MATCH_ID("NRT", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Rat Attack
    MATCH_ID("NRT", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Robotron 64
    MATCH_ID("NRU", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // San Francisco Rush 2049
    MATCH_ID("NRV", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Re-Volt
    MATCH_ID("NRW", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Turok: Rage Wars
    MATCH_ID("NS2", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Simcity 2000
    MATCH_ID("NSB", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Twisted Edge - Extreme Snowboarding [King Hill 64 - Extreme Snowboarding (J)]
    MATCH_ID("NSD", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Shadow Man
    MATCH_ID("NSF", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // San Francisco Rush - Extreme Racing
    MATCH_ID("NSG", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Shadowgate 64 - Trials Of The Four Towers
    MATCH_ID("NSH", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Saikyou Habu Shougi (J)
    MATCH_ID("NSK", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Snowboard Kids [Snobow Kids (J)]
    MATCH_ID("NSL", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Spider-Man
    MATCH_ID("NSO", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // NBA Showtime - NBA on NBC
    MATCH_ID("NSP", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Superman
    MATCH_ID("NST", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Eikou no Saint Andrews
    MATCH_ID("NSX", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Supercross 2000
    MATCH_ID("NSY", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Scooby-Doo! - Classic Creep Capers
    MATCH_ID("NSZ", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // NFL Blitz - Special Edition
    MATCH_ID("NT2", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Turok 2 - Seeds of Evil [Violence Killer - Turok New Generation (J)]
    MATCH_ID("NT3", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Tony Hawk's Pro Skater 3
    MATCH_ID("NT4", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // CyberTiger
    MATCH_ID("NTA", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Disney's Tarzan
    MATCH_ID("NTF", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Tony Hawk's Pro Skater
    MATCH_ID("NTH", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Toy Story 2 - Buzz Lightyear to the Rescue!
    MATCH_ID("NTI", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // WWF: Attitude
    MATCH_ID("NTK", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Turok 3 - Shadow of Oblivion
    MATCH_ID("NTO", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Shin Nihon Pro Wrestling - Toukon Road - Brave Spirits (J)
    MATCH_ID("NTQ", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Tony Hawk's Pro Skater 2
    MATCH_ID("NTS", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Centre Court Tennis [Let's Smash (J)]
    MATCH_ID("NTT", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Tonic Trouble
    MATCH_ID("NTU", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Turok: Dinosaur Hunter [Turok: Jikuu Senshi (J)]
    MATCH_ID("NV2", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Chameleon Twist 2
    MATCH_ID("NV3", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Micro Machines 64 Turbo
    MATCH_ID("NV8", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Vigilante 8
    MATCH_ID("NVC", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Virtual Chess 64
    MATCH_ID("NVG", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Vigilante 8 - Second Offense
    MATCH_ID("NVR", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Virtual Pool 64
    MATCH_ID("NW3", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // WCW: Nitro
    MATCH_ID("NW8", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Wayne Gretzky's 3D Hockey '98
    MATCH_ID("NWB", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Iggy's Reckin' Balls [Iggy-kun no Bura Bura Poyon (J)]
    MATCH_ID("NWD", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Winback - Covert Operations
    MATCH_ID("NWF", SAVE_TYPE_NONE, FEAT_RPAK),                                                             // Wheel of Fortune
    MATCH_ID("NWG", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Wayne Gretzky's 3D Hockey
    MATCH_ID("NWI", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // ECW Hardcore Revolution
    MATCH_ID("NWK", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Michael Owens WLS 2000 [World League Soccer 2000 (E) / Telefoot Soccer 2000 (F)]
    MATCH_ID("NWM", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // WCW: Mayhem
    MATCH_ID("NWN", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // WCW vs. nWo - World Tour
    MATCH_ID("NWO", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // World Driver Championship
    MATCH_ID("NWP", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Wipeout 64
    MATCH_ID("NWS", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // International Superstar Soccer '98 [Jikkyo World Soccer - World Cup France '98 (J)]
    MATCH_ID("NWV", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // WCW: Backstage Assault
    MATCH_ID("NWW", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // WWF: War Zone
    MATCH_ID("NWZ", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // NBA In the Zone 2000
    MATCH_ID("NX2", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Gex 64 - Enter the Gecko
    MATCH_ID("NX3", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Gex 3 - Deep Cover Gecko
    MATCH_ID("NXF", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Xena Warrior Princess - The Talisman of Fate
    MATCH_ID("NXG", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // NBA Hangtime
    MATCH_ID("NY2", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Rayman 2 - The Great Escape
    MATCH_ID("NYP", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Paperboy
    MATCH_ID("NYW", SAVE_TYPE_NONE, FEAT_CPAK),                                                             // Bokujou Monogatari 2
    MATCH_ID("NZO", SAVE_TYPE_NONE, FEAT_CPAK | FEAT_RPAK),                                                 // Battlezone - Rise of the Black Dogs

    MATCH_END,
};


static void fix_rom_header_endianness (rom_header_t *rom_header, rom_info_t *rom_info) {
    uint8_t *raw = (uint8_t *) (rom_header);

    switch (rom_header->pi_dom1_config) {
        case PI_CONFIG_LITTLE_ENDIAN:
            rom_info->endianness = ENDIANNESS_LITTLE;
            for (int i = 0; i < sizeof(rom_header_t); i += 4) {
                SWAP_VARS(raw[i + 0], raw[i + 3]);
                SWAP_VARS(raw[i + 1], raw[i + 2]);
            }
            break;

        case PI_CONFIG_BYTE_SWAPPED:
            rom_info->endianness = ENDIANNESS_BYTE_SWAP;
            for (int i = 0; i < sizeof(rom_header_t); i += 2) {
                SWAP_VARS(raw[i + 0], raw[i + 1]);
            }
            break;

        default:
            rom_info->endianness = ENDIANNESS_BIG;
            break;
    }
}

static bool compare_id (const match_t *match, rom_header_t *rom_header) {
    int characters_to_check = (match->type == MATCH_TYPE_ID) ? 3 : 4;

    for (int i = (characters_to_check - 1); i >= 0; i--) {
        if (match->fields.id[i] != rom_header->game_code[i]) {
            return false;
        }
    }

    if (match->type == MATCH_TYPE_ID_REGION_VERSION) {
        if (match->fields.version != rom_header->version) {
            return false;
        }
    }

    return true;
}

static match_t find_rom_in_database (rom_header_t *rom_header) {
    const match_t *match;

    for (match = database; match->type != MATCH_TYPE_END; match++) {
        switch (match->type) {
            case MATCH_TYPE_ID:
            case MATCH_TYPE_ID_REGION:
            case MATCH_TYPE_ID_REGION_VERSION:
                if (compare_id(match, rom_header)) {
                    return *match;
                }
                break;

            case MATCH_TYPE_CHECK_CODE:
                if (match->fields.check_code == rom_header->check_code) {
                    return *match;
                }
                break;

            case MATCH_TYPE_HOMEBREW_HEADER:
                if (strncmp(match->fields.id, rom_header->unique_code, sizeof(rom_header->unique_code)) == 0) {
                    return *match;
                }
                break;

            default:
                break;
        }
    }

    return *match;
}

static uint32_t fix_boot_address (cic_type_t cic_type, uint32_t boot_address) {
    switch (cic_type) {
        case CIC_x103: return (boot_address - 0x100000);
        case CIC_x106: return (boot_address - 0x200000);
        default: return boot_address;
    }
}

static void extract_rom_info (match_t *match, rom_header_t *rom_header, rom_info_t *rom_info) {
    rom_info->cic_type = cic_detect(rom_header->ipl3);

    if (match->type == MATCH_TYPE_HOMEBREW_HEADER) {
        if (rom_header->version & (1 << 0)) {
            match->data.feat |= FEAT_RTC;
        }
        switch ((rom_header->version & 0xF0) >> 4) {
            case 0: match->data.save = SAVE_TYPE_NONE; break;
            case 1: match->data.save = SAVE_TYPE_EEPROM_4K; break;
            case 2: match->data.save = SAVE_TYPE_EEPROM_16K; break;
            case 3: match->data.save = SAVE_TYPE_SRAM; break;
            case 4: match->data.save = SAVE_TYPE_SRAM_BANKED; break;
            case 5: match->data.save = SAVE_TYPE_FLASHRAM; break;
            case 6: match->data.save = SAVE_TYPE_SRAM_128K; break;
            default: match->data.save = SAVE_TYPE_NONE; break;
        }
    }

    rom_info->clock_rate = (rom_header->clock_rate == CLOCK_RATE_DEFAULT) ? 62.5f : (rom_header->clock_rate & ~(CLOCK_RATE_DEFAULT)) / 1000000.0f;
    rom_info->boot_address = fix_boot_address(rom_info->cic_type, rom_header->boot_address);
    rom_info->libultra.version = rom_header->libultra.version;
    rom_info->libultra.revision = rom_header->libultra.revision;
    rom_info->check_code = rom_header->check_code;
    memcpy(rom_info->title, rom_header->title, sizeof(rom_info->title));
    memcpy(rom_info->game_code, rom_header->game_code, sizeof(rom_info->game_code));
    rom_info->version = rom_header->version;

    rom_info->save_type = match->data.save;

    rom_info->features.controller_pak = (match->data.feat & FEAT_CPAK);
    rom_info->features.rumble_pak = (match->data.feat & FEAT_RPAK);
    rom_info->features.transfer_pak = (match->data.feat & FEAT_TPAK);
    rom_info->features.voice_recognition_unit = (match->data.feat & FEAT_VRU);
    rom_info->features.real_time_clock = (match->data.feat & FEAT_RTC);
    rom_info->features.disk_conversion = (match->data.feat & FEAT_64DD_CONVERSION);
    rom_info->features.combo_rom_disk_game = (match->data.feat & FEAT_64DD_ENHANCED);
    if (match->data.feat & FEAT_EXP_PAK_REQUIRED) {
        rom_info->features.expansion_pak = EXPANSION_PAK_REQUIRED;
    } else if (match->data.feat & FEAT_EXP_PAK_RECOMMENDED) {
        rom_info->features.expansion_pak = EXPANSION_PAK_RECOMMENDED;
    } else if (match->data.feat & FEAT_EXP_PAK_ENHANCED) {
        rom_info->features.expansion_pak = EXPANSION_PAK_SUGGESTED;
    } else if (match->data.feat & FEAT_EXP_PAK_BROKEN) {
        rom_info->features.expansion_pak = EXPANSION_PAK_FAULTY;
    } else {
        rom_info->features.expansion_pak = EXPANSION_PAK_NONE;
    }
}


rom_err_t rom_info_load (char *path, rom_info_t *rom_info) {
    FIL fil;
    UINT br;
    rom_header_t rom_header;

    if (f_open(&fil, strip_sd_prefix(path), FA_READ) != FR_OK) {
        return ROM_ERR_NO_FILE;
    }
    if (f_read(&fil, &rom_header, sizeof(rom_header), &br) != FR_OK) {
        f_close(&fil);
        return ROM_ERR_IO;
    }
    if (f_close(&fil) != FR_OK) {
        return ROM_ERR_IO;
    }
    if (br != sizeof(rom_header)) {
        return ROM_ERR_IO;
    }

    fix_rom_header_endianness(&rom_header, rom_info);

    match_t match = find_rom_in_database(&rom_header);

    extract_rom_info(&match, &rom_header, rom_info);

    return ROM_OK;
}
