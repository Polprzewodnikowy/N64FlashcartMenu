#include <libdragon.h>

#include "actions.h"
#include "key_macros.h"


#define ACTIONS_REPEAT_DELAY    (8)
#define KEYBOARD_IDENTIFIER 0x0002


static int dir_repeat_delay;
static joypad_8way_t last_dir = JOYPAD_8WAY_NONE;
static uint16_t key1;

uint64_t controller_identifiers;
uint16_t port1;
uint16_t port2;
uint16_t port3;
uint16_t port4;


static void actions_clear (menu_t *menu) {
    menu->actions.go_up = false;
    menu->actions.go_down = false;
    menu->actions.go_left = false;
    menu->actions.go_right = false;
    menu->actions.go_fast = false;
    menu->actions.go_c_up = false;
    menu->actions.go_c_down = false;
    menu->actions.go_c_left = false;
    menu->actions.go_c_right = false;

    menu->actions.enter = false;
    menu->actions.back = false;
    menu->actions.options = false;
    menu->actions.settings = false;
    menu->actions.lz_context = false;
    menu->actions.previous_tab = false;
    menu->actions.next_tab = false;
    menu->actions.l = false;
}

static void actions_update_direction (menu_t *menu) {
    joypad_8way_t held_dir = JOYPAD_8WAY_NONE;
    joypad_8way_t fast_dir = JOYPAD_8WAY_NONE;

    JOYPAD_PORT_FOREACH (i) {
        held_dir = joypad_get_direction(i, JOYPAD_2D_DPAD | JOYPAD_2D_STICK);
        fast_dir = joypad_get_direction(i, JOYPAD_2D_C);
        if (held_dir != JOYPAD_8WAY_NONE || fast_dir != JOYPAD_8WAY_NONE) {
            break;
        }
    }

    if (fast_dir != JOYPAD_8WAY_NONE) {
        held_dir = fast_dir;
        menu->actions.go_fast = false;
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

    switch (key1)
    {
    case KEYBOARD_DOWN:
        menu->actions.go_down= true;
        break;
    case KEYBOARD_UP:
        menu->actions.go_up= true;
        break;
    case KEYBOARD_LEFT:
        menu->actions.go_left= true;
        break;
    case KEYBOARD_RIGHT:
        menu->actions.go_right= true;
        break;
    }

    if (dir_repeat_delay > 0) {
        dir_repeat_delay -= 1;
    }

    last_dir = held_dir;
}

static void actions_update_buttons (menu_t *menu) {    
    joypad_buttons_t pressed = {0};

    JOYPAD_PORT_FOREACH (i) {
        pressed = joypad_get_buttons_pressed(i);
        if (pressed.raw) {
            break;
        }
    }

    if (pressed.a || key1 == KEYBOARD_ENTER) {
        menu->actions.enter = true;
    } else if (pressed.b || key1 == KEYBOARD_ESC) {
        menu->actions.back = true;
    } else if (pressed.r || key1 == KEYBOARD_R) {
        menu->actions.options = true;
    } else if (pressed.start || key1 == KEYBOARD_OPT) {
        menu->actions.settings = true;
    } else if (pressed.l || pressed.z || key1 == KEYBOARD_L || key1 == KEYBOARD_Z) {
        menu->actions.lz_context = true;
    } else if (pressed.c_left) {
        menu->actions.previous_tab = true;
    } else if (pressed.c_right) {
        menu->actions.next_tab = true;
    } else if (pressed.c_left) {
        menu-> actions.go_c_left = true;
    } else if (pressed.c_right) {
        menu-> actions.go_c_right = true;
    } else if (pressed.c_up) {
        menu-> actions.go_c_up = true;
    } else if (pressed.c_down) {
        menu-> actions.go_c_down = true;
    }
    
    if (pressed.l || key1 == KEYBOARD_L) {
        menu-> actions.l = true;
    }
}

int check_enter(menu_t *menu, uint8_t port) {
    key1 = 0;
    assert(port < 4);

    /* Turn on the Power LED for all connected keyboards */
    static uint8_t led_status[4] = {0x04, 0x04, 0x04, 0x04};

    const uint64_t input[JOYBUS_BLOCK_DWORDS] = {0, 0, 0, 0, 0, 0, 0, 1};
    uint64_t output[JOYBUS_BLOCK_DWORDS];

    uint8_t *input_bytes = (uint8_t *) input;

    input_bytes[port + 0] = 0x02; // Send bytes
    input_bytes[port + 1] = 0x07; // Recv bytes
    input_bytes[port + 2] = 0x13; // Command
    input_bytes[port + 3] = led_status[port];

    joybus_exec(input, output);

    uint8_t *recv_bytes = ((uint8_t *) output) + port + 4;

    key1 = ((uint16_t) recv_bytes[0] << 8) | recv_bytes[1];
    //uint16_t key2 = ((uint16_t) recv_bytes[2] << 8) | recv_bytes[3];
    //uint16_t key3 = ((uint16_t) recv_bytes[4] << 8) | recv_bytes[5];
    //uint8_t out_status = recv_bytes[6];

    wait_ms(50);

    return 1;
}

uint64_t joybus_controller_identifiers(void) {
    const uint64_t input[JOYBUS_BLOCK_DWORDS] =
            {
                    0xff010300ffffffff,
                    0xff010300ffffffff,
                    0xff010300ffffffff,
                    0xff010300ffffffff,
                    0xfe00000000000000,
                    0,
                    0,
                    1
            };
    uint64_t output[JOYBUS_BLOCK_DWORDS];

    joybus_exec(input, output);

    uint32_t *out_words = (uint32_t *) output;

    uint16_t port1 = out_words[1] >> 16;
    uint16_t port2 = out_words[3] >> 16;
    uint16_t port3 = out_words[5] >> 16;
    uint16_t port4 = out_words[7] >> 16;

    return (
            ((uint64_t) port1 << 48) |
            ((uint64_t) port2 << 32) |
            ((uint64_t) port3 << 16) |
            ((uint64_t) port4 << 0)
    );
}


void actions_init (void) {
    JOYPAD_PORT_FOREACH (port) {
        joypad_set_rumble_active(port, false);
    }
    controller_identifiers = joybus_controller_identifiers();
    port1 = controller_identifiers >> 48;
    port2 = controller_identifiers >> 32;
    port3 = controller_identifiers >> 16;
    port4 = controller_identifiers >> 0;
}

void actions_update (menu_t *menu) {
    joypad_poll();

    actions_clear(menu);

    if( port1 == KEYBOARD_IDENTIFIER ) check_enter(menu, 0);
    if( port2 == KEYBOARD_IDENTIFIER ) check_enter(menu, 1);
    if( port3 == KEYBOARD_IDENTIFIER ) check_enter(menu, 2);
    if( port4 == KEYBOARD_IDENTIFIER ) check_enter(menu, 3);
    actions_update_direction(menu);
    actions_update_buttons(menu);
}
