#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <fatfs/ff.h>
#include <libdragon.h>

#include "utils/fs.h"
#include "utils/utils.h"

#include "../flashcart_utils.h"
#include "sc64_ll.h"
#include "sc64.h"


#define SRAM_FLASHRAM_ADDRESS       (0x08000000)
#define ROM_ADDRESS                 (0x10000000)
#define IPL_ADDRESS                 (0x13BC0000)
#define EXTENDED_ADDRESS            (0x14000000)
#define SHADOW_ADDRESS              (0x1FFC0000)
#define EEPROM_ADDRESS              (0x1FFE2000)

#define SUPPORTED_MAJOR_VERSION     (2)
#define SUPPORTED_MINOR_VERSION     (17)
#define SUPPORTED_REVISION          (0)

#define DISK_MAPPING_ROM_OFFSET     (0x02000000)
#define DISK_MAX_SECTORS            (126820)

#define DISK_TRACKS                 (1175)
#define DISK_HEADS                  (2)
#define DISK_BLOCKS                 (2)
#define DISK_SECTORS_PER_BLOCK      (85)
#define DISK_ZONES                  (16)
#define DISK_BAD_TRACKS_PER_ZONE    (12)
#define DISK_TYPES                  (7)
#define DISK_SYSTEM_LBA_COUNT       (24)

#define THB_UNMAPPED                (0xFFFFFFFF)
#define THB_WRITABLE_FLAG           (1 << 31)


static const struct {
    uint8_t head;
    uint8_t sector_length;
    uint8_t tracks;
    uint16_t track_offset;
} zone_mapping[DISK_ZONES] = {
    { 0, 232, 158, 0 },
    { 0, 216, 158, 158 },
    { 0, 208, 149, 316 },
    { 0, 192, 149, 465 },
    { 0, 176, 149, 614 },
    { 0, 160, 149, 763 },
    { 0, 144, 149, 912 },
    { 0, 128, 114, 1061 },
    { 1, 216, 158, 0 },
    { 1, 208, 158, 158 },
    { 1, 192, 149, 316 },
    { 1, 176, 149, 465 },
    { 1, 160, 149, 614 },
    { 1, 144, 149, 763 },
    { 1, 128, 149, 912 },
    { 1, 112, 114, 1061 },
};
static const uint8_t vzone_to_pzone[DISK_TYPES][DISK_ZONES] = {
    {0, 1, 2, 9, 8, 3, 4, 5, 6, 7, 15, 14, 13, 12, 11, 10},
    {0, 1, 2, 3, 10, 9, 8, 4, 5, 6, 7, 15, 14, 13, 12, 11},
    {0, 1, 2, 3, 4, 11, 10, 9, 8, 5, 6, 7, 15, 14, 13, 12},
    {0, 1, 2, 3, 4, 5, 12, 11, 10, 9, 8, 6, 7, 15, 14, 13},
    {0, 1, 2, 3, 4, 5, 6, 13, 12, 11, 10, 9, 8, 7, 15, 14},
    {0, 1, 2, 3, 4, 5, 6, 7, 14, 13, 12, 11, 10, 9, 8, 15},
    {0, 1, 2, 3, 4, 5, 6, 7, 15, 14, 13, 12, 11, 10, 9, 8},
};
static const uint8_t rom_zones[DISK_TYPES] = { 5, 7, 9, 11, 13, 15, 16 };


static uint32_t disk_sectors_start_offset;


static flashcart_err_t load_to_flash (FIL *fil, void *address, size_t size, UINT *br, flashcart_progress_callback_t *progress) {
    size_t erase_block_size;
    UINT bp;

    *br = 0;

    if (sc64_ll_flash_get_erase_block_size(&erase_block_size) != SC64_OK) {
        return FLASHCART_ERR_INT;
    }

    while (size > 0) {
        size_t program_size = MIN(size, erase_block_size);
        if (sc64_ll_flash_erase_block(address) != SC64_OK) {
            return FLASHCART_ERR_INT;
        }
        if (f_read(fil, address, program_size, &bp) != FR_OK) {
            return FLASHCART_ERR_LOAD;
        }
        if (sc64_ll_flash_wait_busy() != SC64_OK) {
            return FLASHCART_ERR_INT;
        }
        if (progress) {
            progress(f_tell(fil) / (float) (f_size(fil)));
        }
        address += program_size;
        size -= program_size;
        *br += bp;
    }

    return FLASHCART_OK;
}

