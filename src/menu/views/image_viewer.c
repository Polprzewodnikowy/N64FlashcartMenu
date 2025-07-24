#include <stdlib.h>
#include "../sound.h"

#include "../png_decoder.h"
#include "views.h"


static bool show_message;
static bool image_loading;
static bool image_set_as_background;
static surface_t *image;


static char *convert_error_message (png_err_t err) {
    switch (err) {
        case PNG_ERR_INT: return "Internal PNG decoder error";
        case PNG_ERR_BUSY: return "PNG decode already in process";
        case PNG_ERR_OUT_OF_MEM: return "PNG decode failed due to insufficient memory";
        case PNG_ERR_NO_FILE: return "PNG decoder couldn't open file";
        case PNG_ERR_BAD_FILE: return "Invalid PNG file";
        default: return "Unknown PNG decoder error";
    }
}

static void image_callback (png_err_t err, surface_t *decoded_image, void *callback_data) {
    menu_t *menu = (menu_t *) (callback_data);

    image_loading = false;
    image = decoded_image;

    if (err != PNG_OK) {
        menu_show_error(menu, convert_error_message(err));
    }
}


static void process (menu_t *menu) {
    if (menu->actions.back) {
        if (show_message) {
            show_message = false;
        } else {
            menu->next_mode = MENU_MODE_BROWSER;
        }
        sound_play_effect(SFX_EXIT);
    } else if (menu->actions.enter && image) {
        if (show_message) {
            show_message = false;
            image_set_as_background = true;
            menu->next_mode = MENU_MODE_BROWSER;
        } else {
            show_message = true;
        }
        sound_play_effect(SFX_ENTER);
    }
}

static void draw (menu_t *menu, surface_t *d) {
    if (!image) {
        rdpq_attach(d, NULL);

        ui_components_background_draw();

        ui_components_loader_draw(png_decoder_get_progress(), "Loading image...");
    } else {
        rdpq_attach_clear(d, NULL);

        uint16_t x = (d->width / 2) - (image->width / 2);
        uint16_t y = (d->height / 2) - (image->height / 2);

        rdpq_mode_push();
            rdpq_set_mode_copy(false);
            rdpq_tex_blit(image, x, y, NULL);
        rdpq_mode_pop();

        if (show_message) {
            ui_components_messagebox_draw(
                "Set \"%s\" as background image?\n\n"
                "A: Yes, B: Back",
                menu->browser.entry->name
            );
        } else if (image_set_as_background) {
            ui_components_messagebox_draw("Preparing background…");
        }
    }

    rdpq_detach_show();
}

static void deinit (menu_t *menu) {
    if (image_loading) {
        png_decoder_abort();
    }

    if (image) {
        if (image_set_as_background) {
            ui_components_background_replace_image(image);
        } else {
            surface_free(image);
            free(image);
        }
    }
}


void view_image_viewer_init (menu_t *menu) {
    show_message = false;
    image_loading = true;
    image_set_as_background = false;
    image = NULL;

    path_t *path = path_clone_push(menu->browser.directory, menu->browser.entry->name);

    png_err_t err = png_decoder_start(path_get(path), 640, 480, image_callback, menu);
    if (err != PNG_OK) {
        menu_show_error(menu, convert_error_message(err));
    }

    path_free(path);
}

void view_image_viewer_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);

    if (menu->next_mode != MENU_MODE_IMAGE_VIEWER) {
        deinit(menu);
    }
}
