#include <stdio.h>
#include <stdlib.h>

#include "../ui_components.h"

sprite_t *sprite_defboxart, *sprite_defbg;

void ui_components_sprites_init(void) {
    sprite_defboxart = sprite_load("rom:/defboxart.sprite");
    sprite_defbg = sprite_load("rom:/defbg.sprite");
}

void ui_components_sprite_draw (sprite_type_t sprite, float pos_x, float pos_y) {
    sprite_t *current_sprite;

    switch (sprite) {
        case SPRITE_BOXART_DEFAULT:
            current_sprite = sprite_defboxart;
            break;
        case SPRITE_BACKGROUND_DEFAULT:
            current_sprite = sprite_defbg;
            break;
        default:
            return;
    }   

    rdpq_set_mode_standard();
    rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
    rdpq_sprite_blit(current_sprite, pos_x, pos_y, &(rdpq_blitparms_t){
                .scale_x = 1, .scale_y = 1,
            });
}
