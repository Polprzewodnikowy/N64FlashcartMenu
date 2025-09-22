#include <miniz.h>
#include <miniz_zip.h>
#include "../sound.h"

#include "utils/fs.h"
#include "views.h"


static const char *n64_rom_extensions[] = { "z64", "n64", "v64", "rom", NULL };
static const char *text_extensions[] = { "txt", NULL };
static const char *config_extensions[] = { "ini", "cfg", "yml", "yaml", "toml", NULL };
static const char *save_extensions[] = { "sav", "eep", "eeprom", "sra", "srm", "ram", "fla", "flashram", NULL };
static const char *patch_extensions[] = { "aps", "bps", "ips", "pps", "ups", "xdelta", NULL };
static const char *archive_extensions[] = { "zip", "rar", "7z", "tar", "gz", NULL };
static const char *image_extensions[] = { "png", "jpg", "gif", NULL };
static const char *music_extensions[] = { "mp3", "wav", "ogg", "wma", "flac", NULL };
static const char *controller_pak_extensions[] = { "mpk", "pak", NULL };
static const char *emulator_extensions[] = { "nes", "smc", "gb", "gbc", "sms", "gg", "chf", NULL };
static const char *cheat_extensions[] = {"cht", "cheats", "datel", "gameshark", NULL};


static mz_zip_archive_file_stat st;


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
    } else if (file_has_extensions(name, controller_pak_extensions)) {
        return " Type: Controller Pak file\n";
    } else if (file_has_extensions(name, emulator_extensions)) {
        return " Type: Emulator ROM file\n";
    } else if (file_has_extensions(name, cheat_extensions)) {
        return " Type: Cheats file\n";
    }
    return " Type: Unknown file\n";
}


static size_t mz_zip_file_write_callback(void *pOpaque, mz_uint64 ofs, const void *pBuf, size_t n) {
    float progress = ofs / (float)st.m_uncomp_size;
    surface_t *d = (progress >= 1.0f) ? display_get() : display_try_get();
    n = fwrite(pBuf, 1, n, (FILE *)pOpaque);

    if (d) {
        rdpq_attach(d, NULL);

        ui_components_background_draw();

        ui_components_loader_draw(progress, "Extracting file...");  

        rdpq_detach_show();
    }

    return n;
}

static void extract(menu_t *menu) {
    path_t *path = path_clone(menu->browser.directory);
    path_pop(path);
    path_push(path, st.m_filename);

    path_t *dir = path_clone(path);
    path_pop(dir);

    if (file_exists(path_get(path))) {
        menu->browser.select_file = path_clone(path);
        menu_show_error(menu, "File already exists");
    } else if (directory_create(path_get(dir))) {
        menu_show_error(menu, "Failed to create directory");
    } else {
        FILE *file = fopen(path_get(path), "wb");
        if (file) {
            if (!mz_zip_reader_extract_to_callback(
                &menu->browser.zip,
                st.m_file_index,
                mz_zip_file_write_callback,
                file, 0)) {
                menu_show_error(menu, "Failed to extract file");
            }
            fclose(file);
            menu->browser.select_file = path_clone(path);
            menu->next_mode = MENU_MODE_BROWSER;
        } else {
            menu_show_error(menu, "Failed to create file");
        }
    }

    path_free(path);
    path_free(dir);
}

static void process (menu_t *menu) {
    if (menu->actions.enter && !st.m_is_directory && st.m_is_supported) {
        sound_play_effect(SFX_ENTER);
        menu->boot_pending.extract_file = true;
    } else if (menu->actions.back) {
        sound_play_effect(SFX_EXIT);
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    ui_components_background_draw();

    if (menu->boot_pending.extract_file) {
        ui_components_loader_draw(0.0f, NULL);
    } else {
        ui_components_layout_draw();

        ui_components_main_text_draw(
            STL_DEFAULT,
            ALIGN_CENTER, VALIGN_TOP,
            "ENTRY INFORMATION\n"
            "\n"
            "%s",
            st.m_filename
        );

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
            " CRC32: %08X",
            st.m_uncomp_size, st.m_comp_size,
            st.m_is_directory ? "Directory" : "File",
            st.m_is_encrypted ? "(Encrypted)" : "",
            format_file_type(st.m_filename, st.m_is_directory),
            st.m_crc32
        );

        if (st.m_comment_size > 0) {
            ui_components_main_text_draw(
                STL_DEFAULT,
                ALIGN_LEFT, VALIGN_BOTTOM,
                " Comment:\n"
                "\n"
                "%s",
                st.m_comment
            );
        }

        ui_components_actions_bar_text_draw(
            STL_DEFAULT,
            ALIGN_LEFT, VALIGN_TOP,
            st.m_is_supported ? "A: Extract\nB: Exit" : "\nB: Exit"
        );
    }

    rdpq_detach_show();
}


void view_extract_file_init (menu_t *menu) {
    if (!mz_zip_reader_file_stat(&menu->browser.zip, menu->browser.entry->index, &st)) {
        menu_show_error(menu, "Couldn't obtain archived file information");
    }
}

void view_extract_file_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);

    if (menu->boot_pending.extract_file) {
        menu->boot_pending.extract_file = false;
        extract(menu);
    }
}
