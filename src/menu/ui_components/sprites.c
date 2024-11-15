#include <stdio.h>
#include <stdlib.h>

#include "../ui_components.h"

sprite_t *sprite_joypad_a;
sprite_t *sprite_joypad_b;
sprite_t *sprite_joypad_c_down;
sprite_t *sprite_joypad_c_left;
sprite_t *sprite_joypad_c_right;
sprite_t *sprite_joypad_c_up;
sprite_t *sprite_joypad_d_down;
sprite_t *sprite_joypad_d_left;
sprite_t *sprite_joypad_d_right;
sprite_t *sprite_joypad_d_up;
sprite_t *sprite_joypad_l;
sprite_t *sprite_joypad_r;
sprite_t *sprite_joypad_start;
sprite_t *sprite_joypad_z;

void ui_components_sprites_init(void) {
    sprite_joypad_a = sprite_load("rom:/joypad_a.sprite");
    sprite_joypad_b = sprite_load("rom:/joypad_b.sprite");
    sprite_joypad_c_down = sprite_load("rom:/joypad_c_down.sprite");
    sprite_joypad_c_left = sprite_load("rom:/joypad_c_left.sprite");
    sprite_joypad_c_right = sprite_load("rom:/joypad_c_right.sprite");
    sprite_joypad_c_up = sprite_load("rom:/joypad_c_up.sprite");
    sprite_joypad_d_down = sprite_load("rom:/joypad_d_down.sprite");
    sprite_joypad_d_left = sprite_load("rom:/joypad_d_left.sprite");
    sprite_joypad_d_right = sprite_load("rom:/joypad_d_right.sprite");
    sprite_joypad_d_up = sprite_load("rom:/joypad_d_up.sprite");
    sprite_joypad_l = sprite_load("rom:/joypad_l.sprite");
    sprite_joypad_r = sprite_load("rom:/joypad_r.sprite");
    sprite_joypad_start = sprite_load("rom:/joypad_start.sprite");
    sprite_joypad_z = sprite_load("rom:/joypad_z.sprite");
}

void ui_components_joypad_buttons_draw (sprite_joypad_button_type_t button, float pos_x, float pos_y) {
    sprite_t *current_button;

    switch (button) {
        case SPRITE_JOYPAD_BUTTON_A:
            current_button = sprite_joypad_a;
            break;
        case SPRITE_JOYPAD_BUTTON_B:
            current_button = sprite_joypad_b;
            break;
        case SPRITE_JOYPAD_BUTTON_C_DOWN:
            current_button = sprite_joypad_c_down;
            break;
        case SPRITE_JOYPAD_BUTTON_C_LEFT:
            current_button = sprite_joypad_c_left;
            break;
        case SPRITE_JOYPAD_BUTTON_C_RIGHT:
            current_button = sprite_joypad_c_right;
            break;
        case SPRITE_JOYPAD_BUTTON_C_UP:
            current_button = sprite_joypad_c_up;
            break;
        case SPRITE_JOYPAD_BUTTON_D_DOWN:
            current_button = sprite_joypad_d_down;
            break;
        case SPRITE_JOYPAD_BUTTON_D_LEFT:
            current_button = sprite_joypad_d_left;
            break;
        case SPRITE_JOYPAD_BUTTON_D_RIGHT:
            current_button = sprite_joypad_d_right;
            break;
        case SPRITE_JOYPAD_BUTTON_D_UP:
            current_button = sprite_joypad_d_up;
            break;
        case SPRITE_JOYPAD_BUTTON_L:
            current_button = sprite_joypad_l;
            break;
        case SPRITE_JOYPAD_BUTTON_R:
            current_button = sprite_joypad_r;
            break;
        case SPRITE_JOYPAD_BUTTON_START:
            current_button = sprite_joypad_z;
            break;
        case SPRITE_JOYPAD_BUTTON_Z:
            current_button = sprite_joypad_z;
            break;
        default:
            return;
    }   

    rdpq_set_mode_standard();
    rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
    rdpq_sprite_blit(current_button, pos_x, pos_y, &(rdpq_blitparms_t){
                .scale_x = 1, .scale_y = 1,
            });
}
