#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <fatfs/ff.h>
#include <libdragon.h>

#include "utils/fs.h"
#include "utils/utils.h"

#include "../flashcart_utils.h"
#include "ed64_ll.h"
#include "ed64.h"


extern int ed_exit(void);

static flashcart_err_t ed64_init (void) {
    if (file_exists(strip_sd_prefix("/menu/RESET"))) {
    f_unlink(strip_sd_prefix("/menu/RESET"));
    
    FIL fil, lsfil, savfil;
    UINT br, lsbr, savbr;
    // Older everdrives cant save during gameplay so we need to the reset method.

    // find the path to last save
        static char path[50];
        f_open(&lsfil, strip_sd_prefix("/menu/LASTROM"), FA_READ);
        f_lseek(&lsfil , SEEK_END);
        int path_size = f_tell(&lsfil);
        f_lseek((&lsfil), 0);
        f_read(&lsfil, (void *)path, path_size, &lsbr);
        f_close(&lsfil);
    // find the size of the save file
        char text_size[50];
        f_open(&savfil, strip_sd_prefix("/menu/LASTSAVE"), FA_READ);
        f_lseek(&savfil , SEEK_END);
        int save_length = f_tell(&savfil);
        f_lseek((&savfil), 0);
        f_read(&savfil, (void *)text_size, save_length, &savbr);
        int save_size = atoi(text_size);
        f_close(&savfil);
    uint8_t cartsave_data[save_size];
    f_open(&fil, strip_sd_prefix(path), FA_WRITE | FA_CREATE_ALWAYS);
        if (save_size == (512 || KiB(2))){
            getEeprom(cartsave_data, save_size);
            f_write(&fil, (void *)cartsave_data, save_size, &br);
        } else if (save_size > KiB(2)){
            getSRAM(cartsave_data, save_size);
            f_write(&fil, (void *)cartsave_data, save_size, &br);
        }
        f_close(&fil);
    }
    return FLASHCART_OK;
}


static flashcart_err_t ed64_deinit (void) {

    // For the moment, just use libCart exit.
    ed_exit();

    return FLASHCART_OK;
}

static bool ed64_has_feature (flashcart_features_t feature) {
    switch (feature) {
        case FLASHCART_FEATURE_64DD: return false;
        default: return false;
    }
}

