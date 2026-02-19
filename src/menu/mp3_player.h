/**
 * @file mp3_player.h
 * @brief MP3 Player
 * @ingroup menu
 */

#ifndef MP3_PLAYER_H__
#define MP3_PLAYER_H__

#include <stdbool.h>

/** @brief Maximum length for metadata string fields. */
#define MP3_METADATA_MAX_LEN    (64)

/**
 * @brief MP3 metadata extracted from ID3 tags.
 */
typedef struct {
    char title[MP3_METADATA_MAX_LEN];   /**< Track title */
    char artist[MP3_METADATA_MAX_LEN];  /**< Artist / performer */
    char album[MP3_METADATA_MAX_LEN];   /**< Album name */
    int track_number;                    /**< Track number (0 if unknown) */
    bool has_metadata;                   /**< True if any metadata was found */
    bool has_cover_art;                  /**< True if an APIC PNG was extracted */
    char cover_art_path[256];            /**< Path to extracted cover art temp file */
} mp3_metadata_t;

/**
 * @brief MP3 file error enumeration.
 * 
 * Enumeration for different types of errors that can occur in the MP3 player.
 */
typedef enum {
    MP3PLAYER_OK,               /**< No error */
    MP3PLAYER_ERR_OUT_OF_MEM,   /**< Out of memory error */
    MP3PLAYER_ERR_IO,           /**< Input/Output error */
    MP3PLAYER_ERR_NO_FILE,      /**< No file found error */
    MP3PLAYER_ERR_INVALID_FILE, /**< Invalid file error */
} mp3player_err_t;

/**
 * @brief Initialize the MP3 player mixer.
 * 
 * This function initializes the mixer for the MP3 player.
 */
void mp3player_mixer_init(void);

/**
 * @brief Initialize the MP3 player.
 * 
 * This function initializes the MP3 player and prepares it for playback.
 * 
 * @return mp3player_err_t Error code indicating the result of the initialization.
 */
mp3player_err_t mp3player_init(void);

/**
 * @brief Deinitialize the MP3 player.
 * 
 * This function deinitializes the MP3 player and releases any resources.
 */
void mp3player_deinit(void);

/**
 * @brief Load an MP3 file.
 * 
 * This function loads an MP3 file from the specified path.
 * 
 * @param path Path to the MP3 file.
 * @return mp3player_err_t Error code indicating the result of the load operation.
 */
mp3player_err_t mp3player_load(char *path);

/**
 * @brief Unload the current MP3 file.
 * 
 * This function unloads the currently loaded MP3 file.
 */
void mp3player_unload(void);

/**
 * @brief Process the MP3 player.
 * 
 * This function processes the MP3 player, handling playback and other operations.
 * 
 * @return mp3player_err_t Error code indicating the result of the process operation.
 */
mp3player_err_t mp3player_process(void);

/**
 * @brief Check if the MP3 player is playing.
 * 
 * This function checks if the MP3 player is currently playing.
 * 
 * @return true if the MP3 player is playing, false otherwise.
 */
bool mp3player_is_playing(void);

/**
 * @brief Check if the MP3 player has finished playing.
 * 
 * This function checks if the MP3 player has finished playing the current file.
 * 
 * @return true if the MP3 player has finished playing, false otherwise.
 */
bool mp3player_is_finished(void);

/**
 * @brief Start playback of the MP3 file.
 * 
 * This function starts playback of the currently loaded MP3 file.
 * 
 * @return mp3player_err_t Error code indicating the result of the play operation.
 */
mp3player_err_t mp3player_play(void);

/**
 * @brief Stop playback of the MP3 file.
 * 
 * This function stops playback of the currently loaded MP3 file.
 */
void mp3player_stop(void);

/**
 * @brief Toggle playback of the MP3 file.
 * 
 * This function toggles playback of the currently loaded MP3 file.
 * 
 * @return mp3player_err_t Error code indicating the result of the toggle operation.
 */
mp3player_err_t mp3player_toggle(void);

/**
 * @brief Mute or unmute the MP3 player.
 * 
 * This function mutes or unmutes the MP3 player.
 * 
 * @param mute true to mute, false to unmute.
 */
void mp3player_mute(bool mute);

/**
 * @brief Seek to a specific position in the MP3 file.
 * 
 * This function seeks to a specific position in the currently loaded MP3 file.
 * 
 * @param seconds Number of seconds to seek.
 * @return mp3player_err_t Error code indicating the result of the seek operation.
 */
mp3player_err_t mp3player_seek(int seconds);

/**
 * @brief Get the duration of the MP3 file.
 * 
 * This function gets the duration of the currently loaded MP3 file.
 * 
 * @return float Duration of the MP3 file in seconds.
 */
float mp3player_get_duration(void);

/**
 * @brief Get the bitrate of the MP3 file.
 * 
 * This function gets the bitrate of the currently loaded MP3 file.
 * 
 * @return float Bitrate of the MP3 file in bps.
 */
float mp3player_get_bitrate(void);

/**
 * @brief Get the sample rate of the MP3 file.
 * 
 * This function gets the sample rate of the currently loaded MP3 file.
 * 
 * @return int Sample rate of the MP3 file in Hz.
 */
int mp3player_get_samplerate(void);

/**
 * @brief Get the current playback progress.
 * 
 * This function gets the current playback progress of the MP3 file.
 * 
 * @return float Current playback progress as a fraction (0.0 to 1.0).
 */
float mp3player_get_progress(void);

/**
 * @brief Get metadata extracted from ID3 tags.
 * 
 * @return const mp3_metadata_t* Pointer to metadata (valid while file is loaded).
 */
const mp3_metadata_t *mp3player_get_metadata(void);

#endif /* MP3_PLAYER_H__ */
