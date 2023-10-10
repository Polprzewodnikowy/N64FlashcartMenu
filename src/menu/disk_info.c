#include <fatfs/ff.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "disk_info.h"
#include "utils/fs.h"


#define SECTORS_PER_BLOCK                   (85)
#define DISK_ZONES                          (16)
#define DISK_BAD_TRACKS_PER_ZONE            (12)

#define SYSTEM_AREA_SECTOR_LENGTH           (232)
#define SYSTEM_AREA_LBA_LENGTH              (SYSTEM_AREA_SECTOR_LENGTH * SECTORS_PER_BLOCK)
#define SYSTEM_AREA_LBA_COUNT               (24)

#define SYSTEM_DATA_LBA_COUNT               (8)
#define DISK_ID_LBA_COUNT                   (2)

#define DEV_SYSTEM_DATA_SECTOR_LENGTH       (192)
#define RETAIL_SYSTEM_DATA_SECTOR_LENGTH    (232)

#define REGION_ID_DEVELOPMENT               (0x00000000)
#define REGION_ID_JAPANESE                  (0xE848D316)
#define REGION_ID_USA                       (0x2263EE56)

#define GET_U32(b)                          (((b)[0] << 24) | ((b)[1] << 16) | ((b)[2] << 8) | (b)[3])


static const int tracks_per_zone[DISK_ZONES] = {
    158, 158, 149, 149, 149, 149, 149, 114, 158, 158, 149, 149, 149, 149, 149, 114
};
static const int system_data_lbas[SYSTEM_DATA_LBA_COUNT] = {
    9, 8, 1, 0, 11, 10, 3, 2
};
static const int disk_id_lbas[DISK_ID_LBA_COUNT] = {
    15, 14
};


static bool load_system_area_lba (FIL *fil, int lba, uint8_t *buffer) {
    UINT bytes_read;
    if (lba >= SYSTEM_AREA_LBA_COUNT) {
        return true;
    }
    if (f_lseek(fil, SYSTEM_AREA_LBA_LENGTH * lba) != FR_OK) {
        return true;
    }
    if (f_read(fil, buffer, SYSTEM_AREA_LBA_LENGTH, &bytes_read) != FR_OK) {
        return true;
    }
    if (bytes_read != SYSTEM_AREA_LBA_LENGTH) {
        return true;
    }
    return false;
}

static bool verify_system_area_lba (uint8_t *buffer, int sector_length) {
    for (int sector = 1; sector < SECTORS_PER_BLOCK; sector++) {
        for (int i = 0; i < sector_length; i++) {
            if (buffer[i] != buffer[(sector * sector_length) + i]) {
                return false;
            }
        }
    }
    return true;
}

static bool verify_system_data_lba (uint8_t *buffer) {
    return (
        (buffer[4] == 0x10) &&
        ((buffer[5] & 0xF0) == 0x10) &&
        ((buffer[5] & 0x0F) <= DISK_TYPE_6) &&
        (GET_U32(&buffer[24]) == 0xFFFFFFFF)
    );
}

static bool set_defect_tracks (uint8_t *buffer, disk_info_t *disk_info) {
    for (int head_zone = 0; head_zone < DISK_ZONES; head_zone++) {
        uint8_t start = ((head_zone == 0) ? 0 : buffer[7 + head_zone]);
        uint8_t end = buffer[7 + head_zone + 1];
        if ((end - start) > DISK_BAD_TRACKS_PER_ZONE) {
            return false;
        }
        int track = 0;
        for (int offset = start; offset < end; offset++) {
            disk_info->defect_tracks[head_zone][track++] = buffer[0x20 + offset];
        }
        for (int i = 0; track < DISK_BAD_TRACKS_PER_ZONE; i++) {
            disk_info->defect_tracks[head_zone][track++] = tracks_per_zone[head_zone] - i - 1;
        }
    }
    return true;
}

