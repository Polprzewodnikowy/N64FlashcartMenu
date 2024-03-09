#include <fatfs/ff.h>
#include <libdragon.h>

#include "rom_patcher.h"


rom_patcher_err_t rom_patcher_load_file (char *path)
{
    // ROM file should be loaded before patch is applied.
    // See https://github.com/marcrobledo/RomPatcher.js/ for an example lib.
    // Though needs conversion from JS to C
    // apply patch dependent on extension.
    //return apply_patch_type_bps(path);
    //return apply_patch_type_ips(path);
    //return apply_patch_type_aps(path);
    //return apply_patch_type_ups(path);
    //return apply_patch_type_xdelta(path);
    // aps should be PATCH_ERR_UNSUPPORTED; as not really a thing?!
    return PATCH_ERR_IO;
}

rom_patcher_err_t apply_patch_type_bps(char *path)
{
    // https://github.com/Alcaro/Flips/blob/master/bps_spec.md
    // https://www.romhacking.net/documents/746/
    return PATCH_ERR_UNSUPPORTED;
}

rom_patcher_err_t apply_patch_type_ips(char *path)
{
    // http://www.smwiki.net/wiki/IPS_file_format
    return PATCH_ERR_INVALID;
}

rom_patcher_err_t apply_patch_type_aps(char *path)
{
    // https://github.com/btimofeev/UniPatcher/wiki/APS-(N64)
    return PATCH_ERR_INVALID;
}

rom_patcher_err_t apply_patch_type_ups(char *path)
{
    // http://www.romhacking.net/documents/392/
    return PATCH_ERR_UNSUPPORTED;
}

rom_patcher_err_t apply_patch_type_xdelta(char *path)
{
    return PATCH_ERR_UNSUPPORTED;
}



// Krikzz implementation:
// #define APS_BUFF_SIZE 32768

// static uint8_t aps_buff[APS_BUFF_SIZE];
// static uint8_t rom_buff[512];
// static uint8_t aps_byte;
// static uint8_t aps_resp;
// static uint32_t aps_addr;
// static uint32_t rom_addr;
// static uint32_t current_rom_sector;
// static uint8_t asp_swap;

// uint8_t apsGetNextByte();
// void apsSetRomByte();
// void apsSetRomByteRLE(uint8_t val);

// uint8_t ips_patch_apply(uint8_t *filename) {

//     uint32_t block_len;
//     uint8_t resp;
//     uint32_t len;
//     uint32_t i;
//     uint8_t rle_val;

//     rom_addr = 0;
//     aps_addr = 0;
//     current_rom_sector = 0;
//     // read the ROM memory
//     bi_dma_read_rom(rom_buff, current_rom_sector, 1);

//     // open the patch
//     resp = fatOpenFileByName(filename, 0);
//     if (resp)return resp;

//     len = file.size - 3;

//     // check the header content
//     for (i = 0; i < 5; i++) {
//         if (apsGetNextByte() != "PATCH"[i])return ERR_WRONG_IPS;
//     }


//     // if (aps_header[5] != 0 || aps_header[6] != 0 || aps_header[7] != 16 || aps_header[8] != 0 || aps_header[9] != 8)return ERR_WRONG_IPS2;

//     asp_swap = 0;


//     // for (i = 0; i < 5; i++)apsGetNextByte();
//     // gSetXY(4, 4);

//     while (aps_addr < len) {



//         rom_addr = apsGetNextByte() << 16;
//         rom_addr |= apsGetNextByte() << 8;
//         rom_addr |= apsGetNextByte();

//         block_len = apsGetNextByte() << 8;
//         block_len |= apsGetNextByte();
//         if (aps_resp)return aps_resp;

//         if (block_len == 0) {
//             block_len = apsGetNextByte() << 8;
//             block_len |= apsGetNextByte();
//             rle_val = apsGetNextByte();
//             if (aps_resp)return aps_resp;

//             while (block_len--) {
//                 apsSetRomByteRLE(rle_val);
//             }
//             continue;
//         }


//         while (block_len--) {

//             apsSetRomByte();
//             if (aps_resp)return aps_resp;
//         }


