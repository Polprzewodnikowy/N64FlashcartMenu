#include <stdlib.h>
#include <math.h>
#include <libdragon.h>
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
        // Restore background before handing off to error view — deinit may not run
        ui_components_background_reload();
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

        /* Scale image to fit screen, preserving aspect ratio */
        float scale_x = (float)d->width / image->width;
        float scale_y = (float)d->height / image->height;
        float scale = (scale_x < scale_y) ? scale_x : scale_y;
        int disp_w = (int)(image->width * scale);
        int disp_h = (int)(image->height * scale);
        int x = (d->width - disp_w) / 2;
        int y = (d->height - disp_h) / 2;

        rdpq_set_mode_standard();
        rdpq_mode_filter(FILTER_BILINEAR);
        rdpq_mode_combiner(RDPQ_COMBINER_TEX);
        rdpq_tex_blit(image, x, y, &(rdpq_blitparms_t) {
            .scale_x = scale,
            .scale_y = scale,
            .filtering = true,
        });

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
            // Restore the background that was freed at init to give the decoder more memory
            ui_components_background_reload();
        }
    } else {
        // Decode failed or was aborted — still restore the background
        ui_components_background_reload();
    }
    image = NULL;
}


void view_image_viewer_init (menu_t *menu) {
    show_message = false;
    image_loading = true;
    image_set_as_background = false;
    image = NULL;
    // Free the background image temporarily so the PNG decoder has its full memory budget;
    // ui_components_background_reload() restores it if the user does not set a new background
    ui_components_background_image_free_only();
    int max_w = display_get_width();
    int max_h = display_get_height();

    path_t *path = path_clone_push(menu->browser.directory, menu->browser.entry->name);

    png_err_t err = png_decoder_start(path_get(path), max_w, max_h, image_callback, menu);
    if (err != PNG_OK) {
        image_loading = false;
        ui_components_background_reload();
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
