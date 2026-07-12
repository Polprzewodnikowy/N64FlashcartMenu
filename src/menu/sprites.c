#include <stdio.h>
#include <stdlib.h>

#include "ui_components.h"

sprite_t *sprite_defboxart, *sprite_defbg, *spr_folder, *spr_compressed, *spr_n64rom, *spr_ddisk, *spr_music, *spr_text, *spr_image, *spr_save, *spr_file;

void ui_components_sprites_init(void) {
    sprite_defboxart = sprite_load("rom:/defboxart.sprite");
    sprite_defbg = sprite_load("rom:/defbg.sprite");
    spr_folder = sprite_load("rom:/folder.sprite");
    spr_compressed = sprite_load("rom:/compressed.sprite");
    spr_n64rom = sprite_load("rom:/N64ROM.sprite");
    spr_ddisk = sprite_load("rom:/ddisk.sprite");
    spr_music = sprite_load("rom:/music.sprite");
    spr_text = sprite_load("rom:/text.sprite");
    spr_image = sprite_load("rom:/image.sprite");
    spr_save = sprite_load("rom:/save.sprite");
    spr_file = sprite_load("rom:/file.sprite");

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
        case SPRITE_FOLDER:
            current_sprite = spr_folder;
            break;
        case SPRITE_COMPRESSED:
            current_sprite = spr_compressed;
            break;
        case SPRITE_N64ROM:
            current_sprite = spr_n64rom;
            break;
        case SPRITE_DDISK:
            current_sprite = spr_ddisk;
            break;
        case SPRITE_MUSIC:  
            current_sprite = spr_music;
            break;
        case SPRITE_TEXT:
            current_sprite = spr_text;
            break;
        case SPRITE_IMAGE:
            current_sprite = spr_image;
            break;
        case SPRITE_SAVE:
            current_sprite = spr_save;
            break;
        case SPRITE_FILE:
            current_sprite = spr_file;
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