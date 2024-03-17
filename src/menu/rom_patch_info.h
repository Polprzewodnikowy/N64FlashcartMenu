/**
 * @file rom_patcher.h
 * @brief N64 ROM patcher
 * @ingroup menu 
 */

#ifndef ROM_PATCH_INFO_H__
#define ROM_PATCH_INFO_H__


#include <stdbool.h>
#include <stdint.h>

#include "path.h"

/** @brief Patch state enumeration. */
typedef enum {
    PATCH_OK,
    PATCH_ERR_IO,
    PATCH_ERR_NO_FILE,
    PATCH_ERR_INVALID,
    PATCH_ERR_UNSUPPORTED,
} rom_patch_load_err_t;

/** @brief Patch type enumeration. */
typedef enum {
    PATCH_TYPE_BPS,
    PATCH_TYPE_APS,
    PATCH_TYPE_IPS,
    PATCH_TYPE_UPS,
    PATCH_TYPE_XDELTA,
} rom_patch_type_t;


#define PATCH_BPS_MAGIC "BPS1"
#define PATCH_APS_MAGIC_GBA "APS1"
#define PATCH_APS_MAGIC_N64 "APS10"
#define PATCH_IPS_MAGIC "PATCH"
#define PATCH_UPS_MAGIC "UPS1"


/** @brief APS patch type enumeration. */
typedef enum {
    /** @brief Is a simple patch. */
    APS_PATCH_TYPE_SIMPLE,
    /** @brief Is an N64 specific patch. */
    APS_PATCH_TYPE_N64_SPECIFIC,
} aps_patch_type_t;

/** @brief APS patch type enumeration. */
typedef enum {
    /** @brief Is a simple patch encoding. */
    PATCH_ENCODING_SIMPLE,
} aps_patch_encoding_t;


/** @brief ROM Patch Information Structure. */
typedef struct {
    rom_patch_type_t patch_type;
    // patch description

} rom_patch_info_t;

//rom_patch_load_err_t rom_patch_info_load (path_t *path, rom_patch_info_t *rom_patch_info);
rom_patch_load_err_t rom_patch_info_load_file (char *path);

#endif
