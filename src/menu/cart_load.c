#include <string.h>

#include <libdragon.h>

#include "cart_load.h"
#include "path.h"
#include "utils/fs.h"
#include "utils/utils.h"

#ifndef SAVES_SUBDIRECTORY
#define SAVES_SUBDIRECTORY      "saves"
#endif
#ifndef DDIPL_LOCATION
#define DDIPL_LOCATION          "/menu/64ddipl"
#endif
#ifndef EMU_LOCATION
#define EMU_LOCATION            "/menu/emulators"
#endif


static bool is_64dd_connected (void) {
    return (
        ((io_read(0x05000540) & 0x0000FFFF) == 0x0000) ||
        (io_read(0x06001010) == 0x2129FFF8)
    );
}

static bool create_saves_subdirectory (path_t *path) {
    path_t *save_folder_path = path_clone(path);
    path_pop(save_folder_path);
    path_push(save_folder_path, SAVES_SUBDIRECTORY);
    bool error = directory_create(path_get(save_folder_path));
    path_free(save_folder_path);
    return error;
}

static flashcart_save_type_t convert_save_type (rom_info_t *info) {
    switch (info->save_type) {
        case SAVE_TYPE_EEPROM_4K: return FLASHCART_SAVE_TYPE_EEPROM_4K;
        case SAVE_TYPE_EEPROM_16K: return FLASHCART_SAVE_TYPE_EEPROM_16K;
        case SAVE_TYPE_SRAM: return FLASHCART_SAVE_TYPE_SRAM;
        case SAVE_TYPE_SRAM_BANKED: return FLASHCART_SAVE_TYPE_SRAM_BANKED;
        case SAVE_TYPE_SRAM_128K: return FLASHCART_SAVE_TYPE_SRAM_128K;
        case SAVE_TYPE_FLASHRAM: return FLASHCART_SAVE_TYPE_FLASHRAM;
        case SAVE_TYPE_FLASHRAM_PKST2: return FLASHCART_SAVE_TYPE_FLASHRAM_PKST2;
        default: return FLASHCART_SAVE_TYPE_NONE;
    }
}


char *cart_load_convert_error_message (cart_load_err_t err) {
    switch (err) {
        case CART_LOAD_OK: return "Cart load OK";
        case CART_LOAD_ERR_ROM_LOAD_FAIL: return "Error occured during ROM loading";
        case CART_LOAD_ERR_SAVE_LOAD_FAIL: return "Error occured during save loading";
        case CART_LOAD_ERR_64DD_PRESENT: return "64DD accessory is connected to the N64";
        case CART_LOAD_ERR_64DD_IPL_NOT_FOUND: return "Required 64DD IPL file was not found";
        case CART_LOAD_ERR_64DD_IPL_LOAD_FAIL: return "Error occured during 64DD IPL loading";
        case CART_LOAD_ERR_64DD_DISK_LOAD_FAIL: return "Error occured during 64DD disk loading";
        case CART_LOAD_ERR_EMU_NOT_FOUND: return "Required emulator file was not found";
        case CART_LOAD_ERR_EMU_LOAD_FAIL: return "Error occured during emulator ROM loading";
        case CART_LOAD_ERR_EMU_ROM_LOAD_FAIL: return "Error occured during emulated ROM loading";
        case CART_LOAD_ERR_CREATE_SAVES_SUBDIR_FAIL: return "Couldn't create saves subdirectory";
        case CART_LOAD_ERR_EXP_PAK_NOT_FOUND: return "Mandatory Expansion Pak accessory was not found";
        case CART_LOAD_ERR_FUNCTION_NOT_SUPPORTED: return "Your flashcart doesn't support required functionality";
        default: return "Unknown error [CART_LOAD]";
    }
}

cart_load_err_t cart_load_n64_rom_and_save (menu_t *menu, flashcart_progress_callback_t progress) {
    path_t *path = path_clone(menu->load.rom_path);

    bool byte_swap = (menu->load.rom_info.endianness == ENDIANNESS_BYTE_SWAP);
    flashcart_save_type_t save_type = convert_save_type(&menu->load.rom_info);

    menu->flashcart_err = flashcart_load_rom(path_get(path), byte_swap, progress);
    if (menu->flashcart_err != FLASHCART_OK) {
        path_free(path);
        return CART_LOAD_ERR_ROM_LOAD_FAIL;
    }

    path_ext_replace(path, "sav");
    if (menu->settings.use_saves_folder) {
        if ((save_type != FLASHCART_SAVE_TYPE_NONE) && create_saves_subdirectory(path)) {
            path_free(path);
            return CART_LOAD_ERR_CREATE_SAVES_SUBDIR_FAIL;
        }
        path_push_subdir(path, SAVES_SUBDIRECTORY);
    }

    menu->flashcart_err = flashcart_load_save(path_get(path), save_type);
    if (menu->flashcart_err != FLASHCART_OK) {
        path_free(path);
        return CART_LOAD_ERR_SAVE_LOAD_FAIL;
    }

    path_free(path);

    return CART_LOAD_OK;
}

