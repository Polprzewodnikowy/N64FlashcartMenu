#include <stdbool.h>
#include <stdio.h>
#include <libdragon.h>
#include <sys/time.h>
#include "../sound.h"
#include "views.h"

#define MAX(a,b)  ({ typeof(a) _a = a; typeof(b) _b = b; _a > _b ? _a : _b; })
#define MIN(a,b)  ({ typeof(a) _a = a; typeof(b) _b = b; _a < _b ? _a : _b; })
#define CLAMP(x, min, max) (MIN(MAX((x), (min)), (max)))

#define YEAR_MIN 1996
#define YEAR_MAX 2095

typedef enum {
    RTC_EDIT_YEAR,
    RTC_EDIT_MONTH,
    RTC_EDIT_DAY,
    RTC_EDIT_HOUR,
    RTC_EDIT_MIN,
    RTC_EDIT_SEC,
} rtc_field_t;

static const char* const DAYS_OF_WEEK[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

static struct tm rtc_tm = {0};
static bool is_editing_mode;
static rtc_field_t editing_field_type = RTC_EDIT_YEAR;

int wrap( uint16_t val, uint16_t min, uint16_t max ) {
    if( val < min ) return max;
    if( val > max ) return min;
    return val;
}

rtc_time_t rtc_time_from_tm( struct tm *time ) {
    return(rtc_time_t){
        .year = CLAMP(time->tm_year + 1900, YEAR_MIN, YEAR_MAX),
        .month = CLAMP(time->tm_mon, 1, 12),
        .day = CLAMP(time->tm_mday, 1, 31),
        .hour = CLAMP(time->tm_hour, 0, 23),
        .min = CLAMP(time->tm_min, 0, 59),
        .sec = CLAMP(time->tm_sec, 0, 59),
        .week_day = CLAMP(time->tm_wday, 0, 6),
    };
}

void adjust_rtc_time( struct tm *t, int incr ) {
    switch(editing_field_type)
    {
        case RTC_EDIT_YEAR:
            t->tm_year = wrap( t->tm_year + incr, YEAR_MIN - 1900, YEAR_MAX - 1900 );
            break;
        case RTC_EDIT_MONTH:
            t->tm_mon = wrap( t->tm_mon + incr, 0, 11 );
            break;
        case RTC_EDIT_DAY:
            t->tm_mday = wrap( t->tm_mday + incr, 1, 31 );
            break;
        case RTC_EDIT_HOUR:
            t->tm_hour = wrap( t->tm_hour + incr, 0, 23 );
            break;
        case RTC_EDIT_MIN:
            t->tm_min = wrap( t->tm_min + incr, 0, 59 );
            break;
        case RTC_EDIT_SEC:
            t->tm_sec = wrap( t->tm_sec + incr, 0, 59 );
            break;
    }
    // Recalculate day-of-week and day-of-year
    time_t timestamp = mktime( t );
    *t = *gmtime( &timestamp );
}

static void component_editdatetime_draw ( void ) {
        // FIXME: move this to components.c once improved.
        /* Format RTC date/time as strings */
        char full_dt[35];
        sprintf( full_dt, ">%04d|%02d|%02d|%02d|%02d|%02d< %s",
            rtc_tm.tm_year + 1900,
            rtc_tm.tm_mon + 1,
            rtc_tm.tm_mday,
            rtc_tm.tm_hour,
            rtc_tm.tm_min,
            rtc_tm.tm_sec,
            DAYS_OF_WEEK[rtc_tm.tm_wday]
            );
        component_messagebox_draw("|YYYY|MM|DD|HH|MM|SS| DOW\n%s\n", full_dt);
}

static void process (menu_t *menu) {
    if (menu->actions.back && !is_editing_mode) {
        sound_play_effect(SFX_EXIT);
        menu->next_mode = MENU_MODE_BROWSER;
    }
    else if (menu->actions.enter && !is_editing_mode) {
        rtc_tm = *gmtime(&menu->current_time);
        is_editing_mode = true;
    }
    
    if (is_editing_mode) {
        if (menu->actions.go_left) {
            if ( editing_field_type <= RTC_EDIT_YEAR ) { editing_field_type = RTC_EDIT_SEC; }
            else { editing_field_type = editing_field_type - 1; }
        }
        else if (menu->actions.go_right) {
            if ( editing_field_type >= RTC_EDIT_SEC ) { editing_field_type = RTC_EDIT_YEAR; }
            else { editing_field_type = editing_field_type + 1; }
        }
        else if (menu->actions.go_up) {
            adjust_rtc_time( &rtc_tm, +1 );
            /* Add a delay so you can just hold the direction */
            wait_ms( 100 );
        }
        else if (menu->actions.go_down) {
            adjust_rtc_time( &rtc_tm, -1 );
            /* Add a delay so you can just hold the direction */
            wait_ms( 100 );
        }
        else if (menu->actions.options) { // R button = save
            if(rtc_is_writable()) {
                // FIXME: settimeofday is not available in libdragon yet.
                // struct timeval new_time = { .tv_sec = mktime(&rtc_tm) };
                // int res = settimeofday(&new_time, NULL);

                rtc_time_t rtc_time = rtc_time_from_tm(&rtc_tm);
                int res = rtc_set(&rtc_time);
                if (res != 1) {
                    menu_show_error(menu, "Failed to set RTC time");
                }
            }
            else {
                menu_show_error(menu, "RTC is not writable");
            }
            is_editing_mode = false;
        }
        else if (menu->actions.back) { // cancel
            is_editing_mode = false;
        }
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
        "To set the RTC date and time, Press A.\n"
        "You can also use the PC terminal application via USB,\n"
        "or even an N64 game with RTC support.\n"
        "\n"
        "Current date & time: %s\n"
        "\n",
        menu->current_time >= 0 ? ctime(&menu->current_time) : "Unknown"
    );

    if (!is_editing_mode) {
        component_actions_bar_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "A: Change\n"
            "B: Back"
        );
    }
    else {
        component_actions_bar_text_draw(
            ALIGN_RIGHT, VALIGN_TOP,
            "Up/Down: Adjust Field\n"
            "Left/Right: Change Field"
        );
        component_actions_bar_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "R: Save\n"
            "B: Back"
        );
    }

    if (is_editing_mode) {
        component_editdatetime_draw();
    }

    rdpq_detach_show();
}


void view_rtc_init (menu_t *menu) {
    is_editing_mode = false;
}

void view_rtc_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}
