#include "views.h"
#include "../sound.h"


static inline const char *format_boolean_type (bool bool_value) {
    return bool_value ? "Supported" : "Unsupported";
}

static void process (menu_t *menu) {
    if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
        sound_play_effect(SFX_EXIT);
    }
}

static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    component_background_draw();

    component_layout_draw();

    component_main_text_draw(
        ALIGN_CENTER, VALIGN_TOP,
        "FLASHCART INFORMATION"
        "\n"
        "\n"
    );

    component_main_text_draw(
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
        "  CIC Detection:    %s.\n"
        "  Region Detection: %s.\n"
        "\n\n",
        "SummerCart64",
        "V?.?.?",
        format_boolean_type(true),
        format_boolean_type(true),
        format_boolean_type(true),
        format_boolean_type(true),
        format_boolean_type(true)
    );

    // FIXME: Display:
    // * cart_type
    // * Firmware version
    // * supported features (flashcart_features_t)


    component_actions_bar_text_draw(
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
