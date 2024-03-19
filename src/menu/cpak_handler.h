/**
 * @file cpak_handler.h
 * @brief ControllerPak handler
 * @ingroup menu
 */

#include <stdbool.h>
#include <stdint.h>

#ifndef CPAK_HANDLER_H__
#define CPAK_HANDLER_H__

/** @brief Controller Pak state enumeration. */
typedef enum {
    CONTROLLER_PAK_OK,
    CONTROLLER_PAK_ERR_IO,
    CONTROLLER_PAK_ERR_NO_FILE,
    CONTROLLER_PAK_ERR_INVALID,
} cpak_err_t;

/** @brief Controller Pak Information Structure. */
typedef struct {
    entry_structure_t entries[16];
    int free_space;

} cpak_info_t;

cpak_err_t cpak_info_load(uint8_t port, cpak_info_t *cpak_info);
int cpak_clone_contents_to_file(char *path, uint8_t port);
cpak_err_t cpak_overwrite_contents_from_file(char *path, uint8_t port);
cpak_err_t cpak_attempt_repair();

#endif
