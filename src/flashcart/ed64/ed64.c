#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <fatfs/ff.h>
#include <libdragon.h>
#include <libcart/cart.h>

#include "utils/fs.h"
#include "utils/utils.h"

#include "../flashcart_utils.h"
#include "ed64_ll.h"
#include "ed64.h"

// #include "../menu/settings.h"

// This is a trial hack before using the settings API.
#ifndef LAST_SAVE_FILE_PATH
#define LAST_SAVE_FILE_PATH "/menu/last_rom.txt"
#endif
#ifndef RESET_CHECK_FILE_PATH
#define RESET_CHECK_FILE_PATH "/menu/RESET"
#endif
#ifndef FLASHRAM_CHECK_FILE_PATH
#define FLASHRAM_CHECK_FILE_PATH "/menu/FLASHRAM"
#endif

extern int ed_exit(void);

static flashcart_err_t ed64_init(void)
{
    // TODO: partly already done, see https://github.com/DragonMinded/libdragon/blob/4ec469d26b6dc4e308caf3d5b86c2b340b708bbd/src/libcart/cart.c#L1064

    // FIXME: Update firmware if needed.
    // FIXME: Enable RTC if available.

    // older everdrives cant save during gameplay so we need to the reset method.
    // works by checking if a file exists.

    if (file_exists(strip_sd_prefix(RESET_CHECK_FILE_PATH)))
    {

        // make sure next boot doesnt trigger the check by deleting the reset file
        f_unlink(strip_sd_prefix(RESET_CHECK_FILE_PATH));

        // finds the last save location
        FIL lrp_fil;
        UINT lrp_br;

        if (f_open(&lrp_fil, strip_sd_prefix(LAST_SAVE_FILE_PATH), FA_READ) != FR_OK)
        {
            return FLASHCART_ERR_LOAD;
        }

        int lrp_size = f_size(&lrp_fil);

        TCHAR lrp_path[lrp_size++];

        if (f_read(&lrp_fil, lrp_path, lrp_size, &lrp_br) != FR_OK)
        {
            f_close(&lrp_fil);
            return FLASHCART_ERR_LOAD;
        }

        if (f_close(&lrp_fil) != FR_OK)
        {
            return FLASHCART_ERR_LOAD;
        }

        // Now save the content back to the SD!
        FIL fil;
        UINT br;
        uint8_t cartsave_data[KiB(128)];

        // find the path to last save
        if (file_exists(strip_sd_prefix(lrp_path)))
        {

            int save_size = file_get_size(strip_sd_prefix(lrp_path));

            if ((f_open(&fil, strip_sd_prefix(lrp_path), FA_CREATE_ALWAYS | FA_READ | FA_WRITE)) != FR_OK)
            {
                return FLASHCART_ERR_LOAD;
            }

            // everdrive doesn't care about the save type other than flash sram and eeprom
            // so minus flashram we can just check the size
            if (file_exists(strip_sd_prefix(FLASHRAM_CHECK_FILE_PATH)))
            { // flashram is bugged atm
                getFlashRAM(cartsave_data, save_size);
              // deletes flag
              f_unlink(strip_sd_prefix(FLASHRAM_CHECK_FILE_PATH));
            }
            else if (save_size > KiB(2))
            { // sram
                getSRAM(cartsave_data, save_size);
            }
            else
            { // eeprom
                getEeprom(cartsave_data, save_size);
            }

            if (f_write(&fil, cartsave_data, save_size, &br) != FR_OK)
            {
                return FLASHCART_ERR_LOAD;
            }

            if (f_close(&fil) != FR_OK)
            {
                return FLASHCART_ERR_LOAD;
            }
        }
    }
    return FLASHCART_OK;
}

static flashcart_err_t ed64_deinit(void)
{

    // For the moment, just use libCart exit.
    ed_exit();

    return FLASHCART_OK;
}

static bool ed64_has_feature(flashcart_features_t feature)
{
    switch (feature)
    {
    case FLASHCART_FEATURE_64DD:
        return false;
    default:
        return false;
    }
}

static flashcart_err_t ed64_load_rom(char *rom_path, flashcart_progress_callback_t *progress)
{
    FIL fil;
    UINT br;

    if (f_open(&fil, strip_sd_prefix(rom_path), FA_READ) != FR_OK)
    {
        return FLASHCART_ERR_LOAD;
    }

    fix_file_size(&fil);

    size_t rom_size = f_size(&fil);

    // FIXME: if the cart is not V3 or X5 or X7, we need probably need to - 128KiB for save compatibility.
    // Or somehow warn that certain ROM's will have corruption due to the address space being used for saves.
    // Conker's Bad Fur Day doesn't have this issue because eeprom data is at a fixed address in pif ram.
    if (rom_size > MiB(64))
    {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }

    if (rom_size == MiB(64))
    {
        rom_size -= KiB(128);
    }
    
    size_t sdram_size = rom_size;

    size_t chunk_size = MiB(1);
    for (int offset = 0; offset < sdram_size; offset += chunk_size)
    {
        size_t block_size = MIN(sdram_size - offset, chunk_size);
        if (f_read(&fil, (void *)(ROM_ADDRESS + offset), block_size, &br) != FR_OK)
        {
            f_close(&fil);
            return FLASHCART_ERR_LOAD;
        }
        if (progress)
        {
            progress(f_tell(&fil) / (float)(f_size(&fil)));
        }
    }
    /*if (f_tell(&fil) != sdram_size)
    {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }*/

    if (f_close(&fil) != FR_OK)
    {
        return FLASHCART_ERR_LOAD;
    }

    return FLASHCART_OK;
}

