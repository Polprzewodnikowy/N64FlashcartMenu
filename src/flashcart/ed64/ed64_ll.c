#include <malloc.h>
#include <libdragon.h>
#include "utils/utils.h"
#include "ed64_ll.h"

//FIXME find a better solution, perferably a libdragon one
void sleep (unsigned long ms);

void sleep (unsigned long ms) {

    unsigned long current_ms = get_ticks_ms();

    while (get_ticks_ms() - current_ms < ms);

}

/* ED64 configuration registers base address  */
#define ED64_CONFIG_REGS_BASE (0xA8040000)

typedef enum {
    // REG_CFG = 0,
    // REG_STATUS = 1,
    // REG_DMA_LENGTH = 2,
    // REG_DMA_RAM_ADDR = 3,
    // REG_MSG = 4,
    // REG_DMA_CFG = 5,
    // REG_SPI = 6,
    // REG_SPI_CFG = 7,
    // REG_KEY = 8,
    REG_SAV_CFG = 9,
    // REG_SEC = 10, /* Sectors?? */
    // REG_FPGA_VERSION = 11, /* Altera (Intel) MAX */
    // REG_GPIO = 12,

} ed64_registers_t;

void pi_initialize (void);
void pi_dma_wait (void);
void pi_initialize_sram (void);
void pi_dma_from_sram (void *dest, unsigned long offset, unsigned long size);
void pi_dma_to_sram (void* src, unsigned long offset, unsigned long size);
void ed64_ll_set_sdcard_timing (void);


#define SAV_EEP_ON 1
#define SAV_SRM_ON 2
#define SAV_EEP_SIZE 4
#define SAV_SRM_SIZE 8

#define SAV_RAM_BANK 128
#define SAV_RAM_BANK_APPLY 32768

void ed64_ll_reg_write (uint32_t reg, uint32_t data);

void ed64_ll_reg_write (uint32_t reg, uint32_t data) {

    *(volatile uint32_t *) (ED64_CONFIG_REGS_BASE);
    *(volatile uint32_t *) (ED64_CONFIG_REGS_BASE + reg * 4) = data;
    *(volatile uint32_t *) (ROM_ADDRESS);

}

uint8_t ed64_ll_sram_bank;
ed64_save_type_t ed64_ll_save_type;


ed64_save_type_t ed64_ll_get_save_type (void) {

    return ed64_ll_save_type;

}

void ed64_ll_set_save_type (ed64_save_type_t type) {

    uint16_t save_cfg;
    uint8_t eeprom_on, sram_on, eeprom_size, sram_size, ram_bank;
    ed64_ll_save_type = type;
    eeprom_on = 0;
    sram_on = 0;
    eeprom_size = 0;
    sram_size = 0;
    ram_bank = ed64_ll_sram_bank;


    switch (type) {
        case SAVE_TYPE_EEPROM_16K:
            eeprom_on = 1;
            eeprom_size = 1;
            break;
        case SAVE_TYPE_EEPROM_4K:
            eeprom_on = 1;
            break;
        case SAVE_TYPE_SRAM:
            sram_on = 1;
            break;
        case SAVE_TYPE_SRAM_128K:
            sram_on = 1;
            sram_size = 1;
            break;
        case SAVE_TYPE_FLASHRAM:
            sram_on = 0;
            sram_size = 1;
            break;
        default:
            sram_on = 0;
            sram_size = 0;
            ram_bank = 1;
            break;
    }

    if (eeprom_on)save_cfg |= SAV_EEP_ON;
    if (sram_on)save_cfg |= SAV_SRM_ON;
    if (eeprom_size)save_cfg |= SAV_EEP_SIZE;
    if (sram_size)save_cfg |= SAV_SRM_SIZE;
    if (ram_bank)save_cfg |= SAV_RAM_BANK;
    save_cfg |= SAV_RAM_BANK_APPLY;

    ed64_ll_reg_write(REG_SAV_CFG, save_cfg);

}

void ed64_ll_set_sram_bank (uint8_t bank) {

    ed64_ll_sram_bank = bank == 0 ? 0 : 1;

}

// FIXME Id like to use libdragon's equivelant for this
void _data_cache_invalidate_all (void) {
    asm(
        "li $8,0x80000000;"
        "li $9,0x80000000;"
        "addu $9,$9,0x1FF0;"
        "cacheloop:;"
        "cache 1,0($8);"
        "cache 1,16($8);"
        "cache 1,32($8);"
        "cache 1,48($8);"
        "cache 1,64($8);"
        "cache 1,80($8);"
        "cache 1,96($8);"
        "addu $8,$8,112;"
        "bne $8,$9,cacheloop;"
        "cache 1,0($8);"
    : // no outputs
    : // no inputs
    : "$8", "$9" // trashed registers
    );
}


// register related functions

