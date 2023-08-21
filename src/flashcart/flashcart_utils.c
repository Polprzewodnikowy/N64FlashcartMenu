#include <libdragon.h>

#include "flashcart_utils.h"
#include "utils/fs.h"
#include "utils/utils.h"


void fix_file_size (FIL *fil) {
    // HACK: Align file size to the SD sector size to prevent FatFs from doing partial sector load.
    //       We are relying on direct transfer from SD to SDRAM without CPU intervention.
    //       Sending some extra bytes isn't an issue here.
    fil->obj.objsize = ALIGN(f_size(fil), FS_SECTOR_SIZE);
}

void pi_dma_read_data (void *src, void *dst, size_t length) {
    data_cache_hit_writeback_invalidate(dst, length);
    dma_read_async(dst, (uint32_t) (src), length);
    dma_wait();
}

void pi_dma_write_data (void *src, void *dst, size_t length) {
    assert((((uint32_t) (src)) & 0x07) == 0);
    assert((((uint32_t) (dst)) & 0x01) == 0);
    assert((length & 1) == 0);

    data_cache_hit_writeback(src, length);
    dma_write_raw_async(src, (uint32_t) (dst), length);
    dma_wait();
}
