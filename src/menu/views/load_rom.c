#include "../cart_load.h"
#include "../rom_info.h"
#include "boot/boot.h"
#include "views.h"


static bool load_pending;
static component_boxart_t *boxart;


static char *convert_error_message (disk_err_t err) {
    switch (err) {
        case ROM_ERR_IO: return "I/O error during loading ROM information";
        case ROM_ERR_NO_FILE: return "Couldn't open ROM file";
        default: return "Unknown ROM info load error";
    }
}

static const char *format_rom_endianness (endianness_t endianness) {
    switch (endianness) {
        case ENDIANNESS_BIG: return "Big (default)";
        case ENDIANNESS_LITTLE: return "Little (unsupported)";
        case ENDIANNESS_BYTE_SWAP: return "Byte swapped";
        default: return "Unknown";
    }
}

static const char *format_rom_media_type (category_type_t media_type) {
    switch (media_type) {
        case N64_CART: return "Cartridge";
        case N64_DISK: return "Disk";
        case N64_CART_EXPANDABLE: return "Cartridge (Expandable)";
        case N64_DISK_EXPANDABLE: return "Disk (Expandable)";
        case N64_ALECK64: return "Aleck64";
        default: return "Unknown";
    }
}

static const char *format_rom_destination_market (destination_type_t market_type) {
    // TODO: These are all assumptions and should be corrected if required.
    // From http://n64devkit.square7.ch/info/submission/pal/01-01.html
    switch (market_type) {
        case MARKET_JAPANESE_MULTI: return "Japanese & English"; // 1080 Snowboarding JPN
        case MARKET_BRAZILIAN: return "Brazilian (Portuguese)";
        case MARKET_CHINESE: return "Chinese";
        case MARKET_GERMAN: return "German";
        case MARKET_NORTH_AMERICA: return "American English";
        case MARKET_FRENCH: return "French";
        case MARKET_DUTCH: return "Dutch";
        case MARKET_ITALIAN: return "Italian";
        case MARKET_JAPANESE: return "Japanese";
        case MARKET_KOREAN: return "Korean";
        case MARKET_CANADIAN: return "Canadaian (English & French)";
        case MARKET_SPANISH: return "Spanish";
        case MARKET_AUSTRALIAN: return "Australian (English)";
        case MARKET_SCANDINAVIAN: return "Scandinavian";
        case MARKET_GATEWAY64_NTSC: return "LodgeNet/Gateway (NTSC)";
        case MARKET_GATEWAY64_PAL: return "LodgeNet/Gateway (PAL)";
        case MARKET_EUROPEAN_BASIC: return "PAL (includes English)"; // Mostly EU but is used on some Australian ROMs
        case MARKET_OTHER_X: return "Regional (non specific)"; // FIXME: AUS HSV Racing ROM's and Asia Top Gear Rally use this so not only EUR
        case MARKET_OTHER_Y: return "European (non specific)";
        case MARKET_OTHER_Z: return "Regional (unknown)";
        default: return "Unknown";
    }
}

static const char *format_rom_save_type (rom_save_type_t save_type) {
    switch (save_type) {
        case SAVE_TYPE_NONE: return "None";
        case SAVE_TYPE_EEPROM_4K: return "EEPROM 4K";
        case SAVE_TYPE_EEPROM_16K: return "EEPROM 16K";
        case SAVE_TYPE_SRAM: return "SRAM";
        case SAVE_TYPE_SRAM_BANKED: return "SRAM Banked";
        case SAVE_TYPE_SRAM_128K: return "SRAM 128K";
        case SAVE_TYPE_FLASHRAM: return "FlashRAM";
        case SAVE_TYPE_FLASHRAM_PKST2: return "FlashRAM (Pokemon Stadium 2)";
        default: return "Unknown";
    }
}

static const char *format_rom_tv_type (rom_tv_type_t tv_type) {
    switch (tv_type) {
        case ROM_TV_TYPE_PAL: return "PAL";
        case ROM_TV_TYPE_NTSC: return "NTSC";
        case ROM_TV_TYPE_MPAL: return "MPAL";
        default: return "Unknown";
    }
}

static char *format_rom_expansion_pak_info (expansion_pak_t expansion_pak_info) {
    switch (expansion_pak_info) {
        case EXPANSION_PAK_REQUIRED: return "Required";
        case EXPANSION_PAK_RECOMMENDED: return "Recommended";
        case EXPANSION_PAK_SUGGESTED: return "Suggested";
        case EXPANSION_PAK_FAULTY: return "May require ROM patch";
        default: return "Not required";
    }
}

