#include <libdragon.h>

#include "flashcart/flashcart.h"
#include "fragments/fragments.h"
#include "views.h"


static char *format_flashcart_error (flashcart_error_t error) {
    switch (error) {
        case FLASHCART_OK:
            return "No error";
        case FLASHCART_ERROR_NOT_DETECTED:
            return "No flashcart hardware was detected";
        case FLASHCART_ERROR_OUTDATED:
            return "Outdated flashcart firmware";
        case FLASHCART_ERROR_SD_CARD:
            return "Error during SD card initialization";
        case FLASHCART_ERROR_ARGS:
            return "Invalid argument passed to flashcart function";
        case FLASHCART_ERROR_LOAD:
            return "Error during loading data into flashcart";
        case FLASHCART_ERROR_INT:
            return "Internal flashcart error";
        default:
            return "Unknown flashcart error";
    }
}


static void draw (menu_t *menu, surface_t *d) {
    layout_t *layout = layout_get();

    const int text_x = layout->offset_x + layout->offset_text_x;
    int text_y = layout->offset_y + layout->offset_text_y;

    const color_t bg_color = RGBA32(0x7F, 0x00, 0x00, 0xFF);
    const color_t text_color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF);

    rdpq_attach(d, NULL);
    rdpq_clear(bg_color);

    fragment_text_start(text_color);
    text_y += fragment_textf(text_x, text_y, "Unrecoverable error:");
    text_y += fragment_textf(text_x, text_y, "  %s", format_flashcart_error(menu->flashcart_error));
    if (menu->flashcart_error == FLASHCART_ERROR_OUTDATED) {
        text_y += fragment_textf(text_x, text_y, "  Minimum supported versions:");
        text_y += fragment_textf(text_x, text_y, "    EverDrive-64: ?");
        text_y += fragment_textf(text_x, text_y, "    64drive:      ?");
        text_y += fragment_textf(text_x, text_y, "    SC64:         2.16.0");
    }

    rdpq_detach_show();
}


void view_fault_init (menu_t *menu) {
    // Nothing to initialize (yet)
}

void view_fault_display (menu_t *menu, surface_t *display) {
    draw(menu, display);
}
