/***
 * @file cheat_load.h
 * @brief Cheat loading functions
 */

#include "path.h"
#include "utils/fs.h"
#include "utils/utils.h"
#include "menu_state.h"

/** @brief Cheat code loading enum */

typedef enum {
    CHEAT_LOAD_OK,
    CHEAT_LOAD_ERR_NO_CHEAT_FILE,
    CHEAT_LOAD_ERR_SIZE_FAILED,
    CHEAT_LOAD_ERR_CHEAT_EMPTY,
    CHEAT_LOAD_ERR_CHEAT_TOO_LARGE,
    CHEAT_LOAD_ERR_MALLOC_FAILED,
    CHEAT_LOAD_ERR_READ_FAILED,
    CHEAT_LOAD_ERR_CLOSE_FAILED,
    CHEAT_LOAD_ERR_UNKNOWN_ERROR
} cheat_load_err_t;

cheat_load_err_t load_cheats(menu_t *menu);
char *cheat_load_convert_error_message (cheat_load_err_t err);