static uint32_t disk_sectors_start (uint32_t offset) {
    disk_sectors_start_offset = offset;
    return (offset + (DISK_MAX_SECTORS * sizeof(uint32_t)));
}

static void disk_sectors_callback (uint32_t sector_count, uint32_t file_sector, uint32_t cluster_sector, uint32_t cluster_size) {
    for (uint32_t i = 0; i < cluster_size; i++) {
        uint32_t offset = file_sector + i;
        uint32_t sector = cluster_sector + i;

        if ((offset > DISK_MAX_SECTORS) || (offset > sector_count)) {
            return;
        }

        io_write(ROM_ADDRESS + disk_sectors_start_offset + (offset * sizeof(uint32_t)), sector);
    }
}

static bool disk_zone_track_is_bad (uint8_t zone, uint8_t track, flashcart_disk_parameters_t *disk_parameters) {
    for (int i = 0; i < DISK_BAD_TRACKS_PER_ZONE; i++) {
        if (disk_parameters->defect_tracks[zone][i] == track) {
            return true;
        }
    }

    return false;
}

static bool disk_system_lba_is_bad (uint16_t lba, flashcart_disk_parameters_t *disk_parameters) {
    if (lba < DISK_SYSTEM_LBA_COUNT) {
        return disk_parameters->bad_system_area_lbas[lba];
    }

    return false;
}

static void disk_set_thb_mapping (uint32_t offset, uint16_t track, uint8_t head, uint8_t block, bool valid, bool writable, int file_offset) {
    uint32_t index = (track << 2) | (head << 1) | (block);
    uint32_t mapping = valid ? ((writable ? THB_WRITABLE_FLAG : 0) | (file_offset & ~(THB_WRITABLE_FLAG))) : THB_UNMAPPED;

    io_write(ROM_ADDRESS + offset + (index * sizeof(uint32_t)), mapping);
}

static uint32_t disk_load_thb_table (uint32_t offset, flashcart_disk_parameters_t *disk_parameters) {
    int file_offset = 0;

    uint16_t lba = 0;
    uint8_t starting_block = 0;

    for (uint8_t vzone = 0; vzone < DISK_ZONES; vzone++) {
        uint8_t pzone = vzone_to_pzone[disk_parameters->disk_type][vzone];

        uint8_t head = zone_mapping[pzone].head;
        uint8_t sector_length = zone_mapping[pzone].sector_length;
        uint8_t tracks = zone_mapping[pzone].tracks;
        uint16_t track_offset = zone_mapping[pzone].track_offset;

        bool reverse = (head != 0);
        int zone_track_start = (reverse ? (tracks - 1) : 0);
        int zone_track_end = (reverse ? (-1) : tracks);

        for (int zone_track = zone_track_start; zone_track != zone_track_end; zone_track += (reverse ? (-1) : 1)) {
            uint16_t track = track_offset + zone_track;

            if (disk_zone_track_is_bad(pzone, zone_track, disk_parameters)) {
                disk_set_thb_mapping(offset, track, head, 0, false, false, 0);
                disk_set_thb_mapping(offset, track, head, 1, false, false, 0);
                continue;
            }

            for (uint8_t block = 0; block < DISK_BLOCKS; block += 1) {
                bool valid = !(disk_system_lba_is_bad(lba, disk_parameters));
                bool writable = (vzone >= rom_zones[disk_parameters->disk_type]);
                disk_set_thb_mapping(offset, track, head, (starting_block ^ block), valid, writable, file_offset);
                file_offset += (sector_length * DISK_SECTORS_PER_BLOCK);
                lba += 1;
            }

            starting_block ^= 1;
        }
    }

    return (offset + (DISK_TRACKS * DISK_HEADS * DISK_BLOCKS * sizeof(uint32_t)));
}


