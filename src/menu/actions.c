#include <libdragon.h>

#include "actions.h"


#define ACTIONS_REPEAT_DELAY    20
#define ACTIONS_REPEAT_RATE     2


void actions_update (menu_t *menu) {
    menu->actions.go_up = false;
    menu->actions.go_down = false;
    menu->actions.fast = false;
    menu->actions.enter = false;
    menu->actions.back = false;
    menu->actions.info = false;
    menu->actions.settings = false;

    controller_scan();
    struct controller_data down = get_keys_down();
    struct controller_data held = get_keys_held();

    if (down.c[0].err != 0) {
        return;
    }

    if (down.c[0].up || down.c[0].C_up) {
        menu->actions.go_up = true;
        menu->actions.held_counter = 0;
        if (down.c[0].C_up) {
            menu->actions.fast = true;
        }
    } else if (down.c[0].down || down.c[0].C_down) {
        menu->actions.go_down = true;
        menu->actions.held_counter = 0;
        if (down.c[0].C_down) {
            menu->actions.fast = true;
        }
    } else if (held.c[0].up || held.c[0].C_up) {
        menu->actions.held_counter += 1;
        if ((menu->actions.held_counter >= ACTIONS_REPEAT_DELAY) && (menu->actions.held_counter % ACTIONS_REPEAT_RATE)) {
            menu->actions.go_up = true;
            if (held.c[0].C_up) {
                menu->actions.fast = true;
            }
        }
    } else if (held.c[0].down || held.c[0].C_down) {
        menu->actions.held_counter += 1;
        if ((menu->actions.held_counter >= ACTIONS_REPEAT_DELAY) && (menu->actions.held_counter % ACTIONS_REPEAT_RATE)) {
            menu->actions.go_down = true;
            if (held.c[0].C_down) {
                menu->actions.fast = true;
            }
        }
    }

    if (down.c[0].A) {
        menu->actions.enter = true;
    } else if (down.c[0].B) {
        menu->actions.back = true;
    } else if (down.c[0].Z) {
        menu->actions.info = true;
    } else if (down.c[0].start) {
        menu->actions.settings = true;
    }
}
