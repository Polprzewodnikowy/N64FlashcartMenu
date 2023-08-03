#include "../rom_database.h"
#include "boot/boot.h"
#include "flashcart/flashcart.h"
#include "views.h"


static bool load_pending;
static rom_header_t rom_header;
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
    switch (market_type) {
        case MARKET_ALL:
            return "All";
        case MARKET_BRAZIL:
            return "Brazil (MPAL)";
        case MARKET_CHINA:
            return "China";
        case MARKET_GERMANY:
            return "Germany (PAL)";
        case MARKET_USA:
            return "USA (NTSC)";
        case MARKET_FRANCE:
            return "France (PAL)";
        case MARKET_NETHERLANDS:
            return "Netherlands (PAL)";
        case MARKET_ITALY:
            return "Italy (PAL)";
        case MARKET_JAPAN:
            return "Japan (NTSC)";
        case MARKET_KOREA:
            return "Korea";
        case MARKET_CANADA:
            return "Canada";
        case MARKET_SPAIN:
            return "Spain (PAL)";
        case MARKET_AUSTRAILA:
            return "Austraila (PAL)";
        case MARKET_SCANDINAVAIA:
            return "Scandinavaia";
        case MARKET_GATEWAY64_NTSC:
            return "Gateway (NTSC)";
        case MARKET_GATEWAY64_PAL:
            return "Gateway (PAL)";
        case MARKET_PAL_GENERIC:
            return "Generic (PAL)";
        case MARKET_PAL_X: // FIXME: some AUS ROM's use this so not only EUR
        case MARKET_PAL_Y:
        case MARKET_PAL_Z:
            return "Unknown (PAL)";
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
            menu->browser.list[menu->browser.selected].name
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
            " Extra info:\n"
            "  Boot address: 0x%08lX\n"
            "  SDK version: %.1f%c",
            format_rom_endian(rom_header.config_flags),
            rom_header.title,
            rom_header.metadata.media_type, format_rom_media_type(rom_header.metadata.media_type),
            (char *) (&rom_header.metadata.unique_identifier),
            rom_header.metadata.destination_market, format_rom_destination_market(rom_header.metadata.destination_market),
            rom_header.version,
            rom_header.checksum,
            format_rom_save_type(rom_db_match_save_type(rom_header)),
            format_rom_expansion_pak_info(rom_db_match_expansion_pak(rom_header)),
            rom_header.boot_address,
            (float) ((rom_header.sdk_version >> 8) & 0xFF) / 10.0f, (char) (rom_header.sdk_version & 0xFF)
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
    surface_t *d = display_try_get();

    if (d) {
        rdpq_attach(d, NULL);

        component_background_draw();

        component_loader_draw(progress);

        rdpq_detach_show();
    }
}

static flashcart_save_type_t convert_save_type (db_savetype_t save_type) {
    switch (save_type) {
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

static void load (menu_t *menu) {
    menu->next_mode = MENU_MODE_BOOT;

    path_t *path = path_clone(menu->browser.directory);
    path_push(path, menu->browser.list[menu->browser.selected].name);

    bool byte_swap = (rom_header.config_flags == ROM_MID_BIG_ENDIAN);
    menu->flashcart_error = flashcart_load_rom(path_get(path), byte_swap, draw_progress);
    if (menu->flashcart_error != FLASHCART_OK) {
        menu->next_mode = MENU_MODE_FAULT;
        path_free(path);
        return;
    }

    uint8_t save_type = rom_db_match_save_type(rom_header);

    path_ext_replace(path, "sav");
    menu->flashcart_error = flashcart_load_save(path_get(path), convert_save_type(save_type));
    if (menu->flashcart_error != FLASHCART_OK) {
        menu->next_mode = MENU_MODE_FAULT;
        path_free(path);
        return;
    }

    path_free(path);

    menu->boot_params->device_type = BOOT_DEVICE_TYPE_ROM;
    menu->boot_params->tv_type = BOOT_TV_TYPE_PASSTHROUGH;
    menu->boot_params->detect_cic_seed = true;
}

static void deinit (void) {
    component_boxart_free(boxart);
}


void view_load_init (menu_t *menu) {
    load_pending = false;

    path_t *path = path_clone(menu->browser.directory);
    path_push(path, menu->browser.list[menu->browser.selected].name);

    rom_header = file_read_rom_header(path_get(path));

    boxart = component_boxart_init(rom_header.metadata.unique_identifier);

    path_free(path);
}

void view_load_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);

    if (load_pending) {
        load_pending = false;
        load(menu);
    }

    if (menu->next_mode != MENU_MODE_LOAD) {
        deinit();
    }
}
