#include <stdbool.h>
#include <stdio.h>
#include <libdragon.h>
#include <sys/time.h>
#include "../sound.h"
#include "views.h"

#define MAX(a,b)  ({ typeof(a) _a = a; typeof(b) _b = b; _a > _b ? _a : _b; })
#define MIN(a,b)  ({ typeof(a) _a = a; typeof(b) _b = b; _a < _b ? _a : _b; })
#define CLAMP(x, min, max) (MIN(MAX((x), (min)), (max)))
#define WRAP(x, min, max)  ({ \
    typeof(x) _x = x; typeof(min) _min = min; typeof(max) _max = max; \
    _x < _min ? _max : _x > _max ? _min : _x; \
})

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
static rtc_field_t editing_field_type;


void adjust_rtc_time( struct tm *t, int incr ) {
    switch(editing_field_type)
    {
        case RTC_EDIT_YEAR:
            t->tm_year = WRAP( t->tm_year + incr, YEAR_MIN - 1900, YEAR_MAX - 1900 );
            break;
        case RTC_EDIT_MONTH:
            t->tm_mon = WRAP( t->tm_mon + incr, 0, 11 );
            break;
        case RTC_EDIT_DAY:
            t->tm_mday = WRAP( t->tm_mday + incr, 1, 31 );
            break;
        case RTC_EDIT_HOUR:
            t->tm_hour = WRAP( t->tm_hour + incr, 0, 23 );
            break;
        case RTC_EDIT_MIN:
            t->tm_min = WRAP( t->tm_min + incr, 0, 59 );
            break;
        case RTC_EDIT_SEC:
            t->tm_sec = WRAP( t->tm_sec + incr, 0, 59 );
            break;
    }
    // Recalculate day-of-week and day-of-year
    time_t timestamp = mktime( t );
    *t = *gmtime( &timestamp );
}

void rtc_ui_component_editdatetime_draw ( struct tm t, rtc_field_t selected_field ) {
    // FIXME: move this to ui_components.c once improved.
    /* Format RTC date/time as strings */
    char full_dt[30];
    char current_selection_chars[30];

    snprintf( full_dt, sizeof(full_dt), ">%04d|%02d|%02d|%02d|%02d|%02d< %s",
        CLAMP(t.tm_year + 1900, YEAR_MIN, YEAR_MAX),
        CLAMP(t.tm_mon + 1, 1, 12),
        CLAMP(t.tm_mday, 1, 31),
        CLAMP(t.tm_hour, 0, 23),
        CLAMP(t.tm_min, 0, 59),
        CLAMP(t.tm_sec, 0, 59),
        DAYS_OF_WEEK[CLAMP(t.tm_wday, 0, 6)]
        );
        
    switch(selected_field)
    {
        // Note: for what ever reason, hat chars need to be duplicated to display correctly. This will be solved when there is a decent UI for it.
        case RTC_EDIT_YEAR:
            snprintf( current_selection_chars, sizeof(current_selection_chars), "*^^^^^^^^********************");
            break;
        case RTC_EDIT_MONTH:
            snprintf( current_selection_chars, sizeof(current_selection_chars), "******^^^^*****************");
            break;
        case RTC_EDIT_DAY:
            snprintf( current_selection_chars, sizeof(current_selection_chars), "*********^^^^**************");
            break;
        case RTC_EDIT_HOUR:
            snprintf( current_selection_chars, sizeof(current_selection_chars), "************^^^^***********");
            break;
        case RTC_EDIT_MIN:
            snprintf( current_selection_chars, sizeof(current_selection_chars), "***************^^^^********");
            break;
        case RTC_EDIT_SEC:
            snprintf( current_selection_chars, sizeof(current_selection_chars), "******************^^^^*****");
            break;
    }
        ui_components_messagebox_draw(
            "|YYYY|MM|DD|HH|MM|SS| DOW\n%s\n%s\n", full_dt, current_selection_chars);
}

static void process (menu_t *menu) {
    if (menu->actions.back && !is_editing_mode) {
        sound_play_effect(SFX_EXIT);
        menu->next_mode = MENU_MODE_BROWSER;
    }
    else if (menu->actions.enter && !is_editing_mode && menu->current_time >= 0) {
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
        }
        else if (menu->actions.go_down) {
            adjust_rtc_time( &rtc_tm, -1 );
        }
        else if (menu->actions.options) { // R button = save
            if( rtc_get_source() == RTC_SOURCE_JOYBUS && rtc_is_source_available( RTC_SOURCE_JOYBUS ) ) {
                struct timeval new_time = { .tv_sec = mktime(&rtc_tm) };
                int res = settimeofday(&new_time, NULL);

                if (res != 0) {
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

    ui_components_background_draw();

    ui_components_layout_draw();

    if (!is_editing_mode) {
         if( menu->current_time >= 0 ) {

            ui_components_main_text_draw(
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

            ui_components_actions_bar_text_draw(
                ALIGN_LEFT, VALIGN_TOP,
                "A: Adjust time\n"
                "B: Back"
            );
         }
         else {

            ui_components_main_text_draw(
                ALIGN_CENTER, VALIGN_TOP,
                "ADJUST REAL TIME CLOCK\n"
                "\n"
                "\n"
                "This cart does not support a real time clock."
                "\n"
                "Current date & time: %s\n"
                "\n",
                menu->current_time >= 0 ? ctime(&menu->current_time) : "Unknown"
            );

            ui_components_actions_bar_text_draw(
                ALIGN_LEFT, VALIGN_TOP,
                "\n"
                "B: Back"
            );
         }
    }
    else {
        ui_components_actions_bar_text_draw(
            ALIGN_RIGHT, VALIGN_TOP,
            "Up/Down: Adjust Field\n"
            "Left/Right: Switch Field"
        );
        ui_components_actions_bar_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "R: Save\n"
            "B: Back"
        );
    }

    if (is_editing_mode) {
        rtc_ui_component_editdatetime_draw(rtc_tm, editing_field_type);
    }

    rdpq_detach_show();
}


void view_rtc_init (menu_t *menu) {
    /* Resync the time from the hardware RTC */
    rtc_set_source( rtc_get_source() );
    is_editing_mode = false;
    editing_field_type = RTC_EDIT_YEAR;
}

void view_rtc_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}
