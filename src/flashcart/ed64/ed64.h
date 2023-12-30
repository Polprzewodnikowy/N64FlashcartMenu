/**
 * @file ed64.h
 * @brief ED64 Flashcart Utilities
 * @ingroup flashcart 
 */

#ifndef FLASHCART_ED64_H__
#define FLASHCART_ED64_H__


#include "../flashcart.h"


/**
 * @addtogroup ed64
 * @{
 */

typedef struct {
    uint16_t hardware_series;
    uint16_t hardware_revision;
    uint16_t fpga_firmware_version;
    uint16_t cpld_firmware_version;
    uint16_t manufacture_date;
    uint16_t manufacture_time;
    uint16_t serial_number;
} flashcart_information_type;

flashcart_t *ed64_get_flashcart (void);

/** @} */ /* ed64 */


#endif
