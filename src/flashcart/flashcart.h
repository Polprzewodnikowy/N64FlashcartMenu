/**
 * @file flashcart.h
 * @brief Flashcart Subsystem
 * @ingroup flashcart 
 */

#ifndef FLASHCART_H__
#define FLASHCART_H__


#include <stdbool.h>
#include <stdint.h>


/** @brief Flashcart error enumeration */
typedef enum {
    FLASHCART_OK,
    FLASHCART_ERR_NOT_DETECTED,
    FLASHCART_ERR_OUTDATED,
    FLASHCART_ERR_SD_CARD,
    FLASHCART_ERR_ARGS,
    FLASHCART_ERR_LOAD,
    FLASHCART_ERR_INT,
    FLASHCART_ERR_FUNCTION_NOT_SUPPORTED,
} flashcart_err_t;

/** @brief List of optional supported flashcart features */
typedef enum {
    FLASHCART_FEATURE_64DD,
    FLASHCART_FEATURE_RTC,
    FLASHCART_FEATURE_USB,
} flashcart_features_t;

/** @brief Flashcart save type enumeration */
typedef enum {
    FLASHCART_SAVE_TYPE_NONE,
    FLASHCART_SAVE_TYPE_EEPROM_4K,
    FLASHCART_SAVE_TYPE_EEPROM_16K,
    FLASHCART_SAVE_TYPE_SRAM,
    FLASHCART_SAVE_TYPE_SRAM_BANKED,
    FLASHCART_SAVE_TYPE_SRAM_128K,
    FLASHCART_SAVE_TYPE_FLASHRAM,
    FLASHCART_SAVE_TYPE_FLASHRAM_PKST2,
    __FLASHCART_SAVE_TYPE_END
} flashcart_save_type_t;

/** @brief Flashcart Disk Parameter Structure. */
typedef struct {
    bool development_drive;
    uint8_t disk_type;
    bool bad_system_area_lbas[24];
    uint8_t defect_tracks[16][12];
} flashcart_disk_parameters_t;

typedef void flashcart_progress_callback_t (float progress);

/** @brief Flashcart Structure */
typedef struct {
    /** @brief The flashcart initialization function */
    flashcart_err_t (*init) (void);
    /** @brief The flashcart de-initialization function */
    flashcart_err_t (*deinit) (void);
    /** @brief The flashcart feature function */
    bool (*has_feature) (flashcart_features_t feature);
    /** @brief The flashcart ROM load function */
    flashcart_err_t (*load_rom) (char *rom_path, flashcart_progress_callback_t *progress);
    /** @brief The flashcart file load function */
    flashcart_err_t (*load_file) (char *file_path, uint32_t rom_offset, uint32_t file_offset);
    /** @brief The flashcart save file load function */
    flashcart_err_t (*load_save) (char *save_path);
    /** @brief The flashcart disk bios load function */
    flashcart_err_t (*load_64dd_ipl) (char *ipl_path, flashcart_progress_callback_t *progress);
    /** @brief The flashcart disk load function */
    flashcart_err_t (*load_64dd_disk) (char *disk_path, flashcart_disk_parameters_t *disk_parameters);
    /** @brief The flashcart set save type function */
    flashcart_err_t (*set_save_type) (flashcart_save_type_t save_type);
    /** @brief The flashcart set save writeback function */
    flashcart_err_t (*set_save_writeback) (uint32_t *sectors);
} flashcart_t;


char *flashcart_convert_error_message (flashcart_err_t err);
flashcart_err_t flashcart_init (void);
flashcart_err_t flashcart_deinit (void);
bool flashcart_has_feature (flashcart_features_t feature);
flashcart_err_t flashcart_load_rom (char *rom_path, bool byte_swap, flashcart_progress_callback_t *progress);
flashcart_err_t flashcart_load_file (char *file_path, uint32_t rom_offset, uint32_t file_offset);
flashcart_err_t flashcart_load_save (char *save_path, flashcart_save_type_t save_type);
flashcart_err_t flashcart_load_64dd_ipl (char *ipl_path, flashcart_progress_callback_t *progress);
flashcart_err_t flashcart_load_64dd_disk (char *disk_path, flashcart_disk_parameters_t *disk_parameters);


#endif
