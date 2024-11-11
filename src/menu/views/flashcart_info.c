#include "views.h"
#include "../sound.h"
#include <libcart/cart.h>


static inline const char *format_boolean_type (bool bool_value) {
    return bool_value ? "Supported" : "Unsupported";
}

static const char *format_cart_type () {
    switch (cart_type) {
        case CART_CI:
            return "64drive";

        case CART_EDX:
            return "Series X EverDrive-64";

        case CART_ED:
            return "Series V EverDrive-64";

        case CART_SC:
            return "SummerCart64";

        default:        // Probably emulator
            return "Emulator?";
    }
}

static void process (menu_t *menu) {
    if (menu->actions.back) {
        sound_play_effect(SFX_EXIT);
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    ui_components_background_draw();

    ui_components_layout_draw();

    ui_components_main_text_draw(
        ALIGN_CENTER, VALIGN_TOP,
        "FLASHCART INFORMATION"
        "\n"
        "\n"
    );

    ui_components_main_text_draw(
        ALIGN_LEFT, VALIGN_TOP,
        "\n"
        "\n"
        "Type:\n"
        "  %s\n\n"
        "Firmware:\n"
        "  %s\n\n"
        "Features:\n"
        "  Virtual 64DD:     %s.\n"
        "  Real Time Clock:  %s.\n"
        "  USB Debugging:    %s.\n"
        "  Automatic CIC:    %s.\n"
        "  Region Detection: %s.\n"
        "  Save Writeback:   %s.\n"
        "  Update from menu: %s.\n"
        "\n\n",
        format_cart_type(),
        "Not Available", // TODO get cart firmware version(s).
        format_boolean_type(flashcart_has_feature(FLASHCART_FEATURE_64DD)),
        format_boolean_type(flashcart_has_feature(FLASHCART_FEATURE_RTC)),
        format_boolean_type(flashcart_has_feature(FLASHCART_FEATURE_USB)),
        format_boolean_type(flashcart_has_feature(FLASHCART_FEATURE_AUTO_CIC)),
        format_boolean_type(flashcart_has_feature(FLASHCART_FEATURE_AUTO_REGION)),
        format_boolean_type(flashcart_has_feature(FLASHCART_FEATURE_SAVE_WRITEBACK)),
        format_boolean_type(flashcart_has_feature(FLASHCART_FEATURE_BIOS_UPDATE_FROM_MENU))

        //TODO: display the battery and temperature information (if available).
        //format_diagnostic_data(flashcart_has_feature(FLASHCART_FEATURE_DIAGNOSTIC_DATA))
    );

    ui_components_actions_bar_text_draw(
        ALIGN_LEFT, VALIGN_TOP,
        "\n"
        "B: Back"
    );

    rdpq_detach_show();
}


void view_flashcart_info_init (menu_t *menu) {
    // Nothing to initialize (yet)
}

void view_flashcart_info_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}
