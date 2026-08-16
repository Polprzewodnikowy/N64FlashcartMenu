#include <stdio.h>
#include <stdlib.h>

#include "ui_components.h"

typedef struct {
    const char *path;
    sprite_t *sprite;
} sprite_entry_t;

static sprite_entry_t sprite_entries[SPRITE_LIST_END] = {
    [SPRITE_BOXART_DEFAULT]    = { "rom:/defboxart.sprite" },
    [SPRITE_BACKGROUND_DEFAULT] = { "rom:/defbg.sprite" },
    [SPRITE_FOLDER]            = { "rom:/folder.sprite" },
    [SPRITE_COMPRESSED]        = { "rom:/compressed.sprite" },
    [SPRITE_N64ROM]            = { "rom:/N64ROM.sprite" },
    [SPRITE_DDISK]             = { "rom:/ddisk.sprite" },
    [SPRITE_MUSIC]             = { "rom:/music.sprite" },
    [SPRITE_TEXT]              = { "rom:/text.sprite" },
    [SPRITE_IMAGE]             = { "rom:/image.sprite" },
    [SPRITE_SAVE]              = { "rom:/save.sprite" },
    [SPRITE_FILE]              = { "rom:/file.sprite" },
};

void ui_components_sprites_init(void) {
    for (int i = 0; i < SPRITE_LIST_END; i++) {
        sprite_entries[i].sprite = sprite_load(sprite_entries[i].path);
    }
}

void ui_components_sprite_draw (sprite_type_t sprite, float pos_x, float pos_y) {
    if (sprite >= SPRITE_LIST_END) {
        return;
    }

    sprite_t *current_sprite = sprite_entries[sprite].sprite;
    if (current_sprite == NULL) {
        return;
    }

    rdpq_set_mode_standard();
    rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);
    rdpq_sprite_blit(current_sprite, pos_x, pos_y, &(rdpq_blitparms_t){
                    .scale_x = 1, .scale_y = 1,
                });
}