static flashcart_err_t sc64_init (void) {
    uint16_t major;
    uint16_t minor;
    uint32_t revision;

    if (sc64_ll_get_version(&major, &minor, &revision) != SC64_OK) {
        return FLASHCART_ERR_OUTDATED;
    }
    if (major != SUPPORTED_MAJOR_VERSION) {
        return FLASHCART_ERR_OUTDATED;
    }
    if (minor < SUPPORTED_MINOR_VERSION) {
        return FLASHCART_ERR_OUTDATED;
    } else if (minor == SUPPORTED_MINOR_VERSION && revision < SUPPORTED_REVISION) {
        return FLASHCART_ERR_OUTDATED;
    }

    bool writeback_pending;
    do {
        if (sc64_ll_writeback_pending(&writeback_pending) != SC64_OK) {
            return FLASHCART_ERR_INT;
        }
    } while (writeback_pending);

    const struct {
        sc64_cfg_id_t id;
        uint32_t value;
    } default_config[] = {
        { CFG_ID_BOOTLOADER_SWITCH, false },
        { CFG_ID_ROM_WRITE_ENABLE, true },
        { CFG_ID_ROM_SHADOW_ENABLE, false },
        { CFG_ID_DD_MODE, DD_MODE_DISABLED },
        { CFG_ID_ISV_ADDRESS, 0x00000000 },
        { CFG_ID_BOOT_MODE, BOOT_MODE_MENU },
        { CFG_ID_SAVE_TYPE, SAVE_TYPE_NONE },
        { CFG_ID_CIC_SEED, CIC_SEED_AUTO },
        { CFG_ID_TV_TYPE, TV_TYPE_PASSTHROUGH },
        { CFG_ID_DD_SD_ENABLE, false },
        { CFG_ID_DD_DRIVE_TYPE, DRIVE_TYPE_RETAIL },
        { CFG_ID_DD_DISK_STATE, DISK_STATE_EJECTED },
        { CFG_ID_BUTTON_MODE, BUTTON_MODE_NONE },
        { CFG_ID_ROM_EXTENDED_ENABLE, false },
    };

    for (int i = 0; i < sizeof(default_config) / sizeof(default_config[0]); i++) {
        if (sc64_ll_set_config(default_config[i].id, default_config[i].value) != SC64_OK) {
            return FLASHCART_ERR_INT;
        }
    }

    return FLASHCART_OK;
}

static flashcart_err_t sc64_deinit (void) {
    sc64_ll_set_config(CFG_ID_ROM_WRITE_ENABLE, false);

    sc64_ll_lock();

    return FLASHCART_OK;
}

static bool sc64_has_feature (flashcart_features_t feature) {
    switch (feature) {
        case FLASHCART_FEATURE_64DD: return true;
        default: return false;
    }
}

