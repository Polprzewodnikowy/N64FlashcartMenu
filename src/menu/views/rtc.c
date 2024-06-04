#include <time.h>
#include "../menu.h"
#include "../sound.h"
#include "views.h"

// FIXME: add implementation!
// struct {
//     uint16_t seconds;
//     uint16_t minutes;
//     uint16_t hours;
//     uint16_t day;
//     uint16_t month;
//     uint16_t year;
// } adjusted_datetime;

// static void save_adjusted_datetime () {

// }

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
        "ADJUST REAL TIME CLOCK\n"
        "\n"
        "\n"
        "To set the date and time, please use the PC terminal\n"
        "application and set via USB.\n\n"
        "Current date & time: %s\n",
        menu->current_time >= 0 ? ctime(&menu->current_time) : "Unknown\n"
    );

    component_main_text_draw(
        ALIGN_LEFT, VALIGN_TOP,
        "\n"
        "\n"
    );


    component_actions_bar_text_draw(
        ALIGN_LEFT, VALIGN_TOP,
        "\n" // "A: Save\n"
        "B: Back"
    );

    rdpq_detach_show();
}


void view_rtc_init (menu_t *menu) {
    // Nothing to initialize (yet)
}

void view_rtc_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}
