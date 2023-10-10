#include <stdbool.h>

#include <libdragon.h>

#include "mp3_player.h"


#define DEFAULT_FREQUENCY   (44100)
#define NUM_BUFFERS         (2)
#define NUM_CHANNELS        (2)


static bool sound_initialized = false;


static void sound_reconfigure (int frequency) {
    if ((frequency > 0) && (audio_get_frequency() != frequency)) {
        if (sound_initialized) {
            mixer_close();
            audio_close();
        }
        audio_init(frequency, NUM_BUFFERS);
        mixer_init(NUM_CHANNELS);
        mp3player_mixer_init();
        sound_initialized = true;
    }
}


void sound_init_default (void) {
    sound_reconfigure(DEFAULT_FREQUENCY);
}

void sound_init_mp3_playback (void) {
    sound_reconfigure(mp3player_get_samplerate());
}

void sound_deinit (void) {
    if (sound_initialized) {
        mixer_close();
        audio_close();
        sound_initialized = false;
    }
}

void sound_poll (void) {
    if (sound_initialized && audio_can_write()) {
        short *audio_buffer = audio_write_begin();
        mixer_poll(audio_buffer, audio_get_buffer_length());
        audio_write_end();
    }
}
