/**
 * @file disk_info.c
 * @brief Disk Information component implementation
 * @ingroup menu
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
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

/**
 * @brief Load a system area LBA from the disk.
 * 
 * @param f File pointer to the disk image.
 * @param lba Logical block address to load.
 * @param buffer Buffer to store the loaded data.
 * @return true if an error occurred, false otherwise.
 */
static bool load_system_area_lba (FILE *f, int lba, uint8_t *buffer) {
    if (lba >= SYSTEM_AREA_LBA_COUNT) {
        return true;
    }
    if (fseek(f, SYSTEM_AREA_LBA_LENGTH * lba, SEEK_SET)) {
        return true;
    }
    if (fread(buffer, SYSTEM_AREA_LBA_LENGTH, 1, f) != 1) {
        return true;
    }
    return false;
}

/**
 * @brief Verify the integrity of a system area LBA.
 * 
 * @param buffer Buffer containing the LBA data.
 * @param sector_length Length of each sector in the LBA.
 * @return true if the LBA is valid, false otherwise.
 */
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

/**
 * @brief Verify the integrity of a system data LBA.
 * 
 * @param buffer Buffer containing the LBA data.
 * @return true if the LBA is valid, false otherwise.
 */
static bool verify_system_data_lba (uint8_t *buffer) {
    return (
        (buffer[4] == 0x10) &&
        ((buffer[5] & 0xF0) == 0x10) &&
        ((buffer[5] & 0x0F) <= DISK_TYPE_6) &&
        (GET_U32(&buffer[24]) == 0xFFFFFFFF)
    );
}

/**
 * @brief Set the defect tracks for the disk.
 * 
 * @param buffer Buffer containing the defect track data.
 * @param disk_info Pointer to the disk information structure.
 * @return true if the defect tracks were set successfully, false otherwise.
 */
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

/**
 * @brief Update the bad system area LBAs for the disk.
 * 
 * @param disk_info Pointer to the disk information structure.
 */
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

/**
 * @brief Load and verify the system data LBA from the disk.
 * 
 * @param f File pointer to the disk image.
 * @param disk_info Pointer to the disk information structure.
 * @return disk_err_t Error code.
 */
static disk_err_t load_and_verify_system_data_lba (FILE *f, disk_info_t *disk_info) {
    uint8_t buffer[SYSTEM_AREA_LBA_LENGTH];
    int sector_length;

    bool valid_system_data_lba_found = false;

    for (int i = 0; i < SYSTEM_DATA_LBA_COUNT; i++) {
        int lba = system_data_lbas[i];

        if (load_system_area_lba(f, lba, buffer)) {
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

/**
 * @brief Load and verify the disk ID LBA from the disk.
 * 
 * @param f File pointer to the disk image.
 * @param disk_info Pointer to the disk information structure.
 * @return disk_err_t Error code.
 */
static disk_err_t load_and_verify_disk_id_lba (FILE *f, disk_info_t *disk_info) {
    uint8_t buffer[SYSTEM_AREA_LBA_LENGTH];

    bool valid_disk_id_lba_found = false;

    for (int i = 0; i < DISK_ID_LBA_COUNT; i++) {
        int lba = disk_id_lbas[i];

        if (load_system_area_lba(f, lba, buffer)) {
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

/**
 * @brief Load the disk information from the specified path.
 * 
 * @param path Pointer to the path structure.
 * @param disk_info Pointer to the disk information structure.
 * @return disk_err_t Error code.
 */
disk_err_t disk_info_load (path_t *path, disk_info_t *disk_info) {
    FILE *f;
    disk_err_t err;

    for (int i = 0; i < SYSTEM_AREA_LBA_COUNT; i++) {
        disk_info->bad_system_area_lbas[i] = false;
    }

    if ((f = fopen(path_get(path), "rb")) == NULL) {
        return DISK_ERR_NO_FILE;
    }
    setbuf(f, NULL);
    if ((err = load_and_verify_system_data_lba(f, disk_info)) != DISK_OK) {
        fclose(f);
        return err;
    }
    if ((err = load_and_verify_disk_id_lba(f, disk_info)) != DISK_OK) {
        fclose(f);
        return err;
    }
    if (fclose(f)) {
        return DISK_ERR_IO;
    }

    update_bad_system_area_lbas(disk_info);

    return DISK_OK;
}
