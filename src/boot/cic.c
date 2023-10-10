#include "cic.h"
#include "crc32.h"


typedef struct {
    const uint32_t crc32;
    const cic_type_t type;
} ipl3_crc32_t;

static const ipl3_crc32_t ipl3_crc32[] = {
    { .crc32 = 0x587BD543, .type = CIC_5101 },
    { .crc32 = 0x0E018159, .type = CIC_5167 },
    { .crc32 = 0x6170A4A1, .type = CIC_6101 },
    { .crc32 = 0x009E9EA3, .type = CIC_7102 },
    { .crc32 = 0x90BB6CB5, .type = CIC_6102_7101 },
    { .crc32 = 0x0B050EE0, .type = CIC_x103 },
    { .crc32 = 0x98BC2C86, .type = CIC_x105 },
    { .crc32 = 0xACC8580A, .type = CIC_x106 },
    { .crc32 = 0xBC605D0A, .type = CIC_8301 },
    { .crc32 = 0x502C4466, .type = CIC_8302 },
    { .crc32 = 0x0C965795, .type = CIC_8303 },
    { .crc32 = 0x10C68B18, .type = CIC_8401 },
    { .crc32 = 0x8FEBA21E, .type = CIC_8501 },
};


cic_type_t cic_detect (uint8_t *ipl3) {
    uint32_t crc32 = crc32_calculate(ipl3, IPL3_LENGTH);

    for (int i = 0; i < sizeof(ipl3_crc32) / sizeof(ipl3_crc32_t); i++) {
        if (ipl3_crc32[i].crc32 == crc32) {
            return ipl3_crc32[i].type;
        }
    }

    return CIC_UNKNOWN;
}

uint8_t cic_get_seed (cic_type_t cic_type) {
    switch (cic_type) {
        case CIC_5101: return 0xAC;
        case CIC_5167: return 0xDD;
        case CIC_6101: return 0x3F;
        case CIC_7102: return 0x3F;
        case CIC_6102_7101: return 0x3F;
        case CIC_x103: return 0x78;
        case CIC_x105: return 0x91;
        case CIC_x106: return 0x85;
        case CIC_8301: return 0xDD;
        case CIC_8302: return 0xDD;
        case CIC_8303: return 0xDD;
        case CIC_8401: return 0xDD;
        case CIC_8501: return 0xDE;
        default: return 0x3F;
    }
}
