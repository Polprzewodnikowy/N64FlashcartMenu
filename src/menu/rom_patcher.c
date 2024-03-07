#include <fatfs/ff.h>
#include <libdragon.h>

#include "rom_patcher.h"


rom_patcher_err_t rom_patcher_load_file (char *path)
{
    // ROM file should be loaded befoe patch is applied.
    // apply patch dependent on extension.
    //return apply_patch_type_ips(path);
    //return apply_patch_type_bps(path);
    //return apply_patch_type_aps(path);
    //return apply_patch_type_xdelta(path);
    return PATCH_ERR_IO;
}

rom_patcher_err_t apply_patch_type_ips(char *path)
{
    return PATCH_ERR_INVALID;
}

rom_patcher_err_t apply_patch_type_bps(char *path)
{
    return PATCH_ERR_INVALID;
}

rom_patcher_err_t apply_patch_type_aps(char *path)
{
    return PATCH_ERR_INVALID;
}

rom_patcher_err_t apply_patch_type_xdelta(char *path)
{
    return PATCH_ERR_UNSUPPORTED;
}