// FIXME i dont actually think the alt64 names are do what they are saying they are doing
// a proper rename is in order?

void pi_initialize (void) {

	pi_dma_wait();
	io_write(PI_STATUS_REG, 0x03);

}

void pi_dma_wait (void) {  
	while (io_read(PI_STATUS_REG) & (PI_STATUS_IO_BUSY | PI_STATUS_DMA_BUSY));

}

// Inits PI for sram transfer
void pi_initialize_sram (void) {

	io_write(PI_BSD_DOM2_LAT_REG, 0x05);
	io_write(PI_BSD_DOM2_PWD_REG, 0x0C);
	io_write(PI_BSD_DOM2_PGS_REG, 0x0D);
	io_write(PI_BSD_DOM2_RLS_REG, 0x02);

}

void pi_dma_from_sram (void *dest, unsigned long offset, unsigned long size) {

	io_write(PI_DRAM_ADDR_REG, K1_TO_PHYS(dest));
	io_write(PI_CART_ADDR_REG, (PI_SAVE_ADDR + offset));
	 asm volatile ("" : : : "memory");
	io_write(PI_WR_LEN_REG, (size - 1));
	 asm volatile ("" : : : "memory");

}

void pi_dma_to_sram (void *src, unsigned long offset, unsigned long size) {

	pi_dma_wait();

	io_write(PI_STATUS_REG, 2);
	io_write(PI_DRAM_ADDR_REG, K1_TO_PHYS(src));
	io_write(PI_CART_ADDR_REG, (PI_SAVE_ADDR + offset));
    _data_cache_invalidate_all();
	io_write(PI_RD_LEN_REG, (size - 1));

}

void ed64_ll_set_sdcard_timing (void) {

    io_write(PI_BSD_DOM1_LAT_REG, 0x40);
    io_write(PI_BSD_DOM1_PWD_REG, 0x12);
    io_write(PI_BSD_DOM1_PGS_REG, 0x07);
    io_write(PI_BSD_DOM1_RLS_REG, 0x03);

    io_write(PI_BSD_DOM2_LAT_REG, 0x40);
    io_write(PI_BSD_DOM2_PWD_REG, 0x12);
    io_write(PI_BSD_DOM2_PGS_REG, 0x07);
    io_write(PI_BSD_DOM2_RLS_REG, 0x03);

}


void ed64_ll_get_sram (uint8_t *buffer, int size) {

    pi_initialize_sram();

    dma_wait();

    pi_initialize();

    sleep(250);
    
    // checks if the save isnt large and if so grabs it from the large save area
    if(size == KiB(32))
    {
        pi_dma_from_sram(buffer, 0, size) ;
    }
    else
    {
        pi_dma_from_sram(buffer, -KiB(64), size) ;
    }

    dma_wait();

    ed64_ll_set_sdcard_timing();

}

void ed64_ll_get_eeprom (uint8_t *buffer, int size) {

    int blocks=size/8;
    for( int b = 0; b < blocks; b++ ) {
        eeprom_read( b, &buffer[b * 8] );
    }

}


void ed64_ll_get_fram (uint8_t *buffer, int size) {

    ed64_ll_set_save_type(SAVE_TYPE_SRAM_128K); //2
    sleep(512);

    data_cache_hit_writeback_invalidate(buffer,size);
    dma_wait();
    
    ed64_ll_get_sram(buffer, size);

    sleep(512);
    ed64_ll_set_save_type(SAVE_TYPE_FLASHRAM);

}

/*
sram upload
*/


void ed64_ll_set_sram (uint8_t *buffer, int size) {

    pi_dma_wait();

    //Timing
    pi_initialize_sram();

    //Readmode
    pi_initialize();

    data_cache_hit_writeback_invalidate(buffer,size);
    dma_wait();

    // checks if you are no using a large save and if you are puts it in the large save area
    if(size == KiB(32))
    {
        pi_dma_to_sram(buffer, 0, size) ;
    }
    else
    {
        pi_dma_to_sram(buffer, -KiB(64), size) ;
    }

    //Wait
     pi_dma_wait();

    //Restore evd Timing
    ed64_ll_set_sdcard_timing();

}


void ed64_ll_set_eeprom (uint8_t *buffer, int size) {

    int blocks=size/8;
    for( int b = 0; b < blocks; b++ ) {
        eeprom_write( b, &buffer[b * 8] );
    }

}

void ed64_ll_set_fram (uint8_t *buffer, int size) {

    ed64_ll_set_save_type(SAVE_TYPE_SRAM_128K);
    sleep(512);

    data_cache_hit_writeback_invalidate(buffer,size);
    dma_wait();

    ed64_ll_set_sram(buffer, size);

    sleep(512);
    ed64_ll_set_save_type(SAVE_TYPE_FLASHRAM);

}