static const char *format_cic_type (cic_type_t cic_type) {
    switch (cic_type) {
        case CIC_5101: return "5101";
        case CIC_5167: return "5167";
        case CIC_6101: return "6101";
        case CIC_7102: return "7102";
        case CIC_6102_7101: return "6102 / 7101";
        case CIC_x103: return "6103 / 7103";
        case CIC_x105: return "6105 / 7105";
        case CIC_x106: return "6106 / 7106";
        case CIC_8301: return "8301";
        case CIC_8302: return "8302";
        case CIC_8303: return "8303";
        case CIC_8401: return "8401";
        case CIC_8501: return "8501";
        default: return "Unknown";
    }
}

static void set_save_type (menu_t *menu, rom_save_type_t save_type) {
    rom_info_override_save_type(menu->load.rom_path, &menu->load.rom_info, save_type);
    menu->browser.reload = true;
}

static void set_save_type_automatic (menu_t *menu) { set_save_type(menu, SAVE_TYPE_AUTOMATIC); }
static void set_save_type_none (menu_t *menu) { set_save_type(menu, SAVE_TYPE_NONE); }
static void set_save_type_eeprom_4kbit (menu_t *menu) { set_save_type(menu, SAVE_TYPE_EEPROM_4K); }
static void set_save_type_eeprom_16kbit (menu_t *menu) { set_save_type(menu, SAVE_TYPE_EEPROM_16K); }
static void set_save_type_sram_256kbit (menu_t *menu) { set_save_type(menu, SAVE_TYPE_SRAM); }
static void set_save_type_sram_768kbit (menu_t *menu) { set_save_type(menu, SAVE_TYPE_SRAM_BANKED); }
static void set_save_type_sram_1mbit (menu_t *menu) { set_save_type(menu, SAVE_TYPE_SRAM_128K); }
static void set_save_type_flash_ram_1mbit (menu_t *menu) { set_save_type(menu, SAVE_TYPE_FLASHRAM); }

static component_context_menu_t set_save_type_context_menu = { .list = {
    { .text = "Automatic", .action = set_save_type_automatic },
    { .text = "None", .action = set_save_type_none },
    { .text = "EEPROM 4kbit", .action = set_save_type_eeprom_4kbit },
    { .text = "EEPROM 16kbit", .action = set_save_type_eeprom_16kbit },
    { .text = "SRAM 256kbit", .action = set_save_type_sram_256kbit },
    { .text = "SRAM 768kbit", .action = set_save_type_sram_768kbit },
    { .text = "SRAM 1Mbit", .action = set_save_type_sram_1mbit },
    { .text = "FlashRAM 1Mbit", .action = set_save_type_flash_ram_1mbit },
    COMPONENT_CONTEXT_MENU_LIST_END,
}};

static void set_tv_type (menu_t *menu, rom_tv_type_t tv_type) {
    rom_info_override_tv_type(menu->load.rom_path, &menu->load.rom_info, tv_type);
    menu->browser.reload = true;
}

static void set_tv_type_automatic (menu_t *menu) { set_tv_type(menu, ROM_TV_TYPE_AUTOMATIC); }
static void set_tv_type_pal (menu_t *menu) { set_tv_type(menu, ROM_TV_TYPE_PAL); }
static void set_tv_type_ntsc (menu_t *menu) { set_tv_type(menu, ROM_TV_TYPE_NTSC); }
static void set_tv_type_mpal (menu_t *menu) { set_tv_type(menu, ROM_TV_TYPE_MPAL); }

static component_context_menu_t set_tv_type_context_menu = { .list = {
    { .text = "Automatic", .action = set_tv_type_automatic },
    { .text = "PAL", .action = set_tv_type_pal },
    { .text = "NTSC", .action = set_tv_type_ntsc },
    { .text = "MPAL", .action = set_tv_type_mpal },
    COMPONENT_CONTEXT_MENU_LIST_END,
}};

static component_context_menu_t options_context_menu = { .list = {
    { .text = "Set save type", .submenu = &set_save_type_context_menu },
    { .text = "Set TV type", .submenu = &set_tv_type_context_menu },
    COMPONENT_CONTEXT_MENU_LIST_END,
}};

static void process (menu_t *menu) {
    if (component_context_menu_process(menu, &options_context_menu)) {
        return;
    }

    if (menu->actions.enter) {
        load_pending = true;
    } else if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
    } else if (menu->actions.options) {
        component_context_menu_show(&options_context_menu);
    }
}

