#include <libdragon.h>

#include "boot/boot.h"
#include "flashcart/flashcart.h"
#include "fragments/fragments.h"
#include "views.h"
#include "utils/fs.h"

#ifndef EMULATOR_FOLDER
#define EMULATOR_FOLDER "/emulators/"
#endif

static const char *emu_nes_rom_extensions[] = { "nes", NULL };
static const char *emu_gameboy_rom_extensions[] = { "gb", "gbc", NULL };
static const char *emu_sega_rom_extensions[] = {"smc", "gen", "smd", NULL };

static bool load_pending;


static void load_emulator_nes_rom (path_t *path, menu_t *menu) {
    
    if (file_exists(EMULATOR_FOLDER"emu.nes")) { // || neon64bu.rom
 
        menu->flashcart_error = flashcart_load_rom(EMULATOR_FOLDER"emu.nes", false);
        /* Combine EMU and ROM before loading. See https://github.com/hcs64/neon64v2/tree/master/pkg */
         menu->flashcart_error = flashcart_load_emulator_rom(path_get(path));
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


static void load (menu_t *menu) {
    menu->next_mode = MENU_MODE_BOOT;

    path_t *path = path_clone(menu->browser.directory);
    path_push(path, menu->browser.list[menu->browser.selected].name);

    if (file_has_extensions (path_get(path), emu_nes_rom_extensions)) {
        load_emulator_nes_rom(path, menu);
    }
    else if (file_has_extensions (path_get(path), emu_gameboy_rom_extensions)) {
        //load_emulator_gameboy_rom(path, menu);
    }
    else if (file_has_extensions (path_get(path), emu_sega_rom_extensions)) {
        //load_emulator_sega_rom(path, menu);
    }

    path_free(path);

    menu->boot_params->device_type = BOOT_DEVICE_TYPE_ROM;
    menu->boot_params->reset_type = BOOT_RESET_TYPE_COLD;
    menu->boot_params->tv_type = BOOT_TV_TYPE_PASSTHROUGH;
    menu->boot_params->detect_cic_seed = true;
}


static void process (menu_t *menu) {
    if (menu->actions.enter) {
        load_pending = true;
    } else if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
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

    if (load_pending) {
        const int offset_x = 248;
        const int offset_y = 212;
        const int text_offset_x = -39;

        // Loading screen
        widget_border(offset_x, offset_y, d->width - offset_x, d->height - offset_y, layout->border_thickness);
        fragment_text_start(text_color);
        fragment_textf((d->width / 2) + text_offset_x, (d->height / 2) - (layout->line_height / 2), "Loading…");
    } else {
        // Layout
        fragment_borders(d);

        // Text start
        fragment_text_start(text_color);

        // Main screen
        text_y += fragment_textf(text_x, text_y, "ROM Type: %s", "Nintendo Entertainment System");
        text_y += fragment_textf(text_x, text_y, "Emulator: %s", "Neon64v2");

        // Actions bar
        text_y = layout->actions_y + layout->offset_text_y;
        text_y += fragment_textf(text_x, text_y, "A: Load and run EMU ROM");
        text_y += fragment_textf(text_x, text_y, "B: Exit");
    }

    rdpq_detach_show();
}


void view_load_emulator_init (menu_t *menu) {
    load_pending = false;

    path_t *path = path_clone(menu->browser.directory);
    path_push(path, menu->browser.list[menu->browser.selected].name);

    path_free(path);
}

void view_load_emulator_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
    if (load_pending) {
        load_pending = false;
        load(menu);
    }
}
