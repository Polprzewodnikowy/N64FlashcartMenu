/**
 * @file components.h
 * @brief Menu Components
 * @ingroup menu
 */

#ifndef COMPONENTS_H__
#define COMPONENTS_H__


#include <libdragon.h>


/**
 * @addtogroup 
 * @{ menu_components
 */

typedef struct {
    char *cache_location;
    surface_t *image;
    rspq_block_t *image_display_list;
} component_background_t;

component_background_t *component_background_create (char *cache_location);
void component_background_replace_image (component_background_t *c, surface_t *image);
void component_background_draw (component_background_t *c);

/** @} */ /* menu_components */


#endif
