#include "../cart_load.h"
#include "../disk_info.h"
#include "boot/boot.h"
#include "views.h"


static bool load_pending;
static bool load_rom;


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

    component_background_draw();

    if (load_pending) {
        component_loader_draw(0.0f);
    } else {
        component_layout_draw();

        component_main_text_draw(
            ALIGN_CENTER, VALIGN_TOP,
            "64DD disk information\n"
            "\n"
            "%s",
            menu->browser.entry->name
        );

        component_main_text_draw(
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

        component_actions_bar_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "A: Load and run 64DD disk\n"
            "B: Exit"
        );

        if (menu->load.rom_path) {
            component_actions_bar_text_draw(
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

        component_background_draw();

        component_loader_draw(progress);

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

    err = cart_load_64dd_ipl_and_disk(menu, draw_progress);
    if (err != CART_LOAD_OK) {
        menu_show_error(menu, cart_load_convert_error_message(err));
        return;
    }

    menu->next_mode = MENU_MODE_BOOT;
    menu->boot_params->device_type = load_rom ? BOOT_DEVICE_TYPE_ROM : BOOT_DEVICE_TYPE_64DD;
    menu->boot_params->tv_type = BOOT_TV_TYPE_PASSTHROUGH;
    menu->boot_params->detect_cic_seed = true;
}


void view_load_disk_init (menu_t *menu) {
    if (menu->load.disk_path) {
        path_free(menu->load.disk_path);
        menu->load.disk_path = NULL;
    }

    load_pending = false;

    menu->load.disk_path = path_clone_push(menu->browser.directory, menu->browser.entry->name);

    disk_err_t err = disk_info_load(path_get(menu->load.disk_path), &menu->load.disk_info);
    if (err != DISK_OK) {
        menu_show_error(menu, convert_error_message(err));
    }
}

void view_load_disk_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);

    if (load_pending) {
        load_pending = false;
        load(menu);
    }
}
