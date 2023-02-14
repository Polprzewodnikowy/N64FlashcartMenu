#include <stdio.h>
#include <string.h>

#include <fatfs/ff.h>
#include <libdragon.h>

#include "flashcart/flashcart.h"

#include "menu.h"


static char *config_path = "n64/config.txt";
static char game_path[256];
static char save_path[256];
static flashcart_save_type_t save_type = FLASHCART_SAVE_TYPE_NONE;
static bool save_writeback = false;


static void get_string (char *buffer, size_t size, FIL *fil) {
    UINT br;
    char c;

    for (size_t i = 0; i < size; i++) {
        assertf(f_read(fil, &c, 1, &br) == FR_OK, "Couldn't read string");
        if (br == 0 || c == '\r' || c == '\n') {
            *buffer = '\0';
            return;
        }
        *buffer++ = c;
    }
}

static void load_config (void) {
    FIL fil;
    char buffer[266];

    assertf(f_open(&fil, config_path, FA_READ) == FR_OK, "Couldn't open config file");

    while (!f_eof(&fil)) {
        get_string(buffer, sizeof(buffer), &fil);
        if (strncmp("game_path=", buffer, 10) == 0) {
            strncpy(game_path, (buffer + 10), sizeof(game_path));
        } else if (strncmp("save_path=", buffer, 10) == 0) {
            strncpy(save_path, (buffer + 10), sizeof(save_path));
            save_writeback = true;
        } else if (strncmp("save_type=", buffer, 10) == 0) {
            save_type = (buffer[10] - '0');
            assertf(save_type < __FLASHCART_SAVE_TYPE_END, "Invalid save type in config file");
        }
    }

    assertf(f_close(&fil) == FR_OK, "Couldn't close config file");
}


void menu_restore (menu_t *menu) {
    // TODO: restore last menu state from SD card
}

void menu_run (menu_t *menu) {
    // TODO: implement nice user interface here

    console_init();
    console_set_debug(true);

    load_config();

    printf("N64 Flashcart Menu\n\n");

    printf("Loading ROM: %s\n", game_path);
    assertf(flashcart_load_rom(game_path) == FLASHCART_OK, "ROM load error");

    printf("Loading save: %s, type: %d, writeback: %d\n", save_path, save_type, save_writeback);
    assertf(flashcart_load_save(save_path, save_type, save_writeback) == FLASHCART_OK, "Save load error");

    // TODO: write menu state to SD card

    menu->boot_params.device_type = BOOT_DEVICE_TYPE_ROM;
    menu->boot_params.reset_type = BOOT_RESET_TYPE_NMI;
    menu->boot_params.detect_tv_type = true;
    menu->boot_params.detect_cic_seed = true;
}
