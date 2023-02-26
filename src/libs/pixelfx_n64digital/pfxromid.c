/*
    Based on libdragon
    This implementation does not disable interrupts during PIF operation!
*/

#include <stdint.h>
#include <string.h>
#include "pfxromid.h"
#include "libdragon.h"

#define SI_STATUS_DMA_BUSY  (1 << 0)
#define SI_STATUS_IO_BUSY   (1 << 1)

#define MEMORY_BARRIER() asm volatile ("" : : : "memory")

#define cache_op(op, linesize) ({ \
    if (length) { \
        void *cur = (void*)((unsigned long)addr & ~(linesize-1)); \
        int count = (int)length + (addr-cur); \
        for (int i = 0; i < count; i += linesize) \
            asm ("\tcache %0,(%1)\n"::"i" (op), "r" (cur+i)); \
    } \
})

#define UncachedAddr(_addr) ((void *)(((unsigned long)(_addr))|0x20000000))

typedef struct SI_regs_s {
    volatile void * DRAM_addr;
    volatile void * PIF_addr_read;
    uint32_t reserved1;
    uint32_t reserved2;
    volatile void * PIF_addr_write;
    uint32_t reserved3;
    uint32_t status;
} SI_regs_t;

static volatile struct SI_regs_s * const SI_regs = (struct SI_regs_s *)0xa4800000;
static void * const PIF_RAM = (void *)0x1fc007c0;

// Uses the libdragon header function!!!
// static void data_cache_hit_writeback_invalidate(volatile void * addr, unsigned long length)
// {
//     cache_op(0x15, 16);
// }

static void __SI_DMA_wait(void)
{
    while (SI_regs->status & (SI_STATUS_DMA_BUSY | SI_STATUS_IO_BUSY));
}

static void __controller_exec_PIF(void *inblock) { //TODO: this is (nearly) the same as the RTC code (duplicate) use generic function or just libdragon?!)
    volatile uint64_t inblock_temp[8];
    volatile uint64_t outblock_temp[8];

    data_cache_hit_writeback_invalidate(inblock_temp, 64);
    memcpy(UncachedAddr(inblock_temp), inblock, 64);

    /* Be sure another thread doesn't get into a resource fight */
    disable_interrupts();

    __SI_DMA_wait();

    SI_regs->DRAM_addr = inblock_temp; // only cares about 23:0
    MEMORY_BARRIER();
    SI_regs->PIF_addr_write = PIF_RAM; // is it really ever anything else?
    MEMORY_BARRIER();

    __SI_DMA_wait();

    data_cache_hit_writeback_invalidate(outblock_temp, 64);

    SI_regs->DRAM_addr = outblock_temp;
    MEMORY_BARRIER();
    SI_regs->PIF_addr_read = PIF_RAM;
    MEMORY_BARRIER();

    __SI_DMA_wait();

    /* Now that we've copied, its safe to let other threads go */
    enable_interrupts();
}

static void send_raw_command(int controller, int command, int bytesout, unsigned char *out)
{
    unsigned long long SI_read_controllers_block[8] = { 0, 0, 0, 0, 0, 0, 0, 1 };
    uint8_t *data = (uint8_t *)SI_read_controllers_block;

    data[controller + 0] = bytesout + 1;
    data[controller + 1] = 1;
    data[controller + 2] = command;

    memcpy( &data[controller + 3], out, bytesout );
    data[controller + 3 + bytesout] = 0xFE;
    data[controller + 3 + bytesout + 1] = 0xFF;

    __controller_exec_PIF(SI_read_controllers_block);
}

void pfx_send_game_id(uint8_t* crc_hi, uint8_t* crc_lo, uint8_t media_format, uint8_t country_id)
{
    uint8_t out[10];

    memcpy(out, crc_hi, 4);
    memcpy(&out[4], crc_lo, 4);
    
    out[8] = media_format;
    out[9] = country_id;

    send_raw_command(0, 0x1D, sizeof(out), out);
}