//     }

//     // write to the ROM memory
//     bi_dma_write_rom(rom_buff, current_rom_sector, 1);

//     return 0;
// }

// uint8_t aps_patch_apply(uint8_t *filename) {

//     uint16_t block_len;
//     uint8_t resp;
//     uint32_t len;
//     uint32_t i;
//     uint8_t aps_header[78];
//     uint8_t rle_val;


//     rom_addr = 0;
//     aps_addr = 0;
//     current_rom_sector = 0;
//     bi_dma_read_rom(rom_buff, current_rom_sector, 1);

//     resp = fatOpenFileByName(filename, 0);
//     if (resp)return resp;

//     len = file.size;




//     for (i = 0; i < 78; i++) {
//         resp = apsGetNextByte();
//         if (aps_resp)return aps_resp;
//         aps_header[i] = aps_byte;
//     }

//     for (i = 0; i < 5; i++) {
//         if (aps_header[i] != "APS10"[i])return ERR_WRONG_APS;
//     }

//     for (i = 0; i < 8; i++) {
//         if (rom_buff[i + 0x10] != aps_header[i + 0x3d])return ERR_APS_CRC;
//     }

//     asp_swap = aps_header[57] == 0 ? 1 : 0;

//     while (aps_addr < len) {

//         rom_addr = apsGetNextByte();
//         rom_addr |= apsGetNextByte() << 8;
//         rom_addr |= apsGetNextByte() << 16;
//         rom_addr |= apsGetNextByte() << 24;
//         block_len = apsGetNextByte();
//         if (aps_resp)return aps_resp;

//         if (block_len == 0) {
//             block_len = apsGetNextByte();
//             rle_val = apsGetNextByte();
//             if (aps_resp)return aps_resp;

//             while (block_len--) {
//                 apsSetRomByteRLE(rle_val);
//             }
//             continue;
//         }


//         while (block_len--) {

//             apsSetRomByte();
//             if (aps_resp)return aps_resp;
//         }


//     }

//     bi_dma_write_rom(rom_buff, current_rom_sector, 1);

//     return 0;
// }

// inline uint8_t apsGetNextByte() {

//     static uint32_t in_buff_addr;
//     in_buff_addr = aps_addr & (APS_BUFF_SIZE - 1);

//     if (in_buff_addr == 0) {

//         if (file.sec_available < APS_BUFF_SIZE / 512) {
//             aps_resp = fatReadFile(aps_buff, file.sec_available);
//         } else {
//             aps_resp = fatReadFile(aps_buff, APS_BUFF_SIZE / 512);
//         }

//         if (aps_resp)return 0;
//     }


//     aps_byte = aps_buff[in_buff_addr];
//     aps_addr++;

//     return aps_byte;
// }

// inline void apsSetRomByte() {


//     if (rom_addr / 512 != current_rom_sector) {
//         bi_dma_write_rom(rom_buff, current_rom_sector, 1);
//         current_rom_sector = rom_addr / 512;
//         bi_dma_read_rom(rom_buff, current_rom_sector, 1);
//     }


//     if (asp_swap) {
//         rom_buff[(rom_addr & 511) ^ 1] = apsGetNextByte();
//     } else {
//         rom_buff[rom_addr & 511] = apsGetNextByte();
//     }
//     rom_addr++;
// }

// inline void apsSetRomByteRLE(uint8_t val) {

//     if (rom_addr / 512 != current_rom_sector) {
//         bi_dma_write_rom(rom_buff, current_rom_sector, 1);
//         current_rom_sector = rom_addr / 512;
//         bi_dma_read_rom(rom_buff, current_rom_sector, 1);
//     }

//     if (asp_swap) {
//         rom_buff[(rom_addr & 511) ^ 1] = val;
//     } else {
//         rom_buff[rom_addr & 511] = val;
//     }
//     rom_addr++;
// }



// uint8_t patch_apply(uint8_t *filename) {


//     if (osExtension(".ips", filename)) {
//         return ips_patch_apply(filename);
//     } else if (osExtension(".aps", filename)) {
//         return aps_patch_apply(filename);
//     }

//     return 0;
// }
