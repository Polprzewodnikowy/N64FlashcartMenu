#include "flashcart_utils.h"
#include "utils/fs.h"
#include "utils/utils.h"


void fix_file_size (FIL *fil) {
    // HACK: Align file size to the SD sector size to prevent FatFs from doing partial sector load.
    //       We are relying on direct transfer from SD to SDRAM without CPU intervention.
    //       Sending some extra bytes isn't an issue here.
    fil->obj.objsize = ALIGN(f_size(fil), FS_SECTOR_SIZE);
}
