/**
 * @file mp3player.h
 * @brief MP3 Player
 * @ingroup menu 
 */

#ifndef MP3_PLAYER_H__
#define MP3_PLAYER_H__


#include <stdbool.h>


/** @brief MP3 file error enumeration */
typedef enum {
    MP3PLAYER_OK,
    MP3PLAYER_ERR_OUT_OF_MEM,
    MP3PLAYER_ERR_IO,
    MP3PLAYER_ERR_NO_FILE,
    MP3PLAYER_ERR_INVALID_FILE,
} mp3player_err_t;


void mp3player_mixer_init (void);
mp3player_err_t mp3player_init (void);
void mp3player_deinit (void);
mp3player_err_t mp3player_load (char *path);
void mp3player_unload (void);
mp3player_err_t mp3player_process (void);
bool mp3player_is_playing (void);
bool mp3player_is_finished (void);
mp3player_err_t mp3player_play (void);
void mp3player_stop (void);
mp3player_err_t mp3player_toggle (void);
void mp3player_mute (bool mute);
mp3player_err_t mp3player_seek (int seconds);
float mp3player_get_duration (void);
float mp3player_get_bitrate (void);
int mp3player_get_samplerate (void);
float mp3player_get_progress (void);


#endif
