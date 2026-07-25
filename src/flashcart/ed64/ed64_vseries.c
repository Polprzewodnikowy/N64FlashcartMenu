#include "ed64.h"
#include "ed64_vseries.h"

flashcart_t *ed64_vseries_get_flashcart (void) {
    return ed64_get_flashcart();
}

void ed64_vseries_post_init (void) {
    ed64_post_init();
}

const char *ed64_vseries_get_hardware_variant (void) {
    return ed64_get_hardware_variant();
}
