#include "../cart_load.h"
#include "boot/boot.h"
#include "utils/fs.h"
#include "views.h"


static const char *emu_nes_rom_extensions[] = { "nes", NULL };
static const char *emu_snes_rom_extensions[] = { "sfc", "smc", NULL };
static const char *emu_gameboy_rom_extensions[] = { "gb", NULL };
static const char *emu_gameboy_color_rom_extensions[] = { "gbc", NULL };
static const char *emu_sega_8bit_rom_extensions[] = { "sms", "gg", "sg", NULL };

static bool load_pending;
static cart_load_emu_type_t emu_type;

static char *format_emulator_name (cart_load_emu_type_t emulator_info) {
    switch (emulator_info) {
        case CART_LOAD_EMU_TYPE_NES:
            return "Nintendo Famicom (NES)";
        case CART_LOAD_EMU_TYPE_SNES:
            return "Nintendo Super Famicom (SNES)";
        case CART_LOAD_EMU_TYPE_GAMEBOY:
            return "Nintendo GAMEBOY";
        case CART_LOAD_EMU_TYPE_GAMEBOY_COLOR:
            return "Nintendo GAMEBOY Color";
        case CART_LOAD_EMU_TYPE_SEGA_GENERIC_8BIT:
            return "SEGA 8bit system";
        default:
            return "Unknown";
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
            "Load Emulated ROM\n"
        );

        component_main_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "\n"
            "\n"
            "Emulated System: %s\n"
            "Rom Name: %s",
            format_emulator_name(emu_type),
            menu->browser.entry->name
        );

        component_actions_bar_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "A: Load and run Emulated ROM\n"
            "B: Exit"
        );
    }

    rdpq_detach_show();
}

static void draw_progress (float progress) {
    surface_t *d = (progress >= 1.0f) ? display_get() : display_try_get();

    if (d) {
        rdpq_attach(d, NULL);

        component_background_draw();

        component_loader_draw(progress);

        rdpq_detach_show();
    }
}

static void load (menu_t *menu) {
    cart_load_err_t err = cart_load_emulator(menu, emu_type, draw_progress);

    if (err != CART_LOAD_OK) {
        menu_show_error(menu, cart_load_convert_error_message(err));
        return;
    }

    menu->next_mode = MENU_MODE_BOOT;
    menu->boot_params->device_type = BOOT_DEVICE_TYPE_ROM;
    menu->boot_params->tv_type = BOOT_TV_TYPE_PASSTHROUGH;
    menu->boot_params->detect_cic_seed = true;
}


void view_load_emulator_init (menu_t *menu) {
    load_pending = false;

    path_t *path = path_clone_push(menu->browser.directory, menu->browser.entry->name);

    if (file_has_extensions(path_get(path), emu_nes_rom_extensions)) {
        emu_type = CART_LOAD_EMU_TYPE_NES;
    } else if (file_has_extensions(path_get(path), emu_snes_rom_extensions)) {
        emu_type = CART_LOAD_EMU_TYPE_SNES;
    } else if (file_has_extensions(path_get(path), emu_gameboy_rom_extensions)) {
        emu_type = CART_LOAD_EMU_TYPE_GAMEBOY;
    } else if (file_has_extensions(path_get(path), emu_gameboy_color_rom_extensions)) {
        emu_type = CART_LOAD_EMU_TYPE_GAMEBOY_COLOR;
    } else if (file_has_extensions(path_get(path), emu_sega_8bit_rom_extensions)) {
        emu_type = CART_LOAD_EMU_TYPE_SEGA_GENERIC_8BIT;
    } else {
        menu_show_error(menu, "Unsupported ROM");
    }

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
