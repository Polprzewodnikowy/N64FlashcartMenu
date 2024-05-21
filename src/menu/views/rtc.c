#include <time.h>
#include "../sound.h"
#include "views.h"
#include <stdio.h>
#include <libdragon.h>

// FIXME: add implementation!
// struct {
//     uint16_t seconds;
//     uint16_t minutes;
//     uint16_t hours;
//     uint16_t day;
//     uint16_t month;
//     uint16_t year;
// } adjusted_datetime;

typedef enum {
    RTC_EDIT_YEAR,
    RTC_EDIT_MONTH,
    RTC_EDIT_DAY,
    RTC_EDIT_HOUR,
    RTC_EDIT_MIN,
    RTC_EDIT_SEC,
} rtc_field_t;


static rtc_time_t rtc_time;
static uint16_t is_editing_mode = false;
static uint16_t editing_field_type = RTC_EDIT_YEAR;

int wrap( uint16_t val, uint16_t min, uint16_t max )
{
    if( val < min ) return max;
    if( val > max ) return min;
    return val;
}

void adjust_rtc_time( rtc_time_t* dt, int incr )
{
    uint8_t expected_day = 0;
    switch( editing_field_type )
    {
        case RTC_EDIT_YEAR:
            /* TODO Figure out what the max supported year is */
            dt->year = wrap( dt->year + incr, 1996, 2037 );
            break;
        case RTC_EDIT_MONTH:
            dt->month = wrap( dt->month + incr, 0, 11 );
            break;
        case RTC_EDIT_DAY:
            dt->day = wrap( dt->day + incr, 1, 31 );
            expected_day = dt->day;
            break;
        case RTC_EDIT_HOUR:
            dt->hour = wrap( dt->hour + incr, 0, 23 );
            break;
        case RTC_EDIT_MIN:
            dt->min = wrap( dt->min + incr, 0, 59 );
            break;
        case RTC_EDIT_SEC:
            dt->sec = wrap( dt->sec + incr, 0, 59 );
            break;
    }
    rtc_normalize_time( dt );
    /* Handle wrap-around for normalized day of month */
    if( expected_day && expected_day != dt->day && incr > 0 ) dt->day = 1;
}

// static void save_adjusted_datetime () {

// }

static void process (menu_t *menu) {
    if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
        sound_play_effect(SFX_EXIT);
    }
    // FIXME: these are submenu items
    if (menu->actions.enter) {
        is_editing_mode = true;
        //menu->next_mode = MENU_MODE_BROWSER;
    }
    if (menu->actions.go_left) {
        if ( editing_field_type > RTC_EDIT_YEAR ) { editing_field_type = RTC_EDIT_SEC; }
        else { menu->next_mode = editing_field_type - 1; }
    }
    if (menu->actions.go_right) {
        if ( editing_field_type < RTC_EDIT_SEC ) { editing_field_type = RTC_EDIT_YEAR; }
        else { menu->next_mode = editing_field_type + 1; }
    }
    if (menu->actions.go_up) {
        adjust_rtc_time( &rtc_time, +1 );
        /* Add a delay so you can just hold the direction */
        wait_ms( 100 );
    }
    if (menu->actions.go_down) {
        adjust_rtc_time( &rtc_time, -1 );
        /* Add a delay so you can just hold the direction */
        wait_ms( 100 );
    }

}

static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    component_background_draw();

    component_layout_draw();

    if (rtc_is_writable()) {
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
    }
    else {
        // FIXME: time since N64 started?
        component_main_text_draw(
            ALIGN_CENTER, VALIGN_TOP,
            "REAL TIME CLOCK\n"
            "\n"
            "\n"
            "Adjusting the date and time is not supported.\n"
            "Current date & time: %s\n",
            menu->current_time >= 0 ? ctime(&menu->current_time) : "Unknown\n"
        );
    }

    component_main_text_draw(
        ALIGN_LEFT, VALIGN_TOP,
        "\n"
        "\n"
    );

    if (rtc_is_writable()) {
        component_actions_bar_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "A: Change\n"
            "B: Back"
        );
    }
    else {
        component_actions_bar_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "\n"
            "B: Back"
        );
    }

    rdpq_detach_show();
}


void view_rtc_init (menu_t *menu) {
    // Nothing to initialize (yet)
}

void view_rtc_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}