static flashcart_err_t ed64_load_file(char *file_path, uint32_t rom_offset, uint32_t file_offset)
{
    FIL fil;
    UINT br;

    if (f_open(&fil, strip_sd_prefix(file_path), FA_READ) != FR_OK)
    {
        return FLASHCART_ERR_LOAD;
    }

    fix_file_size(&fil);

    size_t file_size = f_size(&fil) - file_offset;

    // FIXME: if the cart is not V3 or X5 or X7, we need probably need to - 128KiB for save compatibility.
    // Or somehow warn that certain ROM's will have corruption due to the address space being used for saves.

    if (file_size > (MiB(64) - rom_offset))
    {
        f_close(&fil);
        return FLASHCART_ERR_ARGS;
    }

    if (f_lseek(&fil, file_offset) != FR_OK)
    {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }

    if (f_read(&fil, (void *)(ROM_ADDRESS + rom_offset), file_size, &br) != FR_OK)
    {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }
    if (br != file_size)
    {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }

    if (f_close(&fil) != FR_OK)
    {
        return FLASHCART_ERR_LOAD;
    }

    return FLASHCART_OK;
}

static flashcart_err_t ed64_load_save(char *save_path)
{
    
    FIL fil;
    UINT br;

    if (f_open(&fil, strip_sd_prefix(save_path), FA_READ) != FR_OK)
    {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }

    size_t save_size = file_get_size(strip_sd_prefix(save_path));
    uint8_t cartsave_data[save_size];

    if (f_read(&fil, cartsave_data, save_size, &br) != FR_OK)
    {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }

    if (f_close(&fil) != FR_OK)
    {
        return FLASHCART_ERR_LOAD;
    }

    ed64_save_type_t type = ed64_ll_get_save_type();
    switch (type)
    {
    case SAVE_TYPE_EEPROM_4K:
    case SAVE_TYPE_EEPROM_16K:
        setEeprom(cartsave_data, save_size);
        break;
    case SAVE_TYPE_SRAM:
    case SAVE_TYPE_SRAM_128K:
        setSRAM(cartsave_data, save_size);
        break;
    case SAVE_TYPE_FLASHRAM:
        setFlashRAM(cartsave_data, save_size);
        // a cold and warm boot has no way of seeing save types and most types can be determined by size
        // this tells the cart to use flash instead of sram 128 since they are the same size
        FIL flashfil;
        if (f_open(&flashfil, strip_sd_prefix(FLASHRAM_CHECK_FILE_PATH), FA_CREATE_ALWAYS) != FR_OK)
        {
            f_close(&flashfil);
            return FLASHCART_ERR_LOAD;
        }

        if (f_close(&flashfil) != FR_OK)
        {
            return FLASHCART_OK;
        }
        break;
    default:
       break;
    }


    FIL lsp_fil;
    UINT lsp_bw;

    if (f_open(&lsp_fil, strip_sd_prefix(LAST_SAVE_FILE_PATH), FA_WRITE | FA_CREATE_ALWAYS) != FR_OK)
    {
        return FLASHCART_ERR_LOAD;
    }
    if (f_write(&lsp_fil, strip_sd_prefix(save_path), strlen(save_path), &lsp_bw) != FR_OK)
    {
        f_close(&lsp_fil);
        return FLASHCART_ERR_LOAD;
    }

    if (f_close(&lsp_fil) != FR_OK)
    {
        return FLASHCART_ERR_LOAD;
    }

    FIL rsfil;

    // simulate a unix touch command to create a file as it only needs to exist to detect a reset

    if (f_open(&rsfil, strip_sd_prefix(RESET_CHECK_FILE_PATH), FA_CREATE_ALWAYS) != FR_OK)
    {
        f_close(&rsfil);
        return FLASHCART_ERR_LOAD;
    }

    if (f_close(&rsfil) != FR_OK)
    {
        return FLASHCART_OK;
    }

    return FLASHCART_OK;
}

static flashcart_err_t ed64_set_save_type(flashcart_save_type_t save_type)
{
    ed64_save_type_t type;

    switch (save_type)
    {
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

flashcart_t *ed64_get_flashcart(void)
{
    return &flashcart_ed64;
}
