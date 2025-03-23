#include "../cart_load.h"
#include "../disk_info.h"
#include "boot/boot.h"
#include "../sound.h"
#include "views.h"
#include "../bookkeeping.h"

static component_boxart_t *boxart;
static char *disk_filename;

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


static void add_favorite (menu_t *menu, void *arg) {
    bookkeeping_favorite_add(&menu->bookkeeping, menu->load.disk_path, menu->load.rom_path, BOOKKEEPING_TYPE_DISK);
}

static component_context_menu_t options_context_menu = { .list = {
    { .text = "Add to favorites", .action = add_favorite },
    COMPONENT_CONTEXT_MENU_LIST_END,
}};


static void process (menu_t *menu) {
    if (ui_components_context_menu_process(menu, &options_context_menu)) {
        return;
    }

    if (menu->actions.enter) {
        menu->boot_pending.disk_file = true;
        menu->load.combined_disk_rom = false;
    } else if (menu->actions.lz_context && menu->load.rom_path) {
        menu->boot_pending.disk_file = true;
        menu->load.combined_disk_rom = true;
        sound_play_effect(SFX_SETTING);
    } else if (menu->actions.back) {
        sound_play_effect(SFX_EXIT);
        menu->next_mode = MENU_MODE_BROWSER;
    } else if (menu->actions.options) {
        ui_components_context_menu_show(&options_context_menu);
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
            disk_filename
        );

        ui_components_main_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "\n\n\n\n"
            "%s%s\n",
            menu->load.rom_path ? "Loaded ROM:\t" : "",
            menu->load.rom_path ? path_last_get(menu->load.rom_path) : ""
        );

        ui_components_main_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "\n\n\n\n\n\n"
            "Description:\n\t%s\n",
            "None."
        );

        ui_components_main_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "\n\n\n\n\n\n\n\n\n\n\n\n"
            " Region:\t\t%s\n"
            " Unique ID:\t%.4s\n"
            " Version:\t%hhu\n"
            " Disk type:\t%d\n"
            "\n"
            ,
            format_disk_region(menu->load.disk_info.region),
            menu->load.disk_info.id,
            menu->load.disk_info.version,
            menu->load.disk_info.disk_type
        );

        ui_components_actions_bar_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "A: Load and run 64DD disk\n"
            "B: Exit\n"
        );

        if (menu->load.rom_path) {
            ui_components_actions_bar_text_draw(
                ALIGN_RIGHT, VALIGN_TOP,
                "L|Z: Load with ROM\n"
                "R:   Options\n"
            );
        } else {
            ui_components_actions_bar_text_draw(
                ALIGN_RIGHT, VALIGN_TOP,
                "\n"
                "R:   Options\n"
            );
        }

        if (boxart != NULL) {
            ui_components_boxart_draw(boxart);
        }

        ui_components_context_menu_draw(&options_context_menu);
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

    if (menu->load.rom_path && menu->load.combined_disk_rom) {
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

    bookkeeping_history_add(&menu->bookkeeping, menu->load.disk_path, menu->load.rom_path, BOOKKEEPING_TYPE_DISK);
    menu->next_mode = MENU_MODE_BOOT;

    if (menu->load.combined_disk_rom) {
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

    if(menu->load.load_history != -1 || menu->load.load_favorite != -1) {
        int id = -1;
        bookkeeping_item_t* items;

        if(menu->load.load_history != -1) {
            id = menu->load.load_history;
            items = menu->bookkeeping.history_items;
        } else if (menu->load.load_favorite != -1) {
            id = menu->load.load_favorite;
            items = menu->bookkeeping.favorite_items;
        }

        menu->load.load_history = -1;
        menu->load.load_favorite = -1;

        menu->load.disk_path = path_clone(items[id].primary_path);
        if(!load_rom(menu, items[id].secondary_path)) {
            return;
        }

    } else {
        menu->load.disk_path = path_clone_push(menu->browser.directory, menu->browser.entry->name);            
    }

    menu->load.load_favorite = -1;
    menu->load.load_history = -1;

    disk_filename = path_last_get(menu->load.disk_path);
    disk_err_t err = disk_info_load(menu->load.disk_path, &menu->load.disk_info);
    if (err != DISK_OK) {
        menu_show_error(menu, convert_error_message(err));
        return;
    }

    ui_components_context_menu_init(&options_context_menu);
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