static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    component_background_draw();

    if (load_pending) {
        component_loader_draw(0.0f);
    } else {
        component_layout_draw();

        component_main_text_draw(
            ALIGN_CENTER, VALIGN_TOP,
            "N64 ROM information\n"
            "\n"
            "%s",
            menu->browser.entry->name
        );

        component_main_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "\n"
            "\n"
            "\n"
            "\n"
            " Endianness: %s\n"
            " Title: %.20s\n"
            " Game code: %c%c%c%c\n"
            " Media type: %s\n"
            " Destination market: %s\n"
            " Version: %hhu\n"
            " Check code: 0x%016llX\n"
            " Save type: %s\n"
            " TV type: %s\n"
            " Expansion PAK: %s\n"
            "\n"
            " Extra information:\n"
            "  CIC: %s\n"
            "  Boot address: 0x%08lX\n"
            "  SDK version: %.1f%c\n"
            "  Clock Rate: %.2fMHz\n",
            format_rom_endianness(menu->load.rom_info.endianness),
            menu->load.rom_info.title,
            menu->load.rom_info.game_code[0], menu->load.rom_info.game_code[1], menu->load.rom_info.game_code[2], menu->load.rom_info.game_code[3],
            format_rom_media_type(menu->load.rom_info.category_code),
            format_rom_destination_market(menu->load.rom_info.destination_code),
            menu->load.rom_info.version,
            menu->load.rom_info.check_code,
            format_rom_save_type(rom_info_get_save_type(&menu->load.rom_info)),
            format_rom_tv_type(rom_info_get_tv_type(&menu->load.rom_info)),
            format_rom_expansion_pak_info(menu->load.rom_info.features.expansion_pak),
            format_cic_type(menu->load.rom_info.cic_type),
            menu->load.rom_info.boot_address,
            (menu->load.rom_info.libultra.version / 10.0f), menu->load.rom_info.libultra.revision,
            menu->load.rom_info.clock_rate
        );

        component_actions_bar_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "A: Load and run ROM\n"
            "B: Exit"
        );

        component_actions_bar_text_draw(
            ALIGN_RIGHT, VALIGN_TOP,
            "\n"
            "R: Options"
        );

        component_boxart_draw(boxart);

        component_context_menu_draw(&options_context_menu);
    }

    rdpq_detach_show();
}

static void draw_progress (float progress) {
    surface_t *d = (progress >= 1.0f) ? display_get() : display_try_get();

    if (d) {
        rdpq_attach(d, NULL);

        component_background_draw();

        component_loader_draw(progress);

        rdpq_detach_show();
    }
}

static void load (menu_t *menu) {
    cart_load_err_t err = cart_load_n64_rom_and_save(menu, draw_progress);

    if (err != CART_LOAD_OK) {
        menu_show_error(menu, cart_load_convert_error_message(err));
        return;
    }

    menu->next_mode = MENU_MODE_BOOT;

    menu->boot_params->device_type = BOOT_DEVICE_TYPE_ROM;
    menu->boot_params->detect_cic_seed = true;
    switch(rom_info_get_tv_type(&menu->load.rom_info)) {
        case ROM_TV_TYPE_PAL: menu->boot_params->tv_type = BOOT_TV_TYPE_PAL; break;
        case ROM_TV_TYPE_NTSC: menu->boot_params->tv_type = BOOT_TV_TYPE_NTSC; break;
        case ROM_TV_TYPE_MPAL: menu->boot_params->tv_type = BOOT_TV_TYPE_MPAL; break;
        default: menu->boot_params->tv_type = BOOT_TV_TYPE_PASSTHROUGH; break;
    }
}

static void deinit (void) {
    component_boxart_free(boxart);
}


void view_load_rom_init (menu_t *menu) {
    if (menu->load.rom_path) {
        path_free(menu->load.rom_path);
        menu->load.rom_path = NULL;
    }

    load_pending = false;

    menu->load.rom_path = path_clone_push(menu->browser.directory, menu->browser.entry->name);

    rom_err_t err = rom_info_load(menu->load.rom_path, &menu->load.rom_info);
    if (err != ROM_OK) {
        menu_show_error(menu, convert_error_message(err));
    }

    boxart = component_boxart_init(menu->load.rom_info.game_code);

    component_context_menu_init(&options_context_menu);
}

void view_load_rom_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);

    if (load_pending) {
        load_pending = false;
        load(menu);
    }

    if (menu->next_mode != MENU_MODE_LOAD_ROM) {
        deinit();
    }
}
