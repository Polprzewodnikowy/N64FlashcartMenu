/**
 * @file boot_io.h
 * @brief Flashcart Boot IO
 * @ingroup boot
 */

#ifndef BOOT_IO_H__
#define BOOT_IO_H__

#include <stddef.h>
#include <stdint.h>

/**
 * @typedef io8_t
 * @brief 8-bit volatile IO type.
 */
typedef volatile uint8_t io8_t;

/**
 * @typedef io32_t
 * @brief 32-bit volatile IO type.
 */
typedef volatile uint32_t io32_t;

/**
 * @brief Convert an address to its uncached equivalent.
 * 
 * This macro takes an address and converts it to its uncached equivalent
 * by setting the appropriate bits.
 * 
 * @param address The address to convert.
 * @return The uncached equivalent of the address.
 */
#define UNCACHED(address)           ((typeof(address)) (((io32_t) (address)) | (0xA0000000UL)))

/**
 * @brief Memory Structure.
 * 
 * This structure represents the memory layout for the SP (Signal Processor),
 * containing both Data Memory (DMEM) and Instruction Memory (IMEM).
 */
typedef struct {
    io32_t DMEM[1024]; /**< Data Memory (DMEM) array of 1024 32-bit words. */
    io32_t IMEM[1024]; /**< Instruction Memory (IMEM) array of 1024 32-bit words. */
} sp_mem_t;

/**
 * @brief Base address for SP memory.
 */
#define SP_MEM_BASE                 (0x04000000UL)

/**
 * @brief Pointer to the SP memory structure.
 */
#define SP_MEM                      ((sp_mem_t *) SP_MEM_BASE)

/**
 * @brief SP Registers Structure.
 * 
 * This structure represents the registers for the SP (Signal Processor).
 */
typedef struct {
    io32_t PADDR; /**< Physical Address Register. */
    io32_t MADDR; /**< Memory Address Register. */
    io32_t RD_LEN; /**< Read Length Register. */
    io32_t WR_LEN; /**< Write Length Register. */
    io32_t SR; /**< Status Register. */
    io32_t DMA_FULL; /**< DMA Full Register. */
    io32_t DMA_BUSY; /**< DMA Busy Register. */
    io32_t SEMAPHORE; /**< Semaphore Register. */
    io32_t __reserved[0xFFF8];
    io32_t PC; /**< Program Counter Register. */
} sp_regs_t;

/**
 * @brief Base address for SP registers.
 */
#define SP_BASE                     (0x04040000UL)

/**
 * @brief Pointer to the SP registers structure.
 */
#define SP                          ((sp_regs_t *) SP_BASE)

#define SP_SR_HALT                  (1 << 0)
#define SP_SR_BROKE                 (1 << 1)
#define SP_SR_DMA_BUSY              (1 << 2)
#define SP_SR_DMA_FULL              (1 << 3)
#define SP_SR_IO_FULL               (1 << 4)
#define SP_SR_SSTEP                 (1 << 5)
#define SP_SR_INTR_BREAK            (1 << 6)
#define SP_SR_SIG0                  (1 << 7)
#define SP_SR_SIG1                  (1 << 8)
#define SP_SR_SIG2                  (1 << 9)
#define SP_SR_SIG3                  (1 << 10)
#define SP_SR_SIG4                  (1 << 11)
#define SP_SR_SIG5                  (1 << 12)
#define SP_SR_SIG6                  (1 << 13)
#define SP_SR_SIG7                  (1 << 14)
#define SP_SR_CLR_HALT              (1 << 0)
#define SP_SR_SET_HALT              (1 << 1)
#define SP_SR_CLR_BROKE             (1 << 2)
#define SP_SR_CLR_INTR              (1 << 3)
#define SP_SR_SET_INTR              (1 << 4)
#define SP_SR_CLR_SSTEP             (1 << 5)
#define SP_SR_SET_SSTEP             (1 << 6)
#define SP_SR_CLR_INTR_BREAK        (1 << 7)
#define SP_SR_SET_INTR_BREAK        (1 << 8)
#define SP_SR_CLR_SIG0              (1 << 9)
#define SP_SR_SET_SIG0              (1 << 10)
#define SP_SR_CLR_SIG1              (1 << 11)
#define SP_SR_SET_SIG1              (1 << 12)
#define SP_SR_CLR_SIG2              (1 << 13)
#define SP_SR_SET_SIG2              (1 << 14)
#define SP_SR_CLR_SIG3              (1 << 15)
#define SP_SR_SET_SIG3              (1 << 16)
#define SP_SR_CLR_SIG4              (1 << 17)
#define SP_SR_SET_SIG4              (1 << 18)
#define SP_SR_CLR_SIG5              (1 << 19)
#define SP_SR_SET_SIG5              (1 << 20)
#define SP_SR_CLR_SIG6              (1 << 21)
#define SP_SR_SET_SIG6              (1 << 22)
#define SP_SR_CLR_SIG7              (1 << 23)
#define SP_SR_SET_SIG7              (1 << 24)

