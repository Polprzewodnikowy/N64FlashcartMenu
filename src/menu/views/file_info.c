#include <fatfs/ff.h>
#include <libdragon.h>
#include <stdlib.h>

#include "../png_decoder.h"
#include "../rom_database.h"
#include "fragments/fragments.h"
#include "utils/fs.h"
#include "utils/str_utils.h"
#include "views.h"


#ifndef ROM_BOXART_PATH
#define ROM_BOXART_PATH "/menu/boxart"
#endif


static FILINFO info;
static const char *n64_rom_extensions[] = { "z64", "n64", "v64", NULL };
static rom_header_t rom_header;

static bool boxart_image_loading;
static surface_t *boxart_image;


static char *format_rom_endian (uint32_t endian) {
    switch (endian)
    {
    case ROM_BIG_ENDIAN:
    case IPL_BIG_ENDIAN:
        return "Big (default)"; // expected
        break;
    case ROM_LITTLE_ENDIAN:
        return "Little";
        break;
    case ROM_MID_LITTLE_ENDIAN:
        return "Middle Little";
        break;
    case ROM_MID_BIG_ENDIAN:
        return "Middle Big";
        break;
    default:
        return "Unknown";
        break;
    }
}

static char *format_rom_media_type (uint8_t type) {
    switch (type)
    {
    case N64_CART:
        return "Cartridge";
        break;
    case N64_DISK:
        return "Disk";
        break;
    case N64_CART_EXPANDABLE:
        return "Cart Expandable";
        break;
    case N64_DISK_EXPANDABLE:
        return "Disk Expandable";
        break;
    case N64_ALECK64:
        return "Aleck64";
        break;
    default:
        return "Unknown";
        break;
    }
}

static char *format_rom_destination_market (uint8_t market_type) {
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
        case MARKET_PAL_X: //FIXME: some AUS ROM's use this so not only EUR
        case MARKET_PAL_Y:
        case MARKET_PAL_Z:
            return "Unknown (PAL)";

        default:
            return "Unknown";
    }
}

static char *format_rom_save_type (uint8_t type) {
    switch (type)
    {
    case DB_SAVE_TYPE_EEPROM_4K:
        return "EEPROM 4K";
        break;
    case DB_SAVE_TYPE_EEPROM_16K:
        return "EEPROM 16K";
        break;
    case DB_SAVE_TYPE_SRAM:
        return "SRAM";
        break;
    case DB_SAVE_TYPE_SRAM_BANKED:
        return "SRAM Banked";
        break;
    case DB_SAVE_TYPE_SRAM_128K:
        return "SRAM 128K [ED64]";
        break;
    case DB_SAVE_TYPE_FLASHRAM:
        return "FLASH RAM";
        break;
    case DB_SAVE_TYPE_CPAK:
        return "Controller PAK";
        break;
    default:
        return "Unknown";
        break;
    }
}

static char *format_rom_memory_type (uint8_t type) {
    switch (type)
    {
    case DB_MEMORY_EXPANSION_REQUIRED:
        return "Required";
        break;
    case DB_MEMORY_EXPANSION_RECOMMENDED:
        return "Recommended";
        break;
    case DB_MEMORY_EXPANSION_SUGGESTED:
        return "Suggested";
        break;
    case DB_MEMORY_EXPANSION_FAULTY:
        return "Ensure Patched";
        break;
    default:
        return "Not Required";
        break;
    }
}


