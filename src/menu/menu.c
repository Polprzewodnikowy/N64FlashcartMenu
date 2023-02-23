#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <libdragon.h>
#include <fatfs/ff.h>

#include "flashcart/flashcart.h"
#include "libs/toml/toml.h"

#include "menu.h"


#define SC64_CONFIG_FILEPATH "sd://config.sc64.toml.txt"
static toml_datum_t rom_path;
static toml_datum_t save_path;
static flashcart_save_type_t save_type = FLASHCART_SAVE_TYPE_NONE;
static bool save_writeback = false;
static bool auto_load_last_rom = false;


static void load_config (void) {
    FILE *fp = NULL;
    char error_buffer[266];
    
    printf("Loading config file %s\n", SC64_CONFIG_FILEPATH);
    wait_ms(1000);

    fp = fopen(SC64_CONFIG_FILEPATH, "r");
	if (!fp) {
		printf("Error loading config file %s\n", SC64_CONFIG_FILEPATH);
        wait_ms(10000);
        assertf(!fp, "Couldn't open toml config file: %s", SC64_CONFIG_FILEPATH);

        // TODO: generate a default config file.
	}

    toml_table_t* conf = toml_parse_file(fp, error_buffer, sizeof(error_buffer));
    if (!conf) {
		printf("Error parsing config: %s\n", error_buffer);
        wait_ms(10000);
        //assertf(!conf, "Couldn't parse toml config: %s", error_buffer);
	}

    fclose(fp);
    //assertf(!fclose(fp), "Couldn't close toml config file");
    fp = NULL;

    toml_table_t* last_rom = toml_table_in(conf, "last_rom");
    if (!last_rom) {
		printf("Missing '[last_rom]' header in config\n");
        wait_ms(10000);
        //assertf(!last_rom, "Missing '[last_rom]' header in config");
	}

    rom_path = toml_string_in(last_rom, "rom_path");
    if (!rom_path.ok) {
		printf("Couldn't read 'rom_path' value in config\n");
        wait_ms(10000);
        //assertf(!rom_path.ok, "Couldn't read 'rom_path' value in config\n");
	}
    else {
        printf("Found rom path: %s\n", rom_path.u.s );
    }

    save_path = toml_string_in(last_rom, "save_path");
    if (!save_path.ok) {
		printf("Couldn't read 'save_path' value in config\n");
        wait_ms(10000);
        //assertf(!save_path.ok, "Couldn't read 'save_path' value in config");
	}
    else {
        printf("Found save path: %s\n", save_path.u.s );
    }

    toml_datum_t tmp_save_type = toml_int_in(last_rom, "save_type");
    if (!tmp_save_type.ok) {
		printf("Couldn't read 'save_type' value in config\n");
        wait_ms(10000);
        //assertf(!tmp_save_type.ok, "Couldn't read 'save_type' int value in config");
	}
    else {
        printf("Found save type: %d\n", (int)tmp_save_type.u.i );
    }
    assertf((int)tmp_save_type.u.i < __FLASHCART_SAVE_TYPE_END, "Invalid save type in config file");
    save_type = (int)tmp_save_type.u.i;

    toml_datum_t tmp_auto_load_last_rom = toml_bool_in(last_rom, "auto_load");
    if (!tmp_auto_load_last_rom.ok) {
		printf("Couldn't read 'auto_load' value in config\n");
        wait_ms(5000);
	}
    else {
        printf("Found autoload: %s\n", tmp_auto_load_last_rom.u.b ? "true" : "false");
        auto_load_last_rom = tmp_auto_load_last_rom.u.b;
    }

    toml_free(conf);

}

void menu_restore (menu_t *menu) {
    // TODO: restore last menu state from SD card
}

FRESULT scan_files (
    char* path        /* Start node to be scanned (***also used as work area***) */
)
{
    FRESULT res;
    DIR dir;
    UINT i;
    static FILINFO fno;


    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
                i = strlen(path);
                sprintf(&path[i], "/%s", fno.fname);
                res = scan_files(path);                    /* Enter the directory */
                if (res != FR_OK) break;
                path[i] = 0;
            } else {                                       /* It is a file. */
                printf("%s/%s\n", path, fno.fname);
            }
        }
        f_closedir(&dir);
    }

    return res;
}

void menu_run (menu_t *menu) {
    // TODO: implement nice user interface here

    console_init();
    console_set_debug(true);

    load_config();

    if (auto_load_last_rom) { // TODO: check if there is a button input to cancel.

        printf("Loading last ROM: %s\n", rom_path.u.s);
        assertf(flashcart_load_rom(rom_path.u.s) == FLASHCART_OK, "ROM load error");

        printf("Loading save: %s, type: %d, writeback: %d\n", save_path.u.s, save_type, save_writeback);
        assertf(flashcart_load_save(save_path.u.s, save_type, save_writeback) == FLASHCART_OK, "Save load error");
    }
    else {
        printf("N64 Flashcart Menu\n\n");
        printf("File list:\n");
        FRESULT res;
        char buff[256];
        strcpy(buff, "/");
        res = scan_files(buff);
    
        // TODO: wait for a key input
    }
    // TODO: write menu state to SD card

    menu->boot_params.device_type = BOOT_DEVICE_TYPE_ROM;
    menu->boot_params.reset_type = BOOT_RESET_TYPE_NMI;
    menu->boot_params.detect_tv_type = true;
    menu->boot_params.detect_cic_seed = true;
}
