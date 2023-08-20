#include <fatfs/ff.h>

#include "utils/fs.h"
#include "views.h"


static const char *n64_rom_extensions[] = { "z64", "n64", "v64", "rom", NULL };
static const char *text_extensions[] = { "txt", NULL };
static const char *config_extensions[] = { "ini", "cfg", "yml", "yaml", "toml", NULL };
static const char *save_extensions[] = { "sav", "eep", "eeprom", "sra", "srm", "ram", "fla", "flashram", NULL };
static const char *patch_extensions[] = { "ips", "aps", "pps", "xdelta", NULL };
static const char *archive_extensions[] = { "zip", "rar", "7z", "tar", "gz", NULL };
static const char *image_extensions[] = { "png", "jpg", "gif", NULL };
static const char *music_extensions[] = { "mp3", "wav", "ogg", "wma", "flac", NULL };
static const char *dexdrive_extensions[] = { "mpk", NULL };
static const char *emulator_extensions[] = { "emu", NULL };


static FILINFO info;


static char *format_file_type (char *name, bool is_directory) {
    if (is_directory) {
        return "";
    } if (file_has_extensions(name, n64_rom_extensions)) {
        return " Type: N64 ROM\n";
    } else if (file_has_extensions(name, text_extensions)) {
        return " Type: Text file\n";
    } else if (file_has_extensions(name, config_extensions)) {
        return " Type: Config file\n";
    } else if (file_has_extensions(name, save_extensions)) {
        return " Type: N64 save\n";
    } else if (file_has_extensions(name, patch_extensions)) {
        return " Type: ROM patch\n";
    } else if (file_has_extensions(name, archive_extensions)) {
        return " Type: Archive\n";
    } else if (file_has_extensions(name, image_extensions)) {
        return " Type: Image file\n";
    } else if (file_has_extensions(name, music_extensions)) {
        return " Type: Music file\n";
    } else if (file_has_extensions(name, dexdrive_extensions)) {
        return " Type: DexDrive CPak backup file\n";
    } else if (file_has_extensions(name, emulator_extensions)) {
        return " Type: Emulator file\n";
    }
    return " Type: Unknown file\n";
}


static void process (menu_t *menu) {
    if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    component_background_draw();

    component_layout_draw();

    component_main_text_draw(
        ALIGN_CENTER, VALIGN_TOP,
        "ENTRY INFORMATION\n"
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
        " Size: %d bytes\n"
        " Attributes: %s%s%s%s%s\n"
        "%s"
        " Modified: %u-%02u-%02u %02u:%02u",
        info.fsize,
        (info.fattrib & AM_DIR) ? "Directory " : "File ",
        (info.fattrib & AM_RDO) ? "| Read only " : "",
        (info.fattrib & AM_SYS) ? "| System " : "",
        (info.fattrib & AM_ARC) ? "| Archive " : "",
        (info.fattrib & AM_HID) ? "| Hidden " : "",
        format_file_type(info.fname, info.fattrib & AM_DIR),
        (info.fdate >> 9) + 1980, info.fdate >> 5 & 0x0F, info.fdate & 0x1F, info.ftime >> 11, info.ftime >> 5 & 0x3F
    );

    component_actions_bar_text_draw(
        ALIGN_LEFT, VALIGN_TOP,
        "\n"
        "B: Exit"
    );

    rdpq_detach_show();
}


void view_file_info_init (menu_t *menu) {
    path_t *path = path_clone_push(menu->browser.directory, menu->browser.entry->name);

    if (f_stat(strip_sd_prefix(path_get(path)), &info) != FR_OK) {
        menu_show_error(menu, "Couldn't obtain file information");
    }

    path_free(path);
}

void view_file_info_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);
}
