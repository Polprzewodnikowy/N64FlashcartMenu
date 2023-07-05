#ifndef MP3PLAYER_H__
#define MP3PLAYER_H__


#include <stdbool.h>


bool mp3player_load (char *path);
void mp3player_unload (void);
void mp3player_start (void);
bool mp3player_process (void);


#endif
