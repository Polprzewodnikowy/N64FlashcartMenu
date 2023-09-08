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
    controller_scan();

    struct controller_data down = get_keys_down();
    struct controller_data held = get_keys_held();
    struct controller_data pressed = get_keys_pressed();

    if (down.c[0].err != ERROR_NONE) {
        return;
    }

    actions_clear(menu);

    if (down.c[0].up || down.c[0].C_up) {
        menu->actions.go_up = true;
        menu->actions.vertical_held_counter = 0;
        if (down.c[0].C_up) {
            menu->actions.fast = true;
        }
    } else if (down.c[0].down || down.c[0].C_down) {
        menu->actions.go_down = true;
        menu->actions.vertical_held_counter = 0;
        if (down.c[0].C_down) {
            menu->actions.fast = true;
        }
    } else if (held.c[0].up || held.c[0].C_up) {
        menu->actions.vertical_held_counter += 1;
        if (menu->actions.vertical_held_counter >= ACTIONS_REPEAT_DELAY) {
            menu->actions.go_up = true;
            if (held.c[0].C_up) {
                menu->actions.fast = true;
            }
        }
    } else if (held.c[0].down || held.c[0].C_down) {
        menu->actions.vertical_held_counter += 1;
        if (menu->actions.vertical_held_counter >= ACTIONS_REPEAT_DELAY) {
            menu->actions.go_down = true;
            if (held.c[0].C_down) {
                menu->actions.fast = true;
            }
        }
    } else if (pressed.c[0].y > +JOYSTICK_DEADZONE) { // TODO: requires improvement for responsiveness
        menu->actions.vertical_held_counter += 1;
        if (menu->actions.vertical_held_counter >= ACTIONS_REPEAT_DELAY / 2) {
            menu->actions.go_up = true;
            if (pressed.c[0].y < +75) {
                menu->actions.vertical_held_counter = 0;
            }
        }
    } else if (pressed.c[0].y < -JOYSTICK_DEADZONE) { // TODO: requires improvement for responsiveness
        menu->actions.vertical_held_counter += 1;
        if (menu->actions.vertical_held_counter >= ACTIONS_REPEAT_DELAY / 2) {
            menu->actions.go_down = true;
            if (pressed.c[0].y > -75) {
                menu->actions.vertical_held_counter = 0;
            }
        }
    }

    if (down.c[0].left) {
        menu->actions.go_left = true;
        menu->actions.horizontal_held_counter = 0;
    } else if (down.c[0].right) {
        menu->actions.go_right = true;
        menu->actions.horizontal_held_counter = 0;
    } else if (held.c[0].left) {
        menu->actions.horizontal_held_counter += 1;
        if (menu->actions.horizontal_held_counter >= ACTIONS_REPEAT_DELAY) {
            menu->actions.go_left = true;
        }
    } else if (held.c[0].right) {
        menu->actions.horizontal_held_counter += 1;
        if (menu->actions.horizontal_held_counter >= ACTIONS_REPEAT_DELAY) {
            menu->actions.go_right = true;
        }
    }

    if (down.c[0].A) {
        menu->actions.enter = true;
    } else if (down.c[0].B) {
        menu->actions.back = true;
    } else if (down.c[0].R) {
        menu->actions.options = true;
    } else if (down.c[0].L) {
        menu->actions.system_info = true;
    } else if (down.c[0].start) {
        menu->actions.settings = true;
    }
}
