#include "cart_load.h"
#include "path.h"
#include "utils/fs.h"


#define SAVES_SUBDIRECTORY      "saves"
#define EMU_LOCATION            "/emulators"


static bool create_saves_subdirectory (menu_t *menu) {
    path_t *save_folder_path = path_clone_push(menu->browser.directory, SAVES_SUBDIRECTORY);
    bool error = directory_create(path_get(save_folder_path));
    path_free(save_folder_path);
    return error;
}

static flashcart_save_type_t convert_save_type (rom_header_t *header) {
    switch (rom_db_match_save_type(*header)) {
        case DB_SAVE_TYPE_EEPROM_4K:
            return FLASHCART_SAVE_TYPE_EEPROM_4K;
        case DB_SAVE_TYPE_EEPROM_16K:
            return FLASHCART_SAVE_TYPE_EEPROM_16K;
        case DB_SAVE_TYPE_SRAM:
            return FLASHCART_SAVE_TYPE_SRAM;
        case DB_SAVE_TYPE_SRAM_BANKED:
            return FLASHCART_SAVE_TYPE_SRAM_BANKED;
        case DB_SAVE_TYPE_SRAM_128K:
            return FLASHCART_SAVE_TYPE_SRAM_128K;
        case DB_SAVE_TYPE_FLASHRAM:
            return FLASHCART_SAVE_TYPE_FLASHRAM;
        default:
            return FLASHCART_SAVE_TYPE_NONE;
    }
}


char *cart_load_convert_error_message (cart_load_err_t err) {
    switch (err) {
        case CART_LOAD_ERR_SAVES_SUBDIR: return "Could not create saves subdirectory";
        case CART_LOAD_ERR_ROM: return "Error during ROM loading";
        case CART_LOAD_ERR_SAVE: return "Error during save creation or loading";
        case CART_LOAD_ERR_EMU_NOT_FOUND: return "Emulator ROM not found";
        case CART_LOAD_ERR_EMU: return "Error during emulator ROM loading";
        case CART_LOAD_ERR_EMU_ROM: return "Error during emulated ROM loading";
        default: return "Unknown cart load error";
    }
}

cart_load_err_t cart_load_n64_rom_and_save (menu_t *menu, rom_header_t *header, flashcart_progress_callback_t progress) {
    path_t *path = path_clone_push(menu->browser.directory, menu->browser.entry->name);
    bool byte_swap = (header->config_flags == ROM_MID_BIG_ENDIAN);
    flashcart_save_type_t save_type = convert_save_type(header);

    menu->flashcart_error = flashcart_load_rom(path_get(path), byte_swap, progress);
    if (menu->flashcart_error != FLASHCART_OK) {
        path_free(path);
        return CART_LOAD_ERR_ROM;
    }

    path_ext_replace(path, "sav");
    if (menu->settings.use_saves_folder) {
        path_push_subdir(path, SAVES_SUBDIRECTORY);
        if ((save_type != FLASHCART_SAVE_TYPE_NONE) && create_saves_subdirectory(menu)) {
            path_free(path);
            return CART_LOAD_ERR_SAVES_SUBDIR;
        }
    }

    menu->flashcart_error = flashcart_load_save(path_get(path), save_type);
    if (menu->flashcart_error != FLASHCART_OK) {
        path_free(path);
        return CART_LOAD_ERR_SAVE;
    }

    path_free(path);

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
            emulated_rom_offset = 0x104000;
            break;
        case CART_LOAD_EMU_TYPE_GAMEBOY:
            path_push(path, "gb.v64");
            save_type = FLASHCART_SAVE_TYPE_FLASHRAM;
            break;
        case CART_LOAD_EMU_TYPE_GAMEBOY_COLOR:
            path_push(path, "gbc.v64");
            save_type = FLASHCART_SAVE_TYPE_FLASHRAM;
            break;
    }

    if (!file_exists(path_get(path))) {
        path_free(path);
        return CART_LOAD_ERR_EMU_NOT_FOUND;
    }

    menu->flashcart_error = flashcart_load_rom(path_get(path), false, progress);
    if (menu->flashcart_error != FLASHCART_OK) {
        path_free(path);
        return CART_LOAD_ERR_EMU;
    }

    path_free(path);

    path = path_clone_push(menu->browser.directory, menu->browser.entry->name);

    switch (emu_type) {
        case CART_LOAD_EMU_TYPE_SNES:
            emulated_file_offset = ((file_get_size(path_get(path)) & 0x3FF) == 0x200) ? 0x200 : 0;
            break;
        default:
            break;
    }

    menu->flashcart_error = flashcart_load_file(path_get(path), emulated_rom_offset, emulated_file_offset);
    if (menu->flashcart_error != FLASHCART_OK) {
        path_free(path);
        return CART_LOAD_ERR_EMU_ROM;
    }

    path_ext_replace(path, "sav");
    if (menu->settings.use_saves_folder) {
        path_push_subdir(path, SAVES_SUBDIRECTORY);
        if ((save_type != FLASHCART_SAVE_TYPE_NONE) && create_saves_subdirectory(menu)) {
            path_free(path);
            return CART_LOAD_ERR_SAVES_SUBDIR;
        }
    }

    menu->flashcart_error = flashcart_load_save(path_get(path), save_type);
    if (menu->flashcart_error != FLASHCART_OK) {
        path_free(path);
        return CART_LOAD_ERR_SAVE;
    }

    path_free(path);

    return CART_LOAD_OK;
}
