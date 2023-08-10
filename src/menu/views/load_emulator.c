#include <libdragon.h>

#include "boot/boot.h"
#include "flashcart/flashcart.h"
#include "utils/fs.h"
#include "views.h"

#ifndef EMULATOR_FOLDER
#define EMULATOR_FOLDER "/emulators/"
#endif

static const char *emu_nes_rom_extensions[] = { "nes", NULL };
static const char *emu_gameboy_rom_extensions[] = { "gb", NULL };
static const char *emu_gameboy_color_rom_extensions[] = { "gbc", NULL };
static const char *emu_sega_rom_extensions[] = {"smc", "gen", "smd", NULL };

static const uint32_t eum_rom_start_address = 0x200000;

static bool load_pending;


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
            "Emulator information\n"
            "THE EMULATOR\n"
            "Rom Name\n"
            "\n"
            "%s",
            menu->browser.entry->name
        );

        component_actions_bar_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "A: Load and run Emulator ROM\n"
            "B: Exit"
        );
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

static void load_emulator_nes_rom (path_t *path, menu_t *menu) {
    if (file_exists(EMULATOR_FOLDER"emu.nes")) { // || neon64bu.rom
        menu->flashcart_error = flashcart_load_rom(EMULATOR_FOLDER"emu.nes", false, draw_progress);
        /* Combine EMU and ROM before loading. See https://github.com/hcs64/neon64v2/tree/master/pkg */
         menu->flashcart_error = flashcart_load_file(path_get(path), eum_rom_start_address);
        if (menu->flashcart_error != FLASHCART_OK) {
            menu->next_mode = MENU_MODE_FAULT;
            path_free(path);
            return;
        }

        path_ext_replace(path, "sav");
        menu->flashcart_error = flashcart_load_save(path_get(path), FLASHCART_SAVE_TYPE_SRAM_BANKED);
        if (menu->flashcart_error != FLASHCART_OK) {
            menu->next_mode = MENU_MODE_FAULT;
            path_free(path);
            return;
        }
    }
}

static void load_emulator_gameboy_rom (path_t *path, menu_t *menu) {
    if (file_exists(EMULATOR_FOLDER"emu.gb")) { // || gb.v64
        menu->flashcart_error = flashcart_load_rom(EMULATOR_FOLDER"emu.gb", false, draw_progress);
        /* Combine EMU and ROM before loading. */
         menu->flashcart_error = flashcart_load_file(path_get(path), eum_rom_start_address);
        if (menu->flashcart_error != FLASHCART_OK) {
            menu->next_mode = MENU_MODE_FAULT;
            path_free(path);
            return;
        }

        path_ext_replace(path, "sav");
        menu->flashcart_error = flashcart_load_save(path_get(path), FLASHCART_SAVE_TYPE_FLASHRAM);
        if (menu->flashcart_error != FLASHCART_OK) {
            menu->next_mode = MENU_MODE_FAULT;
            path_free(path);
            return;
        }
    }
}

static void load_emulator_gameboy_color_rom (path_t *path, menu_t *menu) {
    if (file_exists(EMULATOR_FOLDER"emu.gbc")) { // || gbc.v64
        menu->flashcart_error = flashcart_load_rom(EMULATOR_FOLDER"emu.gbc", false, draw_progress);
        /* Combine EMU and ROM before loading. */
         menu->flashcart_error = flashcart_load_file(path_get(path), eum_rom_start_address);
        if (menu->flashcart_error != FLASHCART_OK) {
            menu->next_mode = MENU_MODE_FAULT;
            path_free(path);
            return;
        }

        path_ext_replace(path, "sav");
        menu->flashcart_error = flashcart_load_save(path_get(path), FLASHCART_SAVE_TYPE_FLASHRAM);
        if (menu->flashcart_error != FLASHCART_OK) {
            menu->next_mode = MENU_MODE_FAULT;
            path_free(path);
            return;
        }
    }
}

static void load (menu_t *menu) {
    menu->next_mode = MENU_MODE_BOOT;

    path_t *path = path_clone_push(menu->browser.directory, menu->browser.entry->name);

    if (file_has_extensions(path_get(path), emu_nes_rom_extensions)) {
        load_emulator_nes_rom(path, menu);
    } else if (file_has_extensions(path_get(path), emu_gameboy_rom_extensions)) {
        load_emulator_gameboy_rom(path, menu);
    } else if (file_has_extensions(path_get(path), emu_gameboy_color_rom_extensions)) {
        load_emulator_gameboy_color_rom(path, menu);
    } else if (file_has_extensions(path_get(path), emu_sega_rom_extensions)) {
        //load_emulator_sega_rom(path, menu);
    }

    path_free(path);

    menu->boot_params->device_type = BOOT_DEVICE_TYPE_ROM;
    menu->boot_params->tv_type = BOOT_TV_TYPE_PASSTHROUGH;
    menu->boot_params->detect_cic_seed = true;
}


void view_load_emulator_init (menu_t *menu) {
    load_pending = false;
}

void view_load_emulator_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);

    if (load_pending) {
        load_pending = false;
        load(menu);
    }
}
