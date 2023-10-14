/**
 * @file sound.h
 * @brief Menu Sound
 * @ingroup menu
 */

#ifndef SOUND_H__
#define SOUND_H__


#define SOUND_MP3_PLAYER_CHANNEL    (0)


void sound_init_default (void);
void sound_init_mp3_playback (void);
void sound_deinit (void);
void sound_poll (void);


#endif
