#include <stdbool.h>

#include <libdragon.h>

#include "mp3_player.h"
#include "sound.h"


#define DEFAULT_FREQUENCY   (44100)
#define NUM_BUFFERS         (4)
#define NUM_CHANNELS        (3)

static wav64_t sfx_cursor, sfx_error, sfx_enter, sfx_exit, sfx_setting;


static bool sound_initialized = false;
static bool sfx_enabled = false;


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


void sound_init_sfx (void) {
    mixer_ch_set_vol(SOUND_SFX_CHANNEL, 0.5f, 0.5f);
    wav64_open(&sfx_cursor, "rom:/cursorsound.wav64");
    wav64_open(&sfx_exit, "rom:/back.wav64");
    wav64_open(&sfx_setting, "rom:/settings.wav64");
    wav64_open(&sfx_enter, "rom:/enter.wav64");
    wav64_open(&sfx_error, "rom:/error.wav64");
    sfx_enabled = true;
}

void sound_use_sfx(bool state) {
    if (state) {
        sfx_enabled = true;
    }
    else {
        sfx_enabled = false;
    }
}

void sound_play_effect(sound_effect_t sfx) {
    if(sfx_enabled) {
        switch (sfx) {
            case SFX_CURSOR:
                wav64_play(&sfx_cursor, SOUND_SFX_CHANNEL);
                break;
            case SFX_EXIT:
                wav64_play(&sfx_exit, SOUND_SFX_CHANNEL);
                break;
            case SFX_SETTING:
                wav64_play(&sfx_setting, SOUND_SFX_CHANNEL);
                break;
            case SFX_ENTER:
                wav64_play(&sfx_enter, SOUND_SFX_CHANNEL);
                break;
            case SFX_ERROR:
                wav64_play(&sfx_error, SOUND_SFX_CHANNEL);
                break;
            default:
                break;
        } 
    }
}


void sound_deinit (void) {
    if (sound_initialized) {
        if (sfx_enabled) {
            wav64_close(&sfx_cursor);
            wav64_close(&sfx_exit);
            wav64_close(&sfx_setting);
            wav64_close(&sfx_enter);
            wav64_close(&sfx_error);
        }
        mixer_close();
        audio_close();
        sound_initialized = false;
    }
}

void sound_poll (void) {
    if (sound_initialized) {
        mixer_try_play();
    }
}