static void update_bad_system_area_lbas (disk_info_t *disk_info) {
    if (disk_info->region == DISK_REGION_DEVELOPMENT) {
        disk_info->bad_system_area_lbas[0] = true;
        disk_info->bad_system_area_lbas[1] = true;
        disk_info->bad_system_area_lbas[8] = true;
        disk_info->bad_system_area_lbas[9] = true;
    } else {
        disk_info->bad_system_area_lbas[2] = true;
        disk_info->bad_system_area_lbas[3] = true;
        disk_info->bad_system_area_lbas[10] = true;
        disk_info->bad_system_area_lbas[11] = true;
        disk_info->bad_system_area_lbas[12] = true;
    }

    for (int lba = 16; lba < SYSTEM_AREA_LBA_COUNT; lba++) {
        disk_info->bad_system_area_lbas[lba] = true;
    }
}

static disk_err_t load_and_verify_system_data_lba (FIL *fil, disk_info_t *disk_info) {
    uint8_t buffer[SYSTEM_AREA_LBA_LENGTH];
    int sector_length;

    bool valid_system_data_lba_found = false;

    for (int i = 0; i < SYSTEM_DATA_LBA_COUNT; i++) {
        int lba = system_data_lbas[i];

        if (load_system_area_lba(fil, lba, buffer)) {
            return DISK_ERR_IO;
        }

        switch (GET_U32(&buffer[0])) {
            case REGION_ID_DEVELOPMENT:
                disk_info->region = DISK_REGION_DEVELOPMENT;
                sector_length = DEV_SYSTEM_DATA_SECTOR_LENGTH;
                break;
            case REGION_ID_JAPANESE:
                disk_info->region = DISK_REGION_JAPANESE;
                sector_length = RETAIL_SYSTEM_DATA_SECTOR_LENGTH;
                break;
            case REGION_ID_USA:
                disk_info->region = DISK_REGION_USA;
                sector_length = RETAIL_SYSTEM_DATA_SECTOR_LENGTH;
                break;
            default:
                disk_info->bad_system_area_lbas[lba] = true;
                continue;
        }

        if (verify_system_area_lba(buffer, sector_length) && verify_system_data_lba(buffer) && set_defect_tracks(buffer, disk_info)) {
            valid_system_data_lba_found = true;
            disk_info->disk_type = (buffer[5] & 0x0F);
        } else {
            disk_info->bad_system_area_lbas[lba] = true;
        }
    }

    return valid_system_data_lba_found ? DISK_OK : DISK_ERR_INVALID;
}

static disk_err_t load_and_verify_disk_id_lba (FIL *fil, disk_info_t *disk_info) {
    uint8_t buffer[SYSTEM_AREA_LBA_LENGTH];

    bool valid_disk_id_lba_found = false;

    for (int i = 0; i < DISK_ID_LBA_COUNT; i++) {
        int lba = disk_id_lbas[i];

        if (load_system_area_lba(fil, lba, buffer)) {
            return DISK_ERR_IO;
        }

        if (verify_system_area_lba(buffer, SYSTEM_AREA_SECTOR_LENGTH)) {
            valid_disk_id_lba_found = true;
            memcpy(disk_info->id, &buffer[0], sizeof(disk_info->id));
            disk_info->version = buffer[4];
        } else {
            disk_info->bad_system_area_lbas[lba] = true;
        }
    }

    return valid_disk_id_lba_found ? DISK_OK : DISK_ERR_INVALID;
}


disk_err_t disk_info_load (char *path, disk_info_t *disk_info) {
    FIL fil;
    disk_err_t err;

    for (int i = 0; i < SYSTEM_AREA_LBA_COUNT; i++) {
        disk_info->bad_system_area_lbas[i] = false;
    }

    if (f_open(&fil, strip_sd_prefix(path), FA_READ) != FR_OK) {
        return DISK_ERR_NO_FILE;
    }

    if ((err = load_and_verify_system_data_lba(&fil, disk_info)) != DISK_OK) {
        f_close(&fil);
        return err;
    }

    if ((err = load_and_verify_disk_id_lba(&fil, disk_info)) != DISK_OK) {
        f_close(&fil);
        return err;
    }

    if (f_close(&fil) != FR_OK) {
        return DISK_ERR_IO;
    }

    update_bad_system_area_lbas(disk_info);

    return DISK_OK;
}
