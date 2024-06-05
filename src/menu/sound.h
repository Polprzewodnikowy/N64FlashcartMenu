/**
 * @file sound.h
 * @brief Menu Sound
 * @ingroup menu
 */

#ifndef SOUND_H__
#define SOUND_H__

#define SOUND_MP3_PLAYER_CHANNEL    (0)
#define SOUND_SFX_CHANNEL           (2)

typedef enum {
    SFX_CURSOR,
    SFX_ERROR,
    SFX_ENTER,
    SFX_EXIT,
    SFX_SETTING,
} sound_effect_t;


void sound_init_default (void);
void sound_init_mp3_playback (void);
void sound_init_sfx (void);
void sound_play_effect(sound_effect_t sfx);
void sound_deinit (void);
void sound_poll (void);

#endif
