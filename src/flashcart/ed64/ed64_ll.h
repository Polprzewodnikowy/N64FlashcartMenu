/**
 * @file flashcart.h
 * @brief ED64 V-Series Flashcart Internal Utilities
 * @ingroup flashcart 
 */

#ifndef FLASHCART_ED64_LL_H__
#define FLASHCART_ED64_LL_H__

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/**
 * @addtogroup ed64
 * @{
 */


typedef enum {
    SAVE_TYPE_NONE = 0,
    SAVE_TYPE_SRAM = 1,
    SAVE_TYPE_SRAM_128K = 2,
    SAVE_TYPE_EEPROM_4K = 3,
    SAVE_TYPE_EEPROM_16K = 4,
    SAVE_TYPE_FLASHRAM = 5,
    SAVE_TYPE_CPAK = 8,
    SAVE_TYPE_DD64 = 16,
} ed64_save_type_t;

typedef enum {
    MEMPAK_MODE = 0,
    EEPROM_MODE = 1,
    SRAM_MODE = 2,
} ed64_save_transfer_mode; 


#define ROM_ADDRESS  (0xB0000000)

/* Save functions */
void ed64_ll_set_ram_bank(uint8_t bank);
ed64_save_type_t ed64_ll_get_save_type();
void ed64_ll_set_save_type(ed64_save_type_t type);

/** @} */ /* ed64 */


//sram.h

void data_cache_hit_writeback_invalidate(volatile void *, unsigned long);
unsigned int CRC_Calculate(unsigned int crc, unsigned char* buf, unsigned int len);
void dma_write_sram(void* src, unsigned long offset, unsigned long size);
void dma_read_sram(void *dest, unsigned long offset, unsigned long size);
void dma_write_s(void * ram_address, unsigned long pi_address, unsigned long len);
void dma_read_s(void * ram_address, unsigned long pi_address, unsigned long len);
int writeSram(void* src, unsigned long size);
void setSDTiming(void);


void PI_Init(void);
void PI_Init_SRAM(void);
void PI_DMAWait(void);
void PI_DMAFromCart(void* dest, void* src, unsigned long size);
void PI_DMAToCart(void* dest, void* src, unsigned long size);
void PI_DMAFromSRAM(void *dest, unsigned long offset, unsigned long size);
void PI_DMAToSRAM(void* src, unsigned long offset, unsigned long size);
void PI_SafeDMAFromCart(void *dest, void *src, unsigned long size);

//memory
/*** MEMORY ***/
void *safe_memalign(size_t boundary, size_t size);
void *safe_calloc(size_t nmemb, size_t size);
void *safe_malloc(size_t size);
void safe_free(void *ptr);
void *safe_memset(void *s, int c, size_t n);
void *safe_memcpy(void *dest, const void *src, size_t n);


#define DP_BASE_REG		0x04100000
#define VI_BASE_REG		0x04400000
#define PI_BASE_REG		0x04600000
#define PIF_RAM_START		0x1FC007C0


/*
 * PI status register has 3 bits active when read from (PI_STATUS_REG - read)
 *	Bit 0: DMA busy - set when DMA is in progress
 *	Bit 1: IO busy  - set when IO is in progress
 *	Bit 2: Error    - set when CPU issues IO request while DMA is busy
 */

#define PI_STATUS_REG		(PI_BASE_REG+0x10)

/* PI DRAM address (R/W): starting RDRAM address */
#define PI_DRAM_ADDR_REG	(PI_BASE_REG+0x00)	/* DRAM address */

/* PI pbus (cartridge) address (R/W): starting AD16 address */
#define PI_CART_ADDR_REG	(PI_BASE_REG+0x04)

/* PI read length (R/W): read data length */
#define PI_RD_LEN_REG		(PI_BASE_REG+0x08)

/* PI write length (R/W): write data length */
#define PI_WR_LEN_REG		(PI_BASE_REG+0x0C)

/*
 * PI status (R): [0] DMA busy, [1] IO busy, [2], error
 *           (W): [0] reset controller (and abort current op), [1] clear intr
 */

#define PI_BSD_DOM1_LAT_REG	(PI_BASE_REG+0x14)

/* PI dom1 pulse width (R/W): [7:0] domain 1 device R/W strobe pulse width */
#define PI_BSD_DOM1_PWD_REG	(PI_BASE_REG+0x18)

/* PI dom1 page size (R/W): [3:0] domain 1 device page size */
#define PI_BSD_DOM1_PGS_REG	(PI_BASE_REG+0x1C)    /*   page size */

