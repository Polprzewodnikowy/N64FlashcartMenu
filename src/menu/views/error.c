#include "views.h"


static void process (menu_t *menu) {
    if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    component_background_draw();

    if (menu->error_message) {
        component_messagebox_draw(menu->error_message);
    } else {
        component_messagebox_draw("Unspecified error");
    }

    rdpq_detach_show();
}

static void deinit (menu_t *menu) {
    menu->error_message = NULL;
    menu->flashcart_err = FLASHCART_OK;
}


void view_error_init (menu_t *menu) {
    if (menu->flashcart_err != FLASHCART_OK) {
        debugf(
            "Flashcart error [%d]: %s\n",
            menu->flashcart_err,
            flashcart_convert_error_message(menu->flashcart_err)
        );
    }
}

void view_error_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);

    if (menu->next_mode != MENU_MODE_ERROR) {
        deinit(menu);
    }
}

void menu_show_error (menu_t *menu, char *error_message) {
    menu->next_mode = MENU_MODE_ERROR;
    menu->error_message = error_message;
    menu->browser.valid = false;
}