/**
 * @brief DPC Registers Structure.
 * 
 * This structure represents the registers for the DPC (Display Processor).
 */
typedef struct {
    io32_t START; /**< Start Register. */
    io32_t END; /**< End Register. */
    io32_t CURRENT; /**< Current Register. */
    io32_t SR; /**< Status Register. */
    io32_t CLOCK; /**< Clock Register. */
    io32_t BUF_BUSY; /**< Buffer Busy Register. */
    io32_t PIPE_BUSY; /**< Pipe Busy Register. */
    io32_t TMEM; /**< TMEM Register. */
} dpc_regs_t;

#define DPC_BASE                    (0x04100000UL)
#define DPC                         ((dpc_regs_t *) DPC_BASE)

#define DPC_SR_XBUS_DMEM_DMA        (1 << 0)
#define DPC_SR_FREEZE               (1 << 1)
#define DPC_SR_FLUSH                (1 << 2)
#define DPC_SR_START_GCLK           (1 << 3)
#define DPC_SR_TMEM_BUSY            (1 << 4)
#define DPC_SR_PIPE_BUSY            (1 << 5)
#define DPC_SR_CMD_BUSY             (1 << 6)
#define DPC_SR_CBUF_READY           (1 << 7)
#define DPC_SR_DMA_BUSY             (1 << 8)
#define DPC_SR_END_VALID            (1 << 9)
#define DPC_SR_START_VALID          (1 << 10)
#define DPC_SR_CLR_XBUS_DMEM_DMA    (1 << 0)
#define DPC_SR_SET_XBUS_DMEM_DMA    (1 << 1)
#define DPC_SR_CLR_FREEZE           (1 << 2)
#define DPC_SR_SET_FREEZE           (1 << 3)
#define DPC_SR_CLR_FLUSH            (1 << 4)
#define DPC_SR_SET_FLUSH            (1 << 5)
#define DPC_SR_CLR_TMEM_CTR         (1 << 6)
#define DPC_SR_CLR_PIPE_CTR         (1 << 7)
#define DPC_SR_CLR_CMD_CTR          (1 << 8)
#define DPC_SR_CLR_CLOCK_CTR        (1 << 9)

/**
 * @brief Video Interface Registers Structure.
 * 
 * This structure represents the registers for the Video Interface (VI).
 */
typedef struct {
    io32_t CR; /**< Control Register. */
    io32_t MADDR; /**< Memory Address. */
    io32_t H_WIDTH; /**< Horizontal Width. */
    io32_t V_INTR; /**< Vertical Interrupt. */
    io32_t CURR_LINE; /**< Current Line. */
    io32_t TIMING; /**< Timings. */
    io32_t V_SYNC; /**< Vertical Sync. */
    io32_t H_SYNC; /**< Horizontal Sync. */
    io32_t H_SYNC_LEAP; /**< Horizontal Sync Leap. */
    io32_t H_LIMITS; /**< Horizontal Limits. */
    io32_t V_LIMITS; /**< Vertical Limits. */
    io32_t COLOR_BURST; /**< Color Burst. */
    io32_t H_SCALE; /**< Horizontal Scale. */
    io32_t V_SCALE; /**< Vertical Scale. */
} vi_regs_t;

