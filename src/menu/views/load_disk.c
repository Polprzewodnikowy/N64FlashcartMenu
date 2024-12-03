#include "../cart_load.h"
#include "../disk_info.h"
#include "boot/boot.h"
#include "../sound.h"
#include "views.h"
#include "../rom_history.h"


static bool load_disk_with_rom;
static component_boxart_t *boxart;
static char* name;

static char *convert_error_message (disk_err_t err) {
    switch (err) {
        case DISK_ERR_IO: return "I/O error during loading 64DD disk information";
        case DISK_ERR_NO_FILE: return "Couldn't open 64DD disk file";
        case DISK_ERR_INVALID: return "Invalid 64DD disk file";
        default: return "Unknown disk info load error";
    }
}

static char *format_disk_region (disk_region_t region) {
    switch (region) {
        case DISK_REGION_DEVELOPMENT: return "Development";
        case DISK_REGION_JAPANESE: return "Japan";
        case DISK_REGION_USA: return "USA";
        default: return "Unknown";
    }
}


static void process (menu_t *menu) {
    if (menu->actions.enter) {
        menu->boot_pending.disk_file = true;
        load_disk_with_rom = false;
    } else if (menu->actions.options && menu->load.rom_path) {
        menu->boot_pending.disk_file = true;
        load_disk_with_rom = true;
        sound_play_effect(SFX_SETTING);
    } else if (menu->actions.back) {
        sound_play_effect(SFX_EXIT);
        menu->next_mode = MENU_MODE_BROWSER;
    } else if (menu->actions.favorite) {
        history_favorite_add(&menu->history, menu->load.rom_path, menu->load.disk_path);
        sound_play_effect(SFX_SETTING);
    }
}

static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    ui_components_background_draw();

    if (menu->boot_pending.disk_file) {
        ui_components_loader_draw(0.0f);
    } else {
        ui_components_layout_draw();

        ui_components_main_text_draw(
            ALIGN_CENTER, VALIGN_TOP,
            "64DD disk information\n"
            "\n"
            "%s",
            name
        );

        ui_components_main_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "\n"
            "\n"
            "\n"
            "\n"
            " Region: %s\n"
            " Unique ID: %.4s\n"
            " Version: %hhu\n"
            " Disk type: %d\n"
            "\n"
            " %s%s",
            format_disk_region(menu->load.disk_info.region),
            menu->load.disk_info.id,
            menu->load.disk_info.version,
            menu->load.disk_info.disk_type,
            menu->load.rom_path ? "ROM: " : "",
            menu->load.rom_path ? path_last_get(menu->load.rom_path) : ""
        );

        ui_components_actions_bar_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "A: Load and run 64DD disk\n"
            "B: Exit"
        );

        ui_components_actions_bar_text_draw(
            ALIGN_CENTER, VALIGN_TOP,
            "\n"
            "C>: Favorite"
        );

        if (menu->load.rom_path) {
            ui_components_actions_bar_text_draw(
                ALIGN_RIGHT, VALIGN_TOP,
                "R: Load with ROM"
            );
        }

        if (boxart != NULL) {
            ui_components_boxart_draw(boxart);
        }
    }

    rdpq_detach_show();
}

static void draw_progress (float progress) {
    surface_t *d = (progress >= 1.0f) ? display_get() : display_try_get();

    if (d) {
        rdpq_attach(d, NULL);

        ui_components_background_draw();

        ui_components_loader_draw(progress);

        rdpq_detach_show();
    }
}

static void load (menu_t *menu) {
    cart_load_err_t err;

    if (menu->load.rom_path && load_disk_with_rom) {
        err = cart_load_n64_rom_and_save(menu, draw_progress);
        if (err != CART_LOAD_OK) {
            menu_show_error(menu, cart_load_convert_error_message(err));
            return;
        }
    }

    err = cart_load_64dd_ipl_and_disk(menu, draw_progress);
    if (err != CART_LOAD_OK) {
        menu_show_error(menu, cart_load_convert_error_message(err));
        return;
    }

    history_last_rom_set(&menu->history, menu->load.disk_path, menu->load.rom_path);
    menu->next_mode = MENU_MODE_BOOT;

    if (load_disk_with_rom) {
        menu->boot_params->device_type = BOOT_DEVICE_TYPE_ROM;
        menu->boot_params->detect_cic_seed = rom_info_get_cic_seed(&menu->load.rom_info, &menu->boot_params->cic_seed);
        switch (rom_info_get_tv_type(&menu->load.rom_info)) {
            case ROM_TV_TYPE_PAL: menu->boot_params->tv_type = BOOT_TV_TYPE_PAL; break;
            case ROM_TV_TYPE_NTSC: menu->boot_params->tv_type = BOOT_TV_TYPE_NTSC; break;
            case ROM_TV_TYPE_MPAL: menu->boot_params->tv_type = BOOT_TV_TYPE_MPAL; break;
            default: menu->boot_params->tv_type = BOOT_TV_TYPE_PASSTHROUGH; break;
        }
        menu->boot_params->cheat_list = NULL;
    } else {
        menu->boot_params->device_type = BOOT_DEVICE_TYPE_64DD;
        menu->boot_params->tv_type = BOOT_TV_TYPE_NTSC;
        menu->boot_params->detect_cic_seed = true;
        menu->boot_params->cheat_list = NULL;
    }
}

static void deinit (void) {
    ui_components_boxart_free(boxart);
}

static bool load_rom(menu_t* menu, path_t* rom_path) {
    if(path_has_value(rom_path)) {
        if (menu->load.rom_path) {
            path_free(menu->load.rom_path);
            menu->load.rom_path = NULL;
        }

        menu->load.rom_path = path_clone(rom_path);

        rom_err_t err = rom_info_load(rom_path, &menu->load.rom_info);
        if (err != ROM_OK) {
            path_free(menu->load.rom_path);
            menu->load.rom_path = NULL;
            menu_show_error(menu, convert_error_message(err));
            return false;
        }        
    }

    return true;
}

void view_load_disk_init (menu_t *menu) {
    if (menu->load.disk_path) {
        path_free(menu->load.disk_path);
        menu->load.disk_path = NULL;
    }

    menu->boot_pending.disk_file = false;

    if(menu->load.load_last) {
        menu->load.disk_path = path_clone(menu->history.last_disk);

        if(!load_rom(menu, menu->history.last_rom)) {
            return;
        }
    } else if(menu->load.load_favorite != -1) {
        menu->load.disk_path = path_clone(menu->history.favorites_disk[menu->load.load_favorite]);

        if(!load_rom(menu, menu->history.favorites_rom[menu->load.load_favorite])) {
            return;
        }
    } else {
        menu->load.disk_path = path_clone_push(menu->browser.directory, menu->browser.entry->name);            
    }

    menu->load.load_favorite = -1;
    menu->load.load_last = false;

    name = path_last_get(menu->load.disk_path);
    disk_err_t err = disk_info_load(menu->load.disk_path, &menu->load.disk_info);
    if (err != DISK_OK) {
        //snprintf(error, 256, "path : '%s' error: '%s'", path_get(menu->load.disk_path), convert_error_message(err));
        menu_show_error(menu, convert_error_message(err));
        return;
    }

    boxart = ui_components_boxart_init(menu->storage_prefix, menu->load.disk_info.id, IMAGE_BOXART_FRONT);
}

void view_load_disk_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);

    if (menu->boot_pending.disk_file) {
        menu->boot_pending.disk_file = false;
        load(menu);
    }

    if (menu->next_mode != MENU_MODE_LOAD_DISK) {
        deinit();
    }
}
