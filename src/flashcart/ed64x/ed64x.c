#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <fatfs/ff.h>
#include <libdragon.h>

#include "utils/fs.h"
#include "utils/utils.h"

#include "../flashcart_utils.h"
#include "ed64x_ll.h"
#include "ed64x.h"

typedef enum {
    // ED64_V1_0 = 1,
    // ED64_V2_0 = 2,
    // ED64_V2_5 = 2.5,
    // ED64_V3_0 = 3,
    ED64_X5 = 5,
    ED64_X7 = 7,
    ED64_UKNOWN = 0,
} ed64x_device_variant_t;

/* ED64 save location base address  */
#define SRAM_ADDRESS (0xA8000000)
/* ED64 ROM location base address  */
#define ROM_ADDRESS  (0xB0000000)

static flashcart_err_t ed64x_init (void) {

    return FLASHCART_OK;
}

static flashcart_err_t ed64x_deinit (void) {

    return FLASHCART_OK;
}

static ed64x_device_variant_t get_cart_model() {
    ed64x_device_variant_t variant = ED64_X7; // FIXME: check cart model from ll for better feature handling.
    return variant;
}

static bool ed64x_has_feature (flashcart_features_t feature) {
    bool is_model_x7 = (get_cart_model() == ED64_X7); 
    switch (feature) {
        case FLASHCART_FEATURE_RTC: return is_model_x7 ? true : false;
        case FLASHCART_FEATURE_USB: return is_model_x7 ? true : false;
        case FLASHCART_FEATURE_AUTO_CIC: return true;
        case FLASHCART_FEATURE_64DD: return false;
        default: return false;
    }
}

static flashcart_err_t ed64x_load_rom (char *rom_path, flashcart_progress_callback_t *progress) {
    FIL fil;
    UINT br;

    if (f_open(&fil, strip_fs_prefix(rom_path), FA_READ) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    fatfs_fix_file_size(&fil);

    size_t rom_size = f_size(&fil);

    if (rom_size > MiB(64)) {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }

    size_t sdram_size = MiB(64);

    size_t chunk_size = KiB(128);
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
    if (f_tell(&fil) != rom_size) {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }

    if (f_close(&fil) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    return FLASHCART_OK;
}

static flashcart_err_t ed64x_load_file (char *file_path, uint32_t rom_offset, uint32_t file_offset) {
    FIL fil;
    UINT br;

    if (f_open(&fil, strip_fs_prefix(file_path), FA_READ) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    fatfs_fix_file_size(&fil);

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

static flashcart_err_t ed64x_load_save (char *save_path) {


    return FLASHCART_OK;
}

static flashcart_err_t ed64x_set_save_type (flashcart_save_type_t save_type) {
    

    return FLASHCART_OK;
}

static flashcart_t flashcart_ed64x = {
    .init = ed64x_init,
    .deinit = ed64x_deinit,
    .has_feature = ed64x_has_feature,
    .load_rom = ed64x_load_rom,
    .load_file = ed64x_load_file,
    .load_save = ed64x_load_save,
    .load_64dd_ipl = NULL,
    .load_64dd_disk = NULL,
    .set_save_type = ed64x_set_save_type,
    .set_save_writeback = NULL,
};


flashcart_t *ed64x_get_flashcart (void) {
    return &flashcart_ed64x;
}