#define VI_BASE                     (0x04400000UL)
#define VI                          ((vi_regs_t *) VI_BASE)

#define VI_CR_TYPE_16               (2 << 0)
#define VI_CR_TYPE_32               (3 << 0)
#define VI_CR_GAMMA_DITHER_ON       (1 << 2)
#define VI_CR_GAMMA_ON              (1 << 3)
#define VI_CR_DIVOT_ON              (1 << 4)
#define VI_CR_SERRATE_ON            (1 << 6)
#define VI_CR_ANTIALIAS_0           (1 << 8)
#define VI_CR_ANTIALIAS_1           (1 << 9)
#define VI_CR_PIXEL_ADVANCE_0       (1 << 12)
#define VI_CR_PIXEL_ADVANCE_1       (1 << 13)
#define VI_CR_PIXEL_ADVANCE_2       (1 << 14)
#define VI_CR_PIXEL_ADVANCE_3       (1 << 15)
#define VI_CR_DITHER_FILTER_ON      (1 << 16)

#define VI_CURR_LINE_FIELD          (1 << 0)

/**
 * @brief Audio Interface Registers Structure.
 * 
 * This structure represents the registers for the Audio Interface (AI).
 */
typedef struct {
    io32_t MADDR; /**< Memory Address. */
    io32_t LEN; /**< Length of bytes. */
    io32_t CR; /**< Control Register. */
    io32_t SR; /**< Status Register. */
    io32_t DACRATE; /**< DAC rate. */
    io32_t BITRATE; /**< Bit rate. */
} ai_regs_t;

#define AI_BASE                     (0x04500000UL)
#define AI                          ((ai_regs_t *) AI_BASE)

#define AI_SR_DMA_BUSY              (1 << 30)
#define AI_SR_FIFO_FULL             (1 << 31)
#define AI_CR_DMA_ON                (1 << 0)

/**
 * @brief Peripheral Interface Register Structure.
 * 
 * This structure represents the registers for the Peripheral Interface (PI).
 */
typedef struct {
    io32_t MADDR; /**< Memory Address. */
    io32_t PADDR; /**< Cart Address. */
    io32_t RDMA; /**< Read Length. */
    io32_t WDMA; /**< Write Length. */
    io32_t SR; /**< Status Register. */
    struct {
        io32_t LAT; /**< Latch Value. */
        io32_t PWD; /**< Pulse Width Value. */
        io32_t PGS; /**< Page Size Value. */
        io32_t RLS; /**< Release Value. */
    } DOM[2]; /**< Domain 2 Registers. */
} pi_regs_t;

#define PI_BASE                     (0x04600000UL)
#define PI                          ((pi_regs_t *) PI_BASE)

#define PI_SR_DMA_BUSY              (1 << 0)
#define PI_SR_IO_BUSY               (1 << 1)
#define PI_SR_DMA_ERROR             (1 << 2)
#define PI_SR_RESET                 (1 << 0)
#define PI_SR_CLR_INTR              (1 << 1)

#define ROM_DDIPL_BASE              (0x06000000UL)
#define ROM_DDIPL                   ((io32_t *) ROM_DDIPL_BASE)

#define ROM_CART_BASE               (0x10000000UL)
#define ROM_CART                    ((io32_t *) ROM_CART_BASE)

/**
 * @brief Read a value from a CPU IO address.
 * 
 * @param address The address to read from.
 * @return uint32_t The value read from the address.
 */
static inline uint32_t cpu_io_read (io32_t *address) {
    io32_t *uncached = UNCACHED(address);
    uint32_t value = *uncached;
    return value;
}

/**
 * @brief Write a value to a CPU IO address.
 * 
 * @param address The address to write to.
 * @param value The value to write.
 */
static inline void cpu_io_write (io32_t *address, uint32_t value) {
    io32_t *uncached = UNCACHED(address);
    *uncached = value;
}

#endif /* BOOT_IO_H__ */
