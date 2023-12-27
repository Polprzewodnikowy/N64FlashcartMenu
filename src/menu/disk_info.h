/**
 * @file disk_info.h
 * @brief 64DD disk information
 * @ingroup menu 
 */

#ifndef DISK_INFO_H__
#define DISK_INFO_H__


#include <stdbool.h>
#include <stdint.h>

/** @brief Disk state enumeration. */
typedef enum {
    DISK_OK,
    DISK_ERR_IO,
    DISK_ERR_NO_FILE,
    DISK_ERR_INVALID,
} disk_err_t;

/** @brief Disk region enumeration. */
typedef enum {
    DISK_REGION_DEVELOPMENT,
    DISK_REGION_JAPANESE,
    DISK_REGION_USA,
} disk_region_t;

/** @brief Disk type enumeration. */
typedef enum {
    DISK_TYPE_0,
    DISK_TYPE_1,
    DISK_TYPE_2,
    DISK_TYPE_3,
    DISK_TYPE_4,
    DISK_TYPE_5,
    DISK_TYPE_6,
} disk_type_t;

/** @brief Disk Information Structure. */
typedef struct {
    disk_region_t region;
    disk_type_t disk_type;
    char id[4];
    uint8_t version;

    bool bad_system_area_lbas[24];
    uint8_t defect_tracks[16][12];
} disk_info_t;


disk_err_t disk_info_load (char *path, disk_info_t *disk_info);


#endif
