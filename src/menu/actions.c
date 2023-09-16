#include <libdragon.h>

#include "actions.h"


#define ACTIONS_REPEAT_DELAY    10
#define JOYSTICK_DEADZONE       32


static void actions_clear (menu_t *menu) {
    menu->actions.go_up = false;
    menu->actions.go_down = false;
    menu->actions.go_left = false;
    menu->actions.go_right = false;
    menu->actions.fast = false;
    menu->actions.enter = false;
    menu->actions.back = false;
    menu->actions.options = false;
    menu->actions.system_info = false;
    menu->actions.settings = false;
}


void actions_update (menu_t *menu) {
    joypad_poll();

    joypad_buttons_t btn_pressed = joypad_get_buttons_pressed(0);
    joypad_buttons_t btn_held = joypad_get_buttons_held(0);
    joypad_inputs_t joypad_inputs = joypad_get_inputs(0);

    // if (btn_pressed.err != ERROR_NONE) {
    //     return;
    // }

    actions_clear(menu);

    if (btn_pressed.d_up || btn_pressed.c_up) {
        menu->actions.go_up = true;
        menu->actions.vertical_held_counter = 0;
        if (btn_pressed.c_up) {
            menu->actions.fast = true;
        }
    } else if (btn_pressed.d_down || btn_pressed.c_down) {
        menu->actions.go_down = true;
        menu->actions.vertical_held_counter = 0;
        if (btn_pressed.c_down) {
            menu->actions.fast = true;
        }
    } else if (btn_held.d_up || btn_held.c_up) {
        menu->actions.vertical_held_counter += 1;
        if (menu->actions.vertical_held_counter >= ACTIONS_REPEAT_DELAY) {
            menu->actions.go_up = true;
            if (btn_held.c_up) {
                menu->actions.fast = true;
            }
        }
    } else if (btn_held.d_down || btn_held.c_down) {
        menu->actions.vertical_held_counter += 1;
        if (menu->actions.vertical_held_counter >= ACTIONS_REPEAT_DELAY) {
            menu->actions.go_down = true;
            if (btn_held.c_down) {
                menu->actions.fast = true;
            }
        }
    } else if (joypad_inputs.stick_y > +JOYSTICK_DEADZONE) { // TODO: requires improvement for responsiveness
        menu->actions.vertical_held_counter += 1;
        if (menu->actions.vertical_held_counter >= ACTIONS_REPEAT_DELAY / 2) {
            menu->actions.go_up = true;
            if (joypad_inputs.stick_y < +75) {
                menu->actions.vertical_held_counter = 0;
            }
        }
    } else if (joypad_inputs.stick_y < -JOYSTICK_DEADZONE) { // TODO: requires improvement for responsiveness
        menu->actions.vertical_held_counter += 1;
        if (menu->actions.vertical_held_counter >= ACTIONS_REPEAT_DELAY / 2) {
            menu->actions.go_down = true;
            if (joypad_inputs.stick_y > -75) {
                menu->actions.vertical_held_counter = 0;
            }
        }
    }

    if (btn_pressed.d_left) {
        menu->actions.go_left = true;
        menu->actions.horizontal_held_counter = 0;
    } else if (btn_pressed.d_right) {
        menu->actions.go_right = true;
        menu->actions.horizontal_held_counter = 0;
    } else if (btn_held.d_left) {
        menu->actions.horizontal_held_counter += 1;
        if (menu->actions.horizontal_held_counter >= ACTIONS_REPEAT_DELAY) {
            menu->actions.go_left = true;
        }
    } else if (btn_held.d_right) {
        menu->actions.horizontal_held_counter += 1;
        if (menu->actions.horizontal_held_counter >= ACTIONS_REPEAT_DELAY) {
            menu->actions.go_right = true;
        }
    }

    if (btn_pressed.a) {
        menu->actions.enter = true;
    } else if (btn_pressed.b) {
        menu->actions.back = true;
    } else if (btn_pressed.r) {
        menu->actions.options = true;
    } else if (btn_pressed.l) {
        menu->actions.system_info = true;
    } else if (btn_pressed.start) {
        menu->actions.settings = true;
    }
}
