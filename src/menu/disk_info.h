/**
 * @file disk_info.h
 * @brief 64DD disk information
 * @ingroup menu 
 */

#ifndef DISK_INFO_H__
#define DISK_INFO_H__


#include <stdbool.h>
#include <stdint.h>

#include "path.h"


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

/** @brief Disk type enumeration.
 *
 * The disk type is used to identify the type of 64DD disk.
 * The values correspond to the disk types defined in the 64DD specification.
 * There are 7 disk types, 0 - 6, corresponding to the relative sizes of the RAM area and the ROM area on the disk.
 * The disk type of a particular disk is determined by the size of the writable area needed by the programmer.
 */
typedef enum {
    DISK_TYPE_0,    /**< ROM: Zone 0-2, RAM: Zone 3-8 */
    DISK_TYPE_1,    /**< ROM: Zone 0-3, RAM: Zone 4-8 */
    DISK_TYPE_2,    /**< ROM: Zone 0-4, RAM: Zone 5-8 */
    DISK_TYPE_3,    /**< ROM: Zone 0-5, RAM: Zone 6-8 */
    DISK_TYPE_4,    /**< ROM: Zone 0-6, RAM: Zone 7-8 */
    DISK_TYPE_5,    /**< ROM: Zone 0-7, RAM: Zone 8 */
    DISK_TYPE_6,    /**< ROM: Zone 0-8, RAM: None */
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


/**
 * @brief Loads disk information from the specified path.
 *
 * This function reads the disk information from the given path and populates
 * the provided disk_info structure with the relevant data.
 *
 * @param path A pointer to a path_t structure that specifies the path to the disk.
 * @param disk_info A pointer to a disk_info_t structure where the disk information will be stored.
 * @return A disk_err_t value indicating the success or failure of the operation.
 */
disk_err_t disk_info_load (path_t *path, disk_info_t *disk_info);


#endif /* DISK_INFO_H__ */