/* PI dom1 release (R/W): [1:0] domain 1 device R/W release duration */
#define PI_BSD_DOM1_RLS_REG	(PI_BASE_REG+0x20)
/* PI dom2 latency (R/W): [7:0] domain 2 device latency */
#define PI_BSD_DOM2_LAT_REG	(PI_BASE_REG+0x24)    /* Domain 2 latency */

/* PI dom2 pulse width (R/W): [7:0] domain 2 device R/W strobe pulse width */
#define PI_BSD_DOM2_PWD_REG	(PI_BASE_REG+0x28)    /*   pulse width */

/* PI dom2 page size (R/W): [3:0] domain 2 device page size */
#define PI_BSD_DOM2_PGS_REG	(PI_BASE_REG+0x2C)    /*   page size */

/* PI dom2 release (R/W): [1:0] domain 2 device R/W release duration */
#define PI_BSD_DOM2_RLS_REG	(PI_BASE_REG+0x30)    /*   release duration */


#define	PI_DOMAIN1_REG		PI_BSD_DOM1_LAT_REG
#define	PI_DOMAIN2_REG		PI_BSD_DOM2_LAT_REG


#define	PI_STATUS_ERROR		0x04
#define	PI_STATUS_IO_BUSY	0x02
#define	PI_STATUS_DMA_BUSY	0x01

#define                      DPC_START                    (DP_BASE_REG + 0x00)
#define                      DPC_END                      (DP_BASE_REG + 0x04)
#define                      DPC_CURRENT                  (DP_BASE_REG + 0x08)
#define                      DPC_STATUS                   (DP_BASE_REG + 0x0C)
#define                      DPC_CLOCK                    (DP_BASE_REG + 0x10)
#define                      DPC_BUFBUSY                  (DP_BASE_REG + 0x14)
#define                      DPC_PIPEBUSY                 (DP_BASE_REG + 0x18)
#define                      DPC_TMEM                     (DP_BASE_REG + 0x1C)

#define	VI_CONTROL	(VI_BASE_REG + 0x00)
#define	VI_FRAMEBUFFER	(VI_BASE_REG + 0x04)
#define	VI_WIDTH	(VI_BASE_REG + 0x08)
#define	VI_V_INT	(VI_BASE_REG + 0x0C)
#define	VI_CUR_LINE	(VI_BASE_REG + 0x10)
#define	VI_TIMING	(VI_BASE_REG + 0x14)
#define	VI_V_SYNC	(VI_BASE_REG + 0x18)
#define	VI_H_SYNC	(VI_BASE_REG + 0x1C)
#define	VI_H_SYNC2	(VI_BASE_REG + 0x20)
#define	VI_H_LIMITS	(VI_BASE_REG + 0x24)
#define	VI_COLOR_BURST	(VI_BASE_REG + 0x28)
#define	VI_H_SCALE	(VI_BASE_REG + 0x2C)
#define	VI_VSCALE	(VI_BASE_REG + 0x30)

#define	PHYS_TO_K0(x)	((unsigned long)(x)|0x80000000)	/* physical to kseg0 */
#define	K0_TO_PHYS(x)	((unsigned long)(x)&0x1FFFFFFF)	/* kseg0 to physical */
#define	PHYS_TO_K1(x)	((unsigned long)(x)|0xA0000000)	/* physical to kseg1 */
#define	K1_TO_PHYS(x)	((unsigned long)(x)&0x1FFFFFFF)	/* kseg1 to physical */

#define	IO_READ(addr)		(*(volatile unsigned long*)PHYS_TO_K1(addr))
#define	IO_WRITE(addr,data)	(*(volatile unsigned long*)PHYS_TO_K1(addr)=(unsigned long)(data))

#define FRAM_EXECUTE_CMD		0xD2000000
#define FRAM_STATUS_MODE_CMD	0xE1000000
#define FRAM_ERASE_OFFSET_CMD	0x4B000000
#define FRAM_WRITE_OFFSET_CMD	0xA5000000
#define FRAM_ERASE_MODE_CMD		0x78000000
#define FRAM_WRITE_MODE_CMD		0xB4000000
#define FRAM_READ_MODE_CMD		0xF0000000

#define FRAM_STATUS_REG	0xA8000000
#define FRAM_COMMAND_REG 0xA8010000

int getSRAM( uint8_t *buffer, int size);
int getEeprom(  uint8_t *buffer, int size);

int setSRAM( uint8_t *buffer, int size);
int setEeprom( uint8_t *buffer, int size);




#endif
