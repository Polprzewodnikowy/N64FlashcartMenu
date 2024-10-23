/**
 * @file sound.h
 * @brief Menu Sound
 * @ingroup menu
 */

#ifndef SOUND_H__
#define SOUND_H__

#include <stdbool.h>

#define SOUND_MP3_PLAYER_CHANNEL    (0)
#define SOUND_SFX_CHANNEL           (2)

/**
 * @brief Enumeration of available sound effects for menu interactions.
 */
typedef enum {
    SFX_CURSOR,
    SFX_ERROR,
    SFX_ENTER,
    SFX_EXIT,
    SFX_SETTING,
} sound_effect_t;


void sound_init_default (void);
void sound_init_mp3_playback (void);

/**
 * @brief Initialize sound effects system.
 */
void sound_init_sfx (void);

/**
 * @brief Enable or disable sound effects.
 * @param enable True to enable sound effects, false to disable.
 */
void sound_use_sfx(bool);

/**
 * @brief Play a specified sound effect.
 * @param sfx The sound effect to play, as defined in sound_effect_t.
 */
void sound_play_effect(sound_effect_t sfx);
void sound_deinit (void);
void sound_poll (void);

#endif
