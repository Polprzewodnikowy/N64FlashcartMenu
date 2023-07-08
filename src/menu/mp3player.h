#ifndef MP3PLAYER_H__
#define MP3PLAYER_H__


#include <stdbool.h>


typedef enum {
    MP3PLAYER_OK,
    MP3PLAYER_ERR_MALLOC,
    MP3PLAYER_ERR_IO,
    MP3PLAYER_ERR_NO_FILE,
    MP3PLAYER_ERR_INVALID_FILE,
} mp3player_err_t;


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
mp3player_err_t mp3player_seek (int seconds);
float mp3player_get_progress (void);


#endif
