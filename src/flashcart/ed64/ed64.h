/**
 * @file ed64.h
 * @brief EverDrive-64 V-series flashcart support
 * @ingroup flashcart
 */

#ifndef FLASHCART_ED64_H__
#define FLASHCART_ED64_H__

#include "../flashcart.h"

flashcart_t *ed64_get_flashcart (void);

void ed64_post_init (void);

const char *ed64_get_hardware_variant (void);

#endif
