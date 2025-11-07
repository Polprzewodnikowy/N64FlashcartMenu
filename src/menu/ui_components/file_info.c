/**
 * @file file_info.c
 * @brief Implementation of the file information UI component.
 * @ingroup ui_components
 */

#include <stdlib.h>

#include "../ui_components.h"
#include "../utils/fs.h"
#include "../fonts.h"
#include "constants.h"

static const char *n64_rom_extensions[] = { "z64", "n64", "v64", "rom", NULL };
static const char *text_extensions[] = { "txt", NULL };
static const char *config_extensions[] = { "ini", "cfg", "yml", "yaml", "toml", NULL };
static const char *save_extensions[] = { "sav", "eep", "eeprom", "sra", "srm", "ram", "fla", "flashram", NULL };
static const char *patch_extensions[] = { "aps", "bps", "ips", "pps", "ups", "xdelta", NULL };
static const char *archive_extensions[] = { "zip", "rar", "7z", "tar", "gz", NULL };
static const char *image_extensions[] = { "png", "jpg", "gif", NULL };
static const char *music_extensions[] = { "mp3", "wav", "ogg", "wma", "flac", NULL };
static const char *controller_pak_extensions[] = { "mpk", "pak", NULL };
static const char *controller_pak_note_extensions[] = { "mpkn", "paknote", NULL };
static const char *emulator_extensions[] = { "nes", "smc", "gb", "gbc", "sms", "gg", "chf", NULL };
static const char *cheat_extensions[] = {"cht", "cheats", "datel", "gameshark", NULL};

/**
 * @brief Format the file extension into a human-readable file type string.
 *
 * @param name The filename including the extension.
 * @param info Pointer to the file information structure (used for flags).
 * @return Constant string describing the file type.
 */
static const char *format_file_type(char *name, file_info_t *info) {
    if (info->directory) {
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
    } else if (file_has_extensions(name, controller_pak_extensions)) {
        info->is_controller_pak_dump = true;
        return " Type: Controller Pak file\n";
    } else if (file_has_extensions(name, controller_pak_note_extensions)) {
        info->is_controller_pak_dump_note = true;
        return " Type: Controller Pak note file\n";
    } 
    else if (file_has_extensions(name, emulator_extensions)) {
        return " Type: Emulator ROM file\n";
    } else if (file_has_extensions(name, cheat_extensions)) {
        return " Type: Cheats file\n";
    }
    return " Type: Unknown file\n";
}

/**
 * @brief Draw the file information UI component.
 *
 * @param filename Name of the file to display information for.
 * @param info Pointer to the file information structure.
 */
void ui_components_file_info_draw(char* filename, file_info_t *info) {
    if (!info) {
        return;
    }

    ui_components_main_text_draw(
        STL_DEFAULT,
        ALIGN_CENTER, VALIGN_TOP,
        "ENTRY INFORMATION\n"
        "\n"
        "%s",
        filename
    );

    const char *file_type = format_file_type(filename, info);
    const char *file_mode = info->directory ? "Directory" : "File";
    const char *file_access = info->encrypted ? "(Encrypted)" : info->writeable ? "" : "(Read only)";
    if (info->compressed > 0) {
        ui_components_main_text_draw(
            STL_DEFAULT,
            ALIGN_LEFT, VALIGN_TOP,
            "\n"
            "\n"
            "\n"
            "\n"
            " Size: %d bytes\n"
            " Compressed: %d bytes\n"
            " Attributes: %s %s\n"
            "%s"
            " Modified: %s"
            " CRC32: %08X",
            info->size,
            info->compressed,
            file_mode,
            file_access,
            file_type,
            ctime(&info->mtime),
            info->crc32
        );
    } else {
        ui_components_main_text_draw(
            STL_DEFAULT,
            ALIGN_LEFT, VALIGN_TOP,
            "\n"
            "\n"
            "\n"
            "\n"
            " Size: %d bytes\n"
            " Attributes: %s %s\n"
            "%s"
            " Modified: %s",
            info->size,
            file_mode,
            file_access,
            file_type,
            ctime(&info->mtime)
        );
    }
}
