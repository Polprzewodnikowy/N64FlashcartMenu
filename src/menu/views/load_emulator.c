#include <libdragon.h>

#include "boot/boot.h"
#include "flashcart/flashcart.h"
#include "fragments/fragments.h"
#include "views.h"

#ifndef EMULATOR_FOLDER
#define EMULATOR_FOLDER "/emulators/"
#endif

static bool load_pending;



static void load (menu_t *menu) {
    menu->next_mode = MENU_MODE_BOOT;

    path_t *path = path_clone(menu->browser.directory);
    path_push(path, menu->browser.list[menu->browser.selected].name);

    // TODO: this is only correct type for https://github.com/hcs64/neon64v2 NES emu!

    // FIXME: Check if the emulator exists in the folder...

    // FIXME: Combine EMU and ROM before loading?!

    // bool byte_swap = (rom_header.endian == ROM_MID_BIG_ENDIAN);
    // menu->flashcart_error = flashcart_load_rom(path_get(path), byte_swap);
    // if (menu->flashcart_error != FLASHCART_OK) {
    //     menu->next_mode = MENU_MODE_FAULT;
    //     path_free(path);
    //     return;
    // }

    uint8_t emu_save_type = FLASHCART_SAVE_TYPE_SRAM_BANKED;

    path_ext_replace(path, "sav");
    menu->flashcart_error = flashcart_load_save(path_get(path), emu_save_type);
    if (menu->flashcart_error != FLASHCART_OK) {
        menu->next_mode = MENU_MODE_FAULT;
        path_free(path);
        return;
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

        // // Main screen
        text_y += fragment_textf(text_x, text_y, "ROM Type: %s", "Nintendo Entertainment System");
        text_y += fragment_textf(text_x, text_y, "Emulator: %s", "Neon64v2");

        // Actions bar
        text_y = layout->actions_y + layout->offset_text_y;
        text_y += fragment_textf(text_x, text_y, "A: Load and run ROM");
        text_y += fragment_textf(text_x, text_y, "B: Exit");
    }

    rdpq_detach_show();
}


void view_load_emulator_init (menu_t *menu) {
    load_pending = false;

    path_t *path = path_clone(menu->browser.directory);
    path_push(path, menu->browser.list[menu->browser.selected].name);

    // rom_header = file_read_rom_header(path_get(path));

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