cart_load_err_t cart_load_64dd_ipl_and_disk (menu_t *menu, flashcart_progress_callback_t progress) {
    if (!flashcart_has_feature(FLASHCART_FEATURE_64DD)) {
        return CART_LOAD_ERR_FUNCTION_NOT_SUPPORTED;
    }

    if (is_64dd_connected()) {
        return CART_LOAD_ERR_64DD_PRESENT;
    }

    if (!is_memory_expanded()) {
        return CART_LOAD_ERR_EXP_PAK_NOT_FOUND;
    }

    path_t *path = path_init("sd:/", DDIPL_LOCATION);
    flashcart_disk_parameters_t disk_parameters;

    disk_parameters.development_drive = (menu->load.disk_info.region == DISK_REGION_DEVELOPMENT);
    disk_parameters.disk_type = menu->load.disk_info.disk_type;
    memcpy(disk_parameters.bad_system_area_lbas, menu->load.disk_info.bad_system_area_lbas, sizeof(disk_parameters.bad_system_area_lbas));
    memcpy(disk_parameters.defect_tracks, menu->load.disk_info.defect_tracks, sizeof(disk_parameters.defect_tracks));

    switch (menu->load.disk_info.region) {
        case DISK_REGION_DEVELOPMENT:
            path_push(path, "NDXJ0.n64");
            break;
        case DISK_REGION_JAPANESE:
            path_push(path, "NDDJ2.n64");
            break;
        case DISK_REGION_USA:
            path_push(path, "NDDE0.n64");
            break;
    }

    if (!file_exists(path_get(path))) {
        path_free(path);
        return CART_LOAD_ERR_64DD_IPL_NOT_FOUND;
    }

    menu->flashcart_err = flashcart_load_64dd_ipl(path_get(path), progress);
    if (menu->flashcart_err != FLASHCART_OK) {
        path_free(path);
        return CART_LOAD_ERR_64DD_IPL_LOAD_FAIL;
    }

    path_free(path);

    menu->flashcart_err = flashcart_load_64dd_disk(path_get(menu->load.disk_path), &disk_parameters);
    if (menu->flashcart_err != FLASHCART_OK) {
        return CART_LOAD_ERR_64DD_DISK_LOAD_FAIL;
    }

    return CART_LOAD_OK;
}

cart_load_err_t cart_load_emulator (menu_t *menu, cart_load_emu_type_t emu_type, flashcart_progress_callback_t progress) {
    path_t *path = path_init("sd:/", EMU_LOCATION);

    flashcart_save_type_t save_type = FLASHCART_SAVE_TYPE_NONE;
    uint32_t emulated_rom_offset = 0x200000;
    uint32_t emulated_file_offset = 0;

    switch (emu_type) {
        case CART_LOAD_EMU_TYPE_NES:
            path_push(path, "neon64bu.rom");
            save_type = FLASHCART_SAVE_TYPE_SRAM_BANKED;
            break;
        case CART_LOAD_EMU_TYPE_SNES:
            path_push(path, "sodium64.z64");
            save_type = FLASHCART_SAVE_TYPE_SRAM;
            break;
        case CART_LOAD_EMU_TYPE_GAMEBOY:
            path_push(path, "gb.v64");
            save_type = FLASHCART_SAVE_TYPE_FLASHRAM;
            break;
        case CART_LOAD_EMU_TYPE_GAMEBOY_COLOR:
            path_push(path, "gbc.v64");
            save_type = FLASHCART_SAVE_TYPE_FLASHRAM;
            break;
        case CART_LOAD_EMU_TYPE_SEGA_GENERIC_8BIT:
            path_push(path, "TotalSMS.z64");
            save_type = FLASHCART_SAVE_TYPE_SRAM;
            break;
    }

    if (!file_exists(path_get(path))) {
        path_free(path);
        return CART_LOAD_ERR_EMU_NOT_FOUND;
    }

    menu->flashcart_err = flashcart_load_rom(path_get(path), false, progress);
    if (menu->flashcart_err != FLASHCART_OK) {
        path_free(path);
        return CART_LOAD_ERR_EMU_LOAD_FAIL;
    }

    path_free(path);

    path = path_clone_push(menu->browser.directory, menu->browser.entry->name);

    switch (emu_type) {
        case CART_LOAD_EMU_TYPE_SNES:
            // NOTE: The emulator expects the header to be removed from the ROM being uploaded.
            emulated_file_offset = ((file_get_size(path_get(path)) & 0x3FF) == 0x200) ? 0x200 : 0;
            break;
        default:
            break;
    }

    menu->flashcart_err = flashcart_load_file(path_get(path), emulated_rom_offset, emulated_file_offset);
    if (menu->flashcart_err != FLASHCART_OK) {
        path_free(path);
        return CART_LOAD_ERR_EMU_ROM_LOAD_FAIL;
    }

    path_ext_replace(path, "sav");
    if (menu->settings.use_saves_folder) {
        if ((save_type != FLASHCART_SAVE_TYPE_NONE) && create_saves_subdirectory(path)) {
            path_free(path);
            return CART_LOAD_ERR_CREATE_SAVES_SUBDIR_FAIL;
        }
        path_push_subdir(path, SAVES_SUBDIRECTORY);
    }

    menu->flashcart_err = flashcart_load_save(path_get(path), save_type);
    if (menu->flashcart_err != FLASHCART_OK) {
        path_free(path);
        return CART_LOAD_ERR_SAVE_LOAD_FAIL;
    }

    path_free(path);

    return CART_LOAD_OK;
}
