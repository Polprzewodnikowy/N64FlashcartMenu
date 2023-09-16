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

    joypad_buttons_t down = joypad_get_buttons_pressed(0);
    joypad_buttons_t held = joypad_get_buttons_held(0);
    //joypad_buttons_t pressed = joypad_get_buttons_released(0);

    // if (down.err != ERROR_NONE) {
    //     return;
    // }

    actions_clear(menu);

    if (down.d_up || down.c_up) {
        menu->actions.go_up = true;
        menu->actions.vertical_held_counter = 0;
        if (down.c_up) {
            menu->actions.fast = true;
        }
    } else if (down.d_down || down.c_down) {
        menu->actions.go_down = true;
        menu->actions.vertical_held_counter = 0;
        if (down.c_down) {
            menu->actions.fast = true;
        }
    } else if (held.d_up || held.c_up) {
        menu->actions.vertical_held_counter += 1;
        if (menu->actions.vertical_held_counter >= ACTIONS_REPEAT_DELAY) {
            menu->actions.go_up = true;
            if (held.c_up) {
                menu->actions.fast = true;
            }
        }
    } else if (held.d_down || held.c_down) {
        menu->actions.vertical_held_counter += 1;
        if (menu->actions.vertical_held_counter >= ACTIONS_REPEAT_DELAY) {
            menu->actions.go_down = true;
            if (held.c_down) {
                menu->actions.fast = true;
            }
        }
    // } else if (down.y > +JOYSTICK_DEADZONE) { // TODO: requires improvement for responsiveness
    //     menu->actions.vertical_held_counter += 1;
    //     if (menu->actions.vertical_held_counter >= ACTIONS_REPEAT_DELAY / 2) {
    //         menu->actions.go_up = true;
    //         if (held.y < +75) {
    //             menu->actions.vertical_held_counter = 0;
    //         }
    //     }
    // } else if (down.y < -JOYSTICK_DEADZONE) { // TODO: requires improvement for responsiveness
    //     menu->actions.vertical_held_counter += 1;
    //     if (menu->actions.vertical_held_counter >= ACTIONS_REPEAT_DELAY / 2) {
    //         menu->actions.go_down = true;
    //         if (held.y > -75) {
    //             menu->actions.vertical_held_counter = 0;
    //         }
    //     }
    }

    if (down.d_left) {
        menu->actions.go_left = true;
        menu->actions.horizontal_held_counter = 0;
    } else if (down.d_right) {
        menu->actions.go_right = true;
        menu->actions.horizontal_held_counter = 0;
    } else if (held.d_left) {
        menu->actions.horizontal_held_counter += 1;
        if (menu->actions.horizontal_held_counter >= ACTIONS_REPEAT_DELAY) {
            menu->actions.go_left = true;
        }
    } else if (held.d_right) {
        menu->actions.horizontal_held_counter += 1;
        if (menu->actions.horizontal_held_counter >= ACTIONS_REPEAT_DELAY) {
            menu->actions.go_right = true;
        }
    }

    if (down.a) {
        menu->actions.enter = true;
    } else if (down.b) {
        menu->actions.back = true;
    } else if (down.r) {
        menu->actions.options = true;
    } else if (down.l) {
        menu->actions.system_info = true;
    } else if (down.start) {
        menu->actions.settings = true;
    }
}
