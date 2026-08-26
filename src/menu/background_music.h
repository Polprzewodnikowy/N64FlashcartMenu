/**
 * @file background_music.h
 * @brief Menu BGM
 * @ingroup menu 
 */

#ifndef BGM_H__
#define BGM_H__


/**
 * @brief Initialize BGM.
 * 
 * This function initializes the BGM used in the menu system. It can load
 * custom muic files from the specified path.
 * 
 * @param custom_bgm_path Path to the custom font file.
 */
void bgm_init(char *custom_bgm_path);

#endif /* BGM_H__ */