static flashcart_err_t ed64_load_rom (char *rom_path, flashcart_progress_callback_t *progress) {
    FIL fil;
    UINT br;

    if (f_open(&fil, strip_sd_prefix(rom_path), FA_READ) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    // FIXME: set the required actions for retriving the save file later (probably SRAM).
    // This would involve creating some content in an area of RAM that would include
    // the ROM location and its save type. This information will be used on init to perform a "save writeback".

    fix_file_size(&fil);

    size_t rom_size = f_size(&fil);

    // FIXME: if the cart is not V3 or X5 or X7, we need probably need to - 128KiB for save compatibility.
    // Or somehow warn that certain ROM's will have corruption due to the address space being used for saves.
    if (rom_size > MiB(64)) {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }

    size_t sdram_size = rom_size;

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


    if (f_close(&fil) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    return FLASHCART_OK;
}

static flashcart_err_t ed64_load_file (char *file_path, uint32_t rom_offset, uint32_t file_offset) {
    FIL fil;
    UINT br;

    if (f_open(&fil, strip_sd_prefix(file_path), FA_READ) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    fix_file_size(&fil);

    size_t file_size = f_size(&fil) - file_offset;

    // FIXME: if the cart is not V3 or X5 or X7, we need probably need to - 128KiB for save compatibility.
    // Or somehow warn that certain ROM's will have corruption due to the address space being used for saves.
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

static flashcart_err_t ed64_load_save (char *save_path) {
    ed64_save_type_t type = ed64_ll_get_save_type();
    ed64_save_transfer_mode mode;
    switch (type) {
        case SAVE_TYPE_EEPROM_4K:
        case SAVE_TYPE_EEPROM_16K:
            mode = EEPROM_MODE;
            break;
        case SAVE_TYPE_SRAM:
        case SAVE_TYPE_SRAM_128K:
        case SAVE_TYPE_FLASHRAM:
            mode = SRAM_MODE;
            break;
        case SAVE_TYPE_NONE:
            mode = MEMPAK_MODE;
        default:
            return FLASHCART_ERR_ARGS;
    }

    if (mode == SRAM_MODE) {
    FIL fil, srmfil;
    UINT br, srmbr;
    size_t save_size = f_size(&fil);
    uint8_t cartsave_data[save_size];
    if (f_open(&fil, strip_sd_prefix(save_path), FA_READ) == FR_OK) {

    if (f_read(&fil, cartsave_data, save_size, &br) != FR_OK) {
            f_close(&fil);
            return FLASHCART_ERR_LOAD;
    }

    if (f_close(&fil) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    setSRAM(cartsave_data, save_size);

    char sram_buffer[50];
    f_open(&srmfil, strip_sd_prefix("/menu/LASTSAVE"), FA_WRITE | FA_CREATE_ALWAYS);
    sprintf(sram_buffer, "%d", save_size);
    f_write(&srmfil, (void *)sram_buffer, f_size(&srmfil), &srmbr);
    f_close(&srmfil);
    }
} else if (mode == EEPROM_MODE){
    FIL epfil, eepfil;
    UINT epbr, eepbr;

    if (f_open(&epfil, strip_sd_prefix(save_path), FA_READ) == FR_OK) {

    size_t save_size = f_size(&epfil);
    uint8_t cartsave_data[save_size];

    if (f_read(&epfil, cartsave_data, save_size, &epbr) != FR_OK) {
            f_close(&epfil);
            return FLASHCART_ERR_LOAD;
    }

    if (f_close(&epfil) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    setEeprom(cartsave_data, save_size);
    char eeprom_buffer[50];
    f_open(&eepfil, strip_sd_prefix("/menu/LASTSAVE"), FA_WRITE | FA_CREATE_ALWAYS);
    sprintf(eeprom_buffer, "%d", save_size);
    f_write(&eepfil, (void *)eeprom_buffer, f_size(&eepfil), &eepbr);
    f_close(&eepfil);
    }
}
    FIL rsfil, lrfil;
    UINT rsbr, lrbr;
    TCHAR reset_byte[1];
    f_open(&rsfil, strip_sd_prefix("/menu/RESET"), FA_WRITE | FA_CREATE_ALWAYS);
    f_write(&rsfil, (void *)reset_byte, 1, &rsbr);
    f_close(&rsfil);
    
    
    char tmp[50];
    f_open(&lrfil, strip_sd_prefix("/menu/LASTROM"), FA_WRITE | FA_CREATE_ALWAYS);
    strcpy(tmp, strip_sd_prefix(save_path));
    f_write(&lrfil, (void *)tmp, f_size(&lrfil), &lrbr);
    f_close(&lrfil);

    return FLASHCART_OK;
}

static flashcart_err_t ed64_set_save_type (flashcart_save_type_t save_type) {
    ed64_save_type_t type;

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
        case FLASHCART_SAVE_TYPE_SRAM_128K:
            type = SAVE_TYPE_SRAM_128K;
            break;
        case FLASHCART_SAVE_TYPE_FLASHRAM_PKST2:
        case FLASHCART_SAVE_TYPE_FLASHRAM:
            type = SAVE_TYPE_FLASHRAM;
            break;
        default:
            return FLASHCART_ERR_ARGS;
    }

    ed64_ll_set_save_type(type);

    return FLASHCART_OK;
}


static flashcart_t flashcart_ed64 = {
    .init = ed64_init,
    .deinit = ed64_deinit,
    .has_feature = ed64_has_feature,
    .load_rom = ed64_load_rom,
    .load_file = ed64_load_file,
    .load_save = ed64_load_save,
    .load_64dd_ipl = NULL,
    .load_64dd_disk = NULL,
    .set_save_type = ed64_set_save_type,
    .set_save_writeback = NULL,
};


flashcart_t *ed64_get_flashcart (void) {
    return &flashcart_ed64;
}
