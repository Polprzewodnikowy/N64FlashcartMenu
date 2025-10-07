#include "../cart_load.h"
#include "boot/boot.h"
#include "utils/fs.h"
#include "../sound.h"
#include "views.h"


static const char *emu_nes_rom_extensions[] = { "nes", NULL };
static const char *emu_snes_rom_extensions[] = { "sfc", "smc", NULL };
static const char *emu_gameboy_rom_extensions[] = { "gb", NULL };
static const char *emu_gameboy_color_rom_extensions[] = { "gbc", NULL };
static const char *emu_sega_8bit_rom_extensions[] = { "sms", "gg", "sg", NULL };
static const char *emu_fairchild_channelf_rom_extensions[] = { "chf", NULL };

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
        case CART_LOAD_EMU_TYPE_FAIRCHILD_CHANNELF:
            return "Fairchild Channel F";
        default:
            return "Unknown";
    }
}


/**
 * Process input actions for the load-emulator menu.
 *
 * When the Enter action is active, marks an emulated ROM load as pending.
 * When the Back action is active, plays the exit sound and requests a switch
 * to the browser menu mode.
 *
 * @param menu Pointer to the current menu context used to read actions and
 *             update pending state and next_mode. Must not be NULL.
 */
static void process (menu_t *menu) {
    if (menu->actions.enter) {
        menu->load_pending.emulator_file = true;
    } else if (menu->actions.back) {
        sound_play_effect(SFX_EXIT);
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

/**
 * Render the "Load Emulated ROM" view to the provided drawing surface.
 *
 * If a ROM load is pending (menu->load_pending.emulator_file), renders the loader UI;
 * otherwise renders the main layout including the centered title, the emulated system
 * and ROM name, and the actions bar.
 *
 * @param menu Pointer to the menu state; used for load_pending flag and browser entry name.
 * @param d Surface to render into.
 */
static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    ui_components_background_draw();

    if (menu->load_pending.emulator_file) {
        ui_components_loader_draw(0.0f, NULL);
    } else {
        ui_components_layout_draw();

        ui_components_main_text_draw(
            STL_DEFAULT,
            ALIGN_CENTER, VALIGN_TOP,
            "Load Emulated ROM\n"
        );

        ui_components_main_text_draw(
            STL_DEFAULT,
            ALIGN_LEFT, VALIGN_TOP,
            "\n"
            "\n"
            "Emulated System: %s\n"
            "Rom Name: %s",
            format_emulator_name(emu_type),
            menu->browser.entry->name
        );

        ui_components_actions_bar_text_draw(
            STL_DEFAULT,
            ALIGN_LEFT, VALIGN_TOP,
            "A: Load and run Emulated ROM\n"
            "B: Exit"
        );
    }

    rdpq_detach_show();
}

/**
 * Render the emulated-ROM loading UI with a progress indicator.
 *
 * If a display is available, draws the background and a loader bar labeled
 * "Loading emulated ROM..." reflecting the given progress.
 *
 * @param progress Progress value from 0.0 to 1.0; values greater than or equal
 *                 to 1.0 are treated as complete.
 */
static void draw_progress (float progress) {
    surface_t *d = (progress >= 1.0f) ? display_get() : display_try_get();

    if (d) {
        rdpq_attach(d, NULL);

        ui_components_background_draw();

        ui_components_loader_draw(progress, "Loading emulated ROM...");

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
    menu->boot_params->cheat_list = NULL;
}


/**
 * Initialize emulator-load state and select the emulator type based on the selected file's extension.
 *
 * Initializes the menu's load_pending.emulator_file flag and inspects the currently selected browser
 * entry's filename to determine and set the global `emu_type` to the matching emulator (NES, SNES,
 * Game Boy, Game Boy Color, SEGA 8-bit, or Fairchild Channel F). If the file extension is not
 * recognized, reports "Unsupported ROM" via the menu error display.
 *
 * @param menu Menu context whose browser entry is inspected and whose load_pending flag is initialized.
 */
void view_load_emulator_init (menu_t *menu) {
    menu->load_pending.emulator_file = false;

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
    } else if (file_has_extensions(path_get(path), emu_fairchild_channelf_rom_extensions)) {
        emu_type = CART_LOAD_EMU_TYPE_FAIRCHILD_CHANNELF;
    } else {
        menu_show_error(menu, "Unsupported ROM");
    }

    path_free(path);
}

/**
 * Update and render the Load Emulated ROM view; if a ROM load is pending, clear the pending
 * flag and begin loading the ROM.
 *
 * Processes input state, draws the view to the provided display surface, and when
 * menu->load_pending.emulator_file is set, resets that flag and invokes the loader.
 *
 * @param menu Current menu state used for input, UI state, and initiating the load.
 * @param display Surface to render the view onto.
 */
void view_load_emulator_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);

    if (menu->load_pending.emulator_file) {
        menu->load_pending.emulator_file = false;
        load(menu);
    }
}