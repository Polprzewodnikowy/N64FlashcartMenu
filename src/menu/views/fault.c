#include "views.h"


static char *convert_error_message (flashcart_error_t error) {
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
    rdpq_attach(d, NULL);

    rdpq_clear(RGBA32(0x7F, 0x00, 0x00, 0xFF));

    const char *firmware_message = (
        "Minimum supported versions:\n"
        "EverDrive-64: ?\n"
        "64drive: ?\n"
        "SC64: 2.16.0"
    );

    component_messagebox_draw(
        "UNRECOVERABLE ERROR\n"
        "\n"
        "%s\n"
        "\n"
        "%s",
        convert_error_message(menu->flashcart_error),
        menu->flashcart_error == FLASHCART_ERROR_OUTDATED ? firmware_message : ""
    );

    rdpq_detach_show();
}


void view_fault_init (menu_t *menu) {
    // Nothing to initialize (yet)
}

void view_fault_display (menu_t *menu, surface_t *display) {
    draw(menu, display);
}
