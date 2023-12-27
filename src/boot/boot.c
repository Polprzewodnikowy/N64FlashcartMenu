#include <libdragon.h>

#include "boot_io.h"
#include "boot.h"
#include "cic.h"


#define C0_STATUS_FR    (1 << 26)
#define C0_STATUS_CU0   (1 << 28)
#define C0_STATUS_CU1   (1 << 29)


extern uint32_t reboot_start __attribute__((section(".text")));
extern size_t reboot_size __attribute__((section(".text")));


static io32_t *boot_get_device_base (boot_params_t *params) {
    io32_t *device_base_address = ROM_CART;
    if (params->device_type == BOOT_DEVICE_TYPE_64DD) {
        device_base_address = ROM_DDIPL;
    }
    return device_base_address;
}

static void boot_detect_cic_seed (boot_params_t *params) {
    io32_t *base = boot_get_device_base(params);

    uint8_t ipl3[IPL3_LENGTH] __attribute__((aligned(8)));

    data_cache_hit_writeback_invalidate(ipl3, sizeof(ipl3));
    dma_read_raw_async(ipl3, (uint32_t) (&base[16]), sizeof(ipl3));
    dma_wait();

    params->cic_seed = cic_get_seed(cic_detect(ipl3));
}


void boot (boot_params_t *params) {
    if (params->tv_type == BOOT_TV_TYPE_PASSTHROUGH) {
        switch (get_tv_type()) {
            case TV_PAL:
                params->tv_type = BOOT_TV_TYPE_PAL;
                break;
            case TV_NTSC:
                params->tv_type = BOOT_TV_TYPE_NTSC;
                break;
            case TV_MPAL:
                params->tv_type = BOOT_TV_TYPE_MPAL;
                break;
            default:
                params->tv_type = BOOT_TV_TYPE_NTSC;
                break;
        }
    }

    if (params->detect_cic_seed) {
        boot_detect_cic_seed(params);
    }

    C0_WRITE_STATUS(C0_STATUS_CU1 | C0_STATUS_CU0 | C0_STATUS_FR);

    while (!(cpu_io_read(&SP->SR) & SP_SR_HALT));

    cpu_io_write(&SP->SR,
        SP_SR_CLR_SIG7 |
        SP_SR_CLR_SIG6 |
        SP_SR_CLR_SIG5 |
        SP_SR_CLR_SIG4 |
        SP_SR_CLR_SIG3 |
        SP_SR_CLR_SIG2 |
        SP_SR_CLR_SIG1 |
        SP_SR_CLR_SIG0 |
        SP_SR_CLR_INTR_BREAK |
        SP_SR_CLR_SSTEP |
        SP_SR_CLR_INTR |
        SP_SR_CLR_BROKE |
        SP_SR_SET_HALT
    );
    cpu_io_write(&SP->SEMAPHORE, 0);
    cpu_io_write(&SP->PC, 0);

    while (cpu_io_read(&SP->DMA_BUSY));

    cpu_io_write(&PI->SR, PI_SR_CLR_INTR | PI_SR_RESET);
    while ((cpu_io_read(&VI->CURR_LINE) & ~(VI_CURR_LINE_FIELD)) != 0);
    cpu_io_write(&VI->V_INTR, 0x3FF);
    cpu_io_write(&VI->H_LIMITS, 0);
    cpu_io_write(&VI->CURR_LINE, 0);
    cpu_io_write(&AI->MADDR, 0);
    cpu_io_write(&AI->LEN, 0);

    while (cpu_io_read(&SP->SR) & SP_SR_DMA_BUSY);

    uint32_t *reboot_src = &reboot_start;
    io32_t *reboot_dst = SP_MEM->IMEM;
    size_t reboot_instructions = (size_t) (&reboot_size) / sizeof(uint32_t);

    for (int i = 0; i < reboot_instructions; i++) {
        cpu_io_write(&reboot_dst[i], reboot_src[i]);
    }

    cpu_io_write(&PI->DOM[0].LAT, 0xFF);
    cpu_io_write(&PI->DOM[0].PWD, 0xFF);
    cpu_io_write(&PI->DOM[0].PGS, 0x0F);
    cpu_io_write(&PI->DOM[0].RLS, 0x03);

    io32_t *base = boot_get_device_base(params);
    uint32_t pi_config = io_read((uint32_t) (base));

    cpu_io_write(&PI->DOM[0].LAT, pi_config & 0xFF);
    cpu_io_write(&PI->DOM[0].PWD, pi_config >> 8);
    cpu_io_write(&PI->DOM[0].PGS, pi_config >> 16);
    cpu_io_write(&PI->DOM[0].RLS, pi_config >> 20);

    if (cpu_io_read(&DPC->SR) & DPC_SR_XBUS_DMEM_DMA) {
        while (cpu_io_read(&DPC->SR) & DPC_SR_PIPE_BUSY);
    }

    io32_t *ipl3_src = base;
    io32_t *ipl3_dst = SP_MEM->DMEM;

    for (int i = 16; i < 1024; i++) {
        cpu_io_write(&ipl3_dst[i], io_read((uint32_t) (&ipl3_src[i])));
    }

    register uint32_t boot_device asm ("s3");
    register uint32_t tv_type asm ("s4");
    register uint32_t reset_type asm ("s5");
    register uint32_t cic_seed asm ("s6");
    register uint32_t version asm ("s7");

    boot_device = (params->device_type & 0x01);
    tv_type = (params->tv_type & 0x03);
    reset_type = BOOT_RESET_TYPE_COLD;
    cic_seed = (params->cic_seed & 0xFF);
    version = (params->tv_type == BOOT_TV_TYPE_PAL) ? 6
            : (params->tv_type == BOOT_TV_TYPE_NTSC) ? 1
            : (params->tv_type == BOOT_TV_TYPE_MPAL) ? 4
            : 0;

    asm volatile (
        "la $t3, reboot \n"
        "jr $t3 \n" ::
        [boot_device] "r" (boot_device),
        [tv_type] "r" (tv_type),
        [reset_type] "r" (reset_type),
        [cic_seed] "r" (cic_seed),
        [version] "r" (version) :
        "t3"
    );

    while (1);
}