static char *format_file_type (void) {
    // TODO: should be at least a switch statement!
    if (str_endswith(info.fname, ".z64", false) ||
        str_endswith(info.fname, ".n64", false) ||
        str_endswith(info.fname, ".v64", false) ||
        str_endswith(info.fname, ".rom", false)
        ) {
        // TODO: check the necessary bytes in the header to ensure!
        return "N64 ROM";
    }
    else if (str_endswith(info.fname, ".txt", false)) {
        return "Text File";
    }
    else if (str_endswith(info.fname, ".ini", false)) {
        return "INI File";
    }
    else if (str_endswith(info.fname, ".yml", false) || str_endswith(info.fname, ".yaml", false)) {
        return "YAML File";
    }
    else if (str_endswith(info.fname, ".toml", false)) {
        return "TOML File";
    }
    else if (str_endswith(info.fname, ".sav", false) || str_endswith(info.fname, ".eep", false) || str_endswith(info.fname, ".sra", false) || str_endswith(info.fname, ".srm", false)|| str_endswith(info.fname, ".fla", false)) {
        return "ROM Save File";
    }
    else if (str_endswith(info.fname, ".ips", false) || str_endswith(info.fname, ".aps", false) || str_endswith(info.fname, ".pps", false) || str_endswith(info.fname, ".xdelta", false)) {
        return "ROM Patch File";
    }
    else if (str_endswith(info.fname, ".zip", false)) {
        return "ZIP Archive";
    }
    else if (str_endswith(info.fname, ".mpk", false)) {
        return "DexDrive CPak Backup File";
    }
    else if (str_endswith(info.fname, ".emu", false)) {
        return "Emulator File";
    }
    else {
        return "Unknown File";
    }
}

static void process (menu_t *menu) {
    if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

static void menu_fileinfo_draw_unknown_info(surface_t *d, layout_t *layout) {
    const int text_x = layout->offset_x + layout->offset_text_x;
    int text_y = layout->offset_y + layout->offset_text_y;
    text_y += fragment_textf(text_x, text_y, "File Information:\n\n");
    text_y += fragment_textf(text_x, text_y, "  Name:\n\n  %s\n", info.fname);
    text_y += fragment_textf(text_x, text_y, "  Size:\n\n  %d Bytes\n", (int)info.fsize);           
    text_y += fragment_textf(text_x, text_y, "  Attributes:\n\n  %s%s%s%s%s\n",
                ((info.fattrib & AM_DIR) ? "  Directory" : "File"),
                ((info.fattrib & AM_RDO) ? " | ReadOnly" : ""),
                ((info.fattrib & AM_SYS) ? " | System" : ""),
                ((info.fattrib & AM_ARC) ? " | Archive" : ""),
                ((info.fattrib & AM_HID) ? " | Hidden" : "")
                );

    text_y += fragment_textf(text_x, text_y, "  Modified Timestamp:\n\n  %u-%02u-%02u, %02u:%02u\n", 
                (info.fdate >> 9) + 1980,
                info.fdate >> 5 & 15,
                info.fdate & 31,
                info.ftime >> 11,
                info.ftime >> 5 & 63
                );

    text_y += fragment_textf(text_x, text_y, "  Type:\n\n  %s\n", format_file_type());
}

static void menu_fileinfo_draw_n64_rom_info(surface_t *d, layout_t *layout) {

    const int text_x = layout->offset_x + layout->offset_text_x;
    int text_y = layout->offset_y + layout->offset_text_y;
    text_y += fragment_textf(text_x, text_y, "File Information:\n\n");
    text_y += fragment_textf(text_x, text_y, "  Name:\n\n  %s\n", info.fname);
    text_y += fragment_textf(text_x, text_y, "  Size:\n\n  %d Bytes\n", (int)info.fsize);
    text_y += fragment_textf(text_x, text_y, "  Modified Timestamp:\n\n  %u-%02u-%02u, %02u:%02u\n", 
            (info.fdate >> 9) + 1980,
            info.fdate >> 5 & 15,
            info.fdate & 31,
            info.ftime >> 11,
            info.ftime >> 5 & 63
            );

    text_y += fragment_textf(text_x, text_y, "\n");
    text_y += fragment_textf(text_x, text_y, "N64 ROM Information:\n\n");
    text_y += fragment_textf(text_x, text_y, "  Endian: %s\n", format_rom_endian(rom_header.endian));
    text_y += fragment_textf(text_x, text_y, "  Title: %s\n", rom_header.title);
    text_y += fragment_textf(text_x, text_y, "  Media Type: %c - %s\n", rom_header.metadata.media_type, format_rom_media_type(rom_header.metadata.media_type));
    text_y += fragment_textf(text_x, text_y, "  Unique ID: %.2s\n", (char*)&(rom_header.metadata.unique_identifier));
    text_y += fragment_textf(text_x, text_y, "  Destination Market: %c - %s\n", rom_header.metadata.destination_market, format_rom_destination_market(rom_header.metadata.destination_market));
    text_y += fragment_textf(text_x, text_y, "  Version: %hhu\n", rom_header.version);
    text_y += fragment_textf(text_x, text_y, "  Checksum: 0x%016llX\n", rom_header.checksum);
    text_y += fragment_textf(text_x, text_y, "  Save Type: %s\n", format_rom_save_type(rom_db_match_save_type(rom_header)));
    text_y += fragment_textf(text_x, text_y, "  Expansion PAK: %s\n\n", format_rom_memory_type(rom_db_match_expansion_pak(rom_header)));

    if (boxart_image) {
        uint16_t x = (640 - 150) - (boxart_image->width / 2);
        uint16_t y = (480 - 150) - (boxart_image->height / 2);
        rdpq_set_mode_copy(false);
        rdpq_tex_blit(boxart_image, x, y, NULL);
    }
}

static void draw (menu_t *menu, surface_t *d) {
    layout_t *layout = layout_get();

    const int text_x = layout->offset_x + layout->offset_text_x;
    int text_y = layout->offset_y + layout->offset_text_y;

    const color_t bg_color = RGBA32(0x00, 0x00, 0x00, 0xFF);
    const color_t text_color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF);

    rdpq_attach(d, NULL);
    rdpq_clear(bg_color);

    // Layout
    fragment_borders(d);

    // Text start
    fragment_text_start(text_color);

    if (file_has_extensions(info.fname, n64_rom_extensions)) {
        menu_fileinfo_draw_n64_rom_info(d, layout);
    } else {
        menu_fileinfo_draw_unknown_info(d, layout);
    }

    // Ensure RDP mode and loaded texture dont mess up font drawing.
    fragment_text_start(text_color);

    // Actions bar
    text_y = layout->actions_y + layout->offset_text_y;
    text_y += fragment_textf(text_x, text_y, "B: Exit");

    rdpq_detach_show();
}

