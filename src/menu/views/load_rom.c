#include "../cart_load.h"
#include "../rom_database.h"
#include "boot/boot.h"
#include "views.h"


static bool load_pending;
static component_boxart_t *boxart;


static char *format_rom_endian (rom_endian_type_t endian) {
    switch (endian) {
        case ROM_BIG_ENDIAN:
        case IPL_BIG_ENDIAN:
            return "Big (default)";
        case ROM_LITTLE_ENDIAN:
            return "Little (unsupported)";
        case ROM_MID_BIG_ENDIAN:
            return "Byte swapped";
        default:
            return "Unknown";
    }
}

static char *format_rom_media_type (rom_media_type_t media_type) {
    switch (media_type) {
        case N64_CART:
            return "Cartridge";
        case N64_DISK:
            return "Disk";
        case N64_CART_EXPANDABLE:
            return "Cartridge (Expandable)";
        case N64_DISK_EXPANDABLE:
            return "Disk (Expandable)";
        case N64_ALECK64:
            return "Aleck64";
        default:
            return "Unknown";
    }
}

static char *format_rom_destination_market (rom_destination_market_t market_type) {
    // TODO: These are all assumptions and should be corrected if required.
    // From http://n64devkit.square7.ch/info/submission/pal/01-01.html
    switch (market_type) {
        case MARKET_JAPANESE_MULTI:
            return "Japanese & English"; // 1080 Snowboarding JPN
        case MARKET_BRAZILIAN:
            return "Brazilian (Portuguese)";
        case MARKET_CHINESE:
            return "Chinese";
        case MARKET_GERMAN:
            return "German";
        case MARKET_NORTH_AMERICA:
            return "American English";
        case MARKET_FRENCH:
            return "French";
        case MARKET_DUTCH:
            return "Dutch";
        case MARKET_ITALIAN:
            return "Italian";
        case MARKET_JAPANESE:
            return "Japanese";
        case MARKET_KOREAN:
            return "Korean";
        case MARKET_CANADIAN:
            return "Canadaian (English & French)";
        case MARKET_SPANISH:
            return "Spanish";
        case MARKET_AUSTRALIAN:
            return "Australian (English)";
        case MARKET_SCANDINAVIAN:
            return "Scandinavian";
        case MARKET_GATEWAY64_NTSC:
            return "LodgeNet/Gateway (NTSC)";
        case MARKET_GATEWAY64_PAL:
            return "LodgeNet/Gateway (PAL)";
        case MARKET_EUROPEAN_BASIC:
            return "PAL (includes English)"; // Mostly EU but is used on some Australian ROMs
        case MARKET_OTHER_X: // FIXME: AUS HSV Racing ROM's and Asia Top Gear Rally use this so not only EUR
            return "Regional (non specific)";
        case MARKET_OTHER_Y:
            return "European (non specific)";
        case MARKET_OTHER_Z:
            return "Regional (unknown)";
        default:
            return "Unknown";
    }
}

static char *format_rom_save_type (db_savetype_t save_type) {
    switch (save_type) {
        case DB_SAVE_TYPE_NONE:
            return "None";
        case DB_SAVE_TYPE_EEPROM_4K:
            return "EEPROM 4K";
        case DB_SAVE_TYPE_EEPROM_16K:
            return "EEPROM 16K";
        case DB_SAVE_TYPE_SRAM:
            return "SRAM";
        case DB_SAVE_TYPE_SRAM_BANKED:
            return "SRAM Banked";
        case DB_SAVE_TYPE_SRAM_128K:
            return "SRAM 128K";
        case DB_SAVE_TYPE_FLASHRAM:
            return "FlashRAM";
        case DB_SAVE_TYPE_CPAK:
            return "Controller Pak";
        default:
            return "Unknown";
    }
}

static char *format_rom_expansion_pak_info (rom_memorytype_t expansion_pak_info) {
    switch (expansion_pak_info) {
        case DB_MEMORY_EXPANSION_REQUIRED:
            return "Required";
        case DB_MEMORY_EXPANSION_RECOMMENDED:
            return "Recommended";
        case DB_MEMORY_EXPANSION_SUGGESTED:
            return "Suggested";
        case DB_MEMORY_EXPANSION_FAULTY:
            return "May require ROM patch";
        default:
            return "Not required";
    }
}

static float format_rom_clockrate (uint32_t clockrate) {
    /* Generally ROMs have a clock rate of 0x0000000F which signifies they used the default 62.5MHz clock. */
    if (clockrate == 0x0F) {
        return 62.5;
    }

    /* If it did not, we need to show the different value. */
    return (float) clockrate / 1000000;
}


static void process (menu_t *menu) {
    if (menu->actions.enter) {
        load_pending = true;
    } else if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
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
            " Endian: %s\n"
            " Title: %.20s\n"
            " Media type: %c - %s\n"
            " Unique ID: %.2s\n"
            " Destination market: %c - %s\n"
            " Version: %hhu\n"
            " Checksum: 0x%016llX\n"
            " Save type: %s\n"
            " Expansion PAK: %s\n"
            "\n"
            " Extra information:\n"
            "  Boot address: 0x%08lX\n"
            "  SDK version: %.1f%c\n"
            "  Clock Rate: %.2fMHz\n",
            format_rom_endian(menu->load.rom_header.config_flags),
            menu->load.rom_header.title,
            menu->load.rom_header.metadata.media_type, format_rom_media_type(menu->load.rom_header.metadata.media_type),
            (char *) (&menu->load.rom_header.metadata.unique_identifier),
            menu->load.rom_header.metadata.destination_market, format_rom_destination_market(menu->load.rom_header.metadata.destination_market),
            menu->load.rom_header.metadata.version,
            menu->load.rom_header.checksum,
            format_rom_save_type(rom_db_match_save_type(menu->load.rom_header)),
            format_rom_expansion_pak_info(rom_db_match_expansion_pak(menu->load.rom_header)),
            menu->load.rom_header.boot_address,
            (float) ((menu->load.rom_header.sdk_version >> 8) & 0xFF) / 10.0f, (char) (menu->load.rom_header.sdk_version & 0xFF),
            format_rom_clockrate(menu->load.rom_header.clock_rate)
        );

        component_actions_bar_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "A: Load and run ROM\n"
            "B: Exit"
        );

        component_boxart_draw(boxart);
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
    menu->boot_params->tv_type = BOOT_TV_TYPE_PASSTHROUGH;
    menu->boot_params->detect_cic_seed = true;
}

static void deinit (menu_t *menu) {
    component_boxart_free(boxart);
}


void view_load_rom_init (menu_t *menu) {
    if (menu->load.rom_path) {
        path_free(menu->load.rom_path);
        menu->load.rom_path = NULL;
    }

    load_pending = false;

    menu->load.rom_path = path_clone_push(menu->browser.directory, menu->browser.entry->name);

    menu->load.rom_header = file_read_rom_header(path_get(menu->load.rom_path));

    uint8_t media_type = menu->load.rom_header.metadata.media_type;
    uint16_t id = menu->load.rom_header.metadata.unique_identifier;
    boxart = component_boxart_init(media_type, id);
}

void view_load_rom_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);

    if (load_pending) {
        load_pending = false;
        load(menu);
    }

    if (menu->next_mode != MENU_MODE_LOAD_ROM) {
        deinit(menu);
    }
}
