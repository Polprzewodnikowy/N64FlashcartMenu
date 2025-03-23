#include "../cart_load.h"
#include "../rom_patch_info.h"
#include "boot/boot.h"
#include "views.h"


static bool load_pending;
static bool load_rom;


static char *convert_error_message (rom_patch_load_err_t err) {
    switch (err) {
        case PATCH_ERR_IO: return "I/O error during loading patch file information";
        case PATCH_ERR_NO_FILE: return "Couldn't open patch file";
        case PATCH_ERR_INVALID: return "Invalid patch file";
        case PATCH_ERR_UNSUPPORTED: return "The patch type is not (yet) supported";
        default: return "Unknown patch info load error";
    }
}


static void process (menu_t *menu) {
    if (menu->actions.enter) {
        load_pending = true;
        load_rom = false;
    } else if (menu->actions.options && menu->load.rom_path) {
        load_pending = true;
        load_rom = true;
    } else if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    ui_component_background_draw();

    if (load_pending) {
        ui_component_loader_draw(0.0f);
    } else {
        ui_component_layout_draw();

        ui_component_main_text_draw(
            ALIGN_CENTER, VALIGN_TOP,
            "Patch information\n"
            "\n"
            "%s",
            menu->browser.entry->name
        );

        ui_component_main_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "\n"
            "\n"
            "\n"
            " %s%s",
            menu->load.rom_path ? "ROM: " : "",
            menu->load.rom_path ? path_last_get(menu->load.rom_path) : ""
        );

        ui_component_actions_bar_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "A: Load and run patched ROM\n"
            "B: Exit"
        );

        if (menu->load.rom_path) {
            ui_component_actions_bar_text_draw(
                ALIGN_RIGHT, VALIGN_TOP,
                "R: Load with ROM"
            );
        }
    }

    rdpq_detach_show();
}

static void draw_progress (float progress) {
    surface_t *d = (progress >= 1.0f) ? display_get() : display_try_get();

    if (d) {
        rdpq_attach(d, NULL);

        ui_component_background_draw();

        ui_component_loader_draw(progress);

        rdpq_detach_show();
    }
}

static void load (menu_t *menu) {
    cart_load_err_t err;

    if (menu->load.rom_path && load_rom) {
        err = cart_load_n64_rom_and_save(menu, draw_progress);
        if (err != CART_LOAD_OK) {
            menu_show_error(menu, cart_load_convert_error_message(err));
            return;
        }
    }

    // err = cart_load_rom_and_patch(menu, draw_progress);
    // if (err != CART_LOAD_OK) {
    //     menu_show_error(menu, cart_load_convert_error_message(err));
    //     return;
    // }

    menu->next_mode = MENU_MODE_BOOT;

    if (load_rom) {
        menu->boot_params->device_type = BOOT_DEVICE_TYPE_ROM;
        menu->boot_params->detect_cic_seed = rom_info_get_cic_seed(&menu->load.rom_info, &menu->boot_params->cic_seed);
        switch (rom_info_get_tv_type(&menu->load.rom_info)) {
            case ROM_TV_TYPE_PAL: menu->boot_params->tv_type = BOOT_TV_TYPE_PAL; break;
            case ROM_TV_TYPE_NTSC: menu->boot_params->tv_type = BOOT_TV_TYPE_NTSC; break;
            case ROM_TV_TYPE_MPAL: menu->boot_params->tv_type = BOOT_TV_TYPE_MPAL; break;
            default: menu->boot_params->tv_type = BOOT_TV_TYPE_PASSTHROUGH; break;
        }
    } else {
        menu->boot_params->device_type = BOOT_DEVICE_TYPE_ROM;
        menu->boot_params->tv_type = BOOT_TV_TYPE_NTSC;
        menu->boot_params->detect_cic_seed = true;
    }
}


void view_load_rom_patch_init (menu_t *menu) {
    if (menu->load.rom_patch_path) {
        path_free(menu->load.rom_patch_path);
        menu->load.rom_patch_path = NULL;
    }

    load_pending = false;

    menu->load.rom_patch_path = path_clone_push(menu->browser.directory, menu->browser.entry->name);

    // rom_patch_load_err_t err = patch_info_load(path_get(menu->load.rom_patch_path), &menu->load.patch_info);
    // if (err != PATCH_OK) {
    //     menu_show_error(menu, convert_error_message(err));
    // }
}

void view_load_rom_patch_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);

    if (load_pending) {
        load_pending = false;
        load(menu);
    }
}
