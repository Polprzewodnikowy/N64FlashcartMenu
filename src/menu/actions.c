#include <libdragon.h>

#include "actions.h"


#define ACTIONS_REPEAT_DELAY    (8)


static int dir_repeat_delay;
static joypad_8way_t last_dir = JOYPAD_8WAY_NONE;


static void actions_clear (menu_t *menu) {
    menu->actions.go_up = false;
    menu->actions.go_down = false;
    menu->actions.go_left = false;
    menu->actions.go_right = false;
    menu->actions.go_fast = false;

    menu->actions.enter = false;
    menu->actions.back = false;
    menu->actions.options = false;
    menu->actions.system_info = false;
    menu->actions.settings = false;
}

static void actions_update_direction (menu_t *menu) {
    joypad_8way_t held_dir = joypad_get_direction(JOYPAD_PORT_1, JOYPAD_2D_DPAD | JOYPAD_2D_STICK);
    joypad_8way_t fast_dir = joypad_get_direction(JOYPAD_PORT_1, JOYPAD_2D_C);

    if (fast_dir != JOYPAD_8WAY_NONE) {
        held_dir = fast_dir;
        menu->actions.go_fast = true;
    }

    joypad_8way_t final_dir = held_dir;

    if ((last_dir != held_dir) && (last_dir == JOYPAD_8WAY_NONE)) {
        dir_repeat_delay = ACTIONS_REPEAT_DELAY;
    } else if (dir_repeat_delay > 0) {
        final_dir = JOYPAD_8WAY_NONE;
    }

    switch (final_dir) {
        case JOYPAD_8WAY_NONE:
            break;
        case JOYPAD_8WAY_RIGHT:
            menu->actions.go_right = true;
            break;
        case JOYPAD_8WAY_UP_RIGHT:
            menu->actions.go_up = true;
            menu->actions.go_right = true;
            break;
        case JOYPAD_8WAY_UP:
            menu->actions.go_up = true;
            break;
        case JOYPAD_8WAY_UP_LEFT:
            menu->actions.go_up = true;
            menu->actions.go_left = true;
            break;
        case JOYPAD_8WAY_LEFT:
            menu->actions.go_left = true;
            break;
        case JOYPAD_8WAY_DOWN_LEFT:
            menu->actions.go_down = true;
            menu->actions.go_left = true;
            break;
        case JOYPAD_8WAY_DOWN:
            menu->actions.go_down = true;
            break;
        case JOYPAD_8WAY_DOWN_RIGHT:
            menu->actions.go_down = true;
            menu->actions.go_right = true;
            break;
    }

    if (dir_repeat_delay > 0) {
        dir_repeat_delay -= 1;
    }

    last_dir = held_dir;
}

static void actions_update_buttons (menu_t *menu) {    
    joypad_buttons_t pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1);

    if (pressed.a) {
        menu->actions.enter = true;
    } else if (pressed.b) {
        menu->actions.back = true;
    } else if (pressed.r) {
        menu->actions.options = true;
    } else if (pressed.l) {
        menu->actions.system_info = true;
    } else if (pressed.start) {
        menu->actions.settings = true;
    }
}


void actions_update (menu_t *menu) {
    joypad_poll();

    actions_clear(menu);
    actions_update_direction(menu);
    actions_update_buttons(menu);
}
