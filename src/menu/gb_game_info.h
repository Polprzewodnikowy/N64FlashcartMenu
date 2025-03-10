#ifndef ___SELF_GB_INFO_H
#define ___SELF_GB_INFO_H

typedef struct {
    const char *key;  // JSON key
    const char *game_name;  // Game Name
    const char *notes;  // Notes
    const char *game_code;  // Game Code
    unsigned int release_code;  // Release Code
    unsigned int rom_size;  // ROM Size
    const char *language;  // Language
    const char *region;  // Region
    const char *image_path;  // Image path in the dfs
} GameInfo;

GameInfo search_gb_info_by_game_code(char *search_value);

#endif