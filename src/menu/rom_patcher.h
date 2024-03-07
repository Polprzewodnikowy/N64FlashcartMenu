/**
 * @file patcher.h
 * @brief N64 ROM patcher
 * @ingroup menu 
 */

#ifndef ROM_PATCHER_H__
#define ROM_PATCHER_H__


#include <stdbool.h>
#include <stdint.h>

/** @brief Patch state enumeration. */
typedef enum {
    PATCH_OK,
    PATCH_ERR_IO,
    PATCH_ERR_NO_FILE,
    PATCH_ERR_INVALID,
    PATCH_ERR_UNSUPPORTED,
} rom_patcher_err_t;

rom_patcher_err_t rom_patcher_load_file (char *path);

#endif