static void boxart_image_callback (png_err_t err, surface_t *decoded_image, void *callback_data) {
    boxart_image_loading = false;
    boxart_image = decoded_image;
}

/* loads a PNG image for a given ROM ID from ROM_BOXART_PATH. e.g. sd:/menu/boxart/<id>.png */
static void boxart_image_load (uint16_t id) {
    char sd_boxart_path[32];
    sprintf(sd_boxart_path, "%s/%.2s.png", ROM_BOXART_PATH, (char *) (&id));

    if (png_decode_start(sd_boxart_path, 158, 112, boxart_image_callback, NULL) != PNG_OK) {
        debugf("Error loading boxart image\n");
    }
}

static void deinit (menu_t *menu) {
    if (boxart_image_loading) {
        png_decode_abort();
    }

    if (boxart_image) {
        surface_free(boxart_image);
        free(boxart_image);
    }
}


void view_file_info_init (menu_t *menu) {
    boxart_image_loading = true;
    boxart_image = NULL;

    char *file_name = menu->browser.list[menu->browser.selected].name;

    path_t *file = path_clone(menu->browser.directory);
    path_push(file, file_name);

    if (f_stat(path_get(file), &info) != FR_OK) {
        menu->next_mode = MENU_MODE_ERROR;
    }

    if (file_has_extensions(file_name, n64_rom_extensions)) {
        rom_header = file_read_rom_header(path_get(file));

        boxart_image_load(rom_header.metadata.unique_identifier);
    }

    path_free(file);
}

void view_file_info_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);

    if (menu->next_mode != MENU_MODE_FILE_INFO) {
        deinit(menu);
    }
}
