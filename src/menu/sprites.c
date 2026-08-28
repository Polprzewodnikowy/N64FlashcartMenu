#include <stdio.h>
#include <stdlib.h>

#include "ui_components.h"

typedef struct {
    const char *path;
    sprite_t *sprite;
} sprite_entry_t;

static sprite_entry_t sprite_entries[SPRITE_LIST_END] = {
    [FILETYPE_SPRITE_FOLDER]            = { "rom:/filetype_folder.sprite" },
    [FILETYPE_SPRITE_COMPRESSED]        = { "rom:/filetype_compressed.sprite" },
    [FILETYPE_SPRITE_N64ROM]            = { "rom:/filetype_n64cart.sprite" },
    [FILETYPE_SPRITE_DDISK]             = { "rom:/filetype_n64disk.sprite" },
    [FILETYPE_SPRITE_MUSIC]             = { "rom:/filetype_music.sprite" },
    [FILETYPE_SPRITE_TEXT]              = { "rom:/filetype_text.sprite" },
    [FILETYPE_SPRITE_IMAGE]             = { "rom:/filetype_image.sprite" },
    [FILETYPE_SPRITE_SAVE]              = { "rom:/filetype_save.sprite" },
    [FILETYPE_SPRITE_FILE]              = { "rom:/filetype_file.sprite" },
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