static flashcart_err_t sc64_load_rom (char *rom_path, flashcart_progress_callback_t *progress) {
    FIL fil;
    UINT br;

    if (f_open(&fil, strip_sd_prefix(rom_path), FA_READ) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    fix_file_size(&fil);

    size_t rom_size = f_size(&fil);

    if (rom_size > MiB(78)) {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }

    bool shadow_enabled = (rom_size > (MiB(64) - KiB(128)));
    bool extended_enabled = (rom_size > MiB(64));

    size_t sdram_size = shadow_enabled ? (MiB(64) - KiB(128)) : rom_size;
    size_t shadow_size = shadow_enabled ? MIN(rom_size - sdram_size, KiB(128)) : 0;
    size_t extended_size = extended_enabled ? rom_size - MiB(64) : 0;

    size_t chunk_size = MiB(1);
    for (int offset = 0; offset < sdram_size; offset += chunk_size) {
        size_t block_size = MIN(sdram_size - offset, chunk_size);
        if (f_read(&fil, (void *) (ROM_ADDRESS + offset), block_size, &br) != FR_OK) {
            f_close(&fil);
            return FLASHCART_ERR_LOAD;
        }
        if (progress) {
            progress(f_tell(&fil) / (float) (f_size(&fil)));
        }
    }
    if (f_tell(&fil) != sdram_size) {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }

    if (sc64_ll_set_config(CFG_ID_ROM_SHADOW_ENABLE, shadow_enabled) != SC64_OK) {
        f_close(&fil);
        return FLASHCART_ERR_INT;
    }

    if (shadow_enabled) {
        flashcart_err_t err = load_to_flash(&fil, (void *) (SHADOW_ADDRESS), shadow_size, &br, progress);
        if (err != FLASHCART_OK) {
            f_close(&fil);
            return err;
        }
        if (br != shadow_size) {
            f_close(&fil);
            return FLASHCART_ERR_LOAD;
        }
    }

    if (sc64_ll_set_config(CFG_ID_ROM_EXTENDED_ENABLE, extended_enabled) != SC64_OK) {
        f_close(&fil);
        return FLASHCART_ERR_INT;
    }

    if (extended_enabled) {
        flashcart_err_t err = load_to_flash(&fil, (void *) (EXTENDED_ADDRESS), extended_size, &br, progress);
        if (err != FLASHCART_OK) {
            f_close(&fil);
            return err;
        }
        if (br != extended_size) {
            f_close(&fil);
            return FLASHCART_ERR_LOAD;
        }
    }

    if (f_close(&fil) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    return FLASHCART_OK;
}

static flashcart_err_t sc64_load_file (char *file_path, uint32_t rom_offset, uint32_t file_offset) {
    FIL fil;
    UINT br;

    if (f_open(&fil, strip_sd_prefix(file_path), FA_READ) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    fix_file_size(&fil);

    size_t file_size = f_size(&fil) - file_offset;

    if (file_size > (MiB(64) - rom_offset)) {
        f_close(&fil);
        return FLASHCART_ERR_ARGS;
    }

    if (f_lseek(&fil, file_offset) != FR_OK) {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }

    if (f_read(&fil, (void *) (ROM_ADDRESS + rom_offset), file_size, &br) != FR_OK) {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }
    if (br != file_size) {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }

    if (f_close(&fil) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    return FLASHCART_OK;
}

static flashcart_err_t sc64_load_save (char *save_path) {
    void *address = NULL;
    uint32_t value;

    if (sc64_ll_get_config(CFG_ID_SAVE_TYPE, &value) != SC64_OK) {
        return FLASHCART_ERR_INT;
    }

    sc64_save_type_t type = (sc64_save_type_t) (value);

    switch (type) {
        case SAVE_TYPE_EEPROM_4K:
        case SAVE_TYPE_EEPROM_16K:
            address = (void *) (EEPROM_ADDRESS);
            break;
        case SAVE_TYPE_SRAM:
        case SAVE_TYPE_FLASHRAM:
        case SAVE_TYPE_SRAM_BANKED:
            address = (void *) (SRAM_FLASHRAM_ADDRESS);
            break;
        case SAVE_TYPE_NONE:
        default:
            return FLASHCART_ERR_ARGS;
    }

    FIL fil;
    UINT br;

    if (f_open(&fil, strip_sd_prefix(save_path), FA_READ) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    size_t save_size = f_size(&fil);

    if (f_read(&fil, address, save_size, &br) != FR_OK) {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }

    if (f_close(&fil) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    if (br != save_size) {
        return FLASHCART_ERR_LOAD;
    }

    return FLASHCART_OK;
}

static flashcart_err_t sc64_load_64dd_ipl (char *ipl_path, flashcart_progress_callback_t *progress) {
    FIL fil;
    UINT br;

    if (f_open(&fil, strip_sd_prefix(ipl_path), FA_READ) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    fix_file_size(&fil);

    size_t ipl_size = f_size(&fil);

    if (ipl_size > MiB(4)) {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }

    size_t chunk_size = KiB(256);
    for (int offset = 0; offset < ipl_size; offset += chunk_size) {
        size_t block_size = MIN(ipl_size - offset, chunk_size);
        if (f_read(&fil, (void *) (IPL_ADDRESS + offset), block_size, &br) != FR_OK) {
            f_close(&fil);
            return FLASHCART_ERR_LOAD;
        }
        if (progress) {
            progress(f_tell(&fil) / (float) (f_size(&fil)));
        }
    }
    if (f_tell(&fil) != ipl_size) {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }

    if (f_close(&fil) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    return FLASHCART_OK;
}

static flashcart_err_t sc64_load_64dd_disk (char *disk_path, flashcart_disk_parameters_t *disk_parameters) {
    sc64_disk_mapping_t mapping = { .count = 0 };
    uint32_t next_mapping_offset = DISK_MAPPING_ROM_OFFSET;

    // TODO: Support loading multiple disks
    // LOOP START
        mapping.disks[mapping.count].thb_table = next_mapping_offset;
        mapping.disks[mapping.count].sector_table = disk_load_thb_table(mapping.disks[mapping.count].thb_table, disk_parameters);
        next_mapping_offset = disk_sectors_start(mapping.disks[mapping.count].sector_table);
        if (file_get_sectors(disk_path, disk_sectors_callback)) {
            return FLASHCART_ERR_LOAD;
        }
        mapping.count += 1;
    // LOOP END

    if (mapping.count == 0) {
        return FLASHCART_ERR_ARGS;
    }

    if (sc64_ll_set_disk_mapping(&mapping) != SC64_OK) {
        return FLASHCART_ERR_INT;
    }

    sc64_drive_type_t drive_type = disk_parameters->development_drive ? DRIVE_TYPE_DEVELOPMENT : DRIVE_TYPE_RETAIL;

    const struct {
        sc64_cfg_id_t id;
        uint32_t value;
    } config[] = {
        { CFG_ID_DD_MODE, DD_MODE_FULL },
        { CFG_ID_DD_SD_ENABLE, true },
        { CFG_ID_DD_DRIVE_TYPE, drive_type },
        { CFG_ID_DD_DISK_STATE, DISK_STATE_INSERTED },
        { CFG_ID_BUTTON_MODE, BUTTON_MODE_DD_DISK_SWAP },
    };

    for (int i = 0; i < sizeof(config) / sizeof(config[0]); i++) {
        if (sc64_ll_set_config(config[i].id, config[i].value) != SC64_OK) {
            return FLASHCART_ERR_INT;
        }
    }

    return FLASHCART_OK;
}

static flashcart_err_t sc64_set_save_type (flashcart_save_type_t save_type) {
    sc64_save_type_t type;

    switch (save_type) {
        case FLASHCART_SAVE_TYPE_NONE:
            type = SAVE_TYPE_NONE;
            break;
        case FLASHCART_SAVE_TYPE_EEPROM_4K:
            type = SAVE_TYPE_EEPROM_4K;
            break;
        case FLASHCART_SAVE_TYPE_EEPROM_16K:
            type = SAVE_TYPE_EEPROM_16K;
            break;
        case FLASHCART_SAVE_TYPE_SRAM:
            type = SAVE_TYPE_SRAM;
            break;
        case FLASHCART_SAVE_TYPE_SRAM_BANKED:
            type = SAVE_TYPE_SRAM_BANKED;
            break;
        case FLASHCART_SAVE_TYPE_SRAM_128K:
            type = SAVE_TYPE_SRAM_128K;
            break;
        case FLASHCART_SAVE_TYPE_FLASHRAM:
            type = SAVE_TYPE_FLASHRAM;
            break;
        case FLASHCART_SAVE_TYPE_FLASHRAM_PKST2:
            type = SAVE_TYPE_FLASHRAM;
            break;
        default:
            return FLASHCART_ERR_ARGS;
    }

    if (sc64_ll_set_config(CFG_ID_SAVE_TYPE, type) != SC64_OK) {
        return FLASHCART_ERR_INT;
    }

    return FLASHCART_OK;
}

static flashcart_err_t sc64_set_save_writeback (uint32_t *sectors) {
    pi_dma_write_data(sectors, SC64_BUFFERS->BUFFER, 1024);

    if (sc64_ll_writeback_enable(SC64_BUFFERS->BUFFER) != SC64_OK) {
        return FLASHCART_ERR_INT;
    }

    return FLASHCART_OK;
}


static flashcart_t flashcart_sc64 = {
    .init = sc64_init,
    .deinit = sc64_deinit,
    .has_feature = sc64_has_feature,
    .load_rom = sc64_load_rom,
    .load_file = sc64_load_file,
    .load_save = sc64_load_save,
    .load_64dd_ipl = sc64_load_64dd_ipl,
    .load_64dd_disk = sc64_load_64dd_disk,
    .set_save_type = sc64_set_save_type,
    .set_save_writeback = sc64_set_save_writeback,
};


flashcart_t *sc64_get_flashcart (void) {
    return &flashcart_sc64;
}
