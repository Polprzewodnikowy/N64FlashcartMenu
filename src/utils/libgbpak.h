/*
 * libgbpak header
 *
 */

#define GB_NORM		0x00
#define GB_MBC1		0x01
#define GB_MBC2		0x02
#define GB_MMMO1	0x03
#define GB_MBC3		0x04
#define GB_MBC4		0x10 //test
#define GB_MBC5		0x05
#define GB_CAMERA	0x06
#define GB_TAMA5	0x07
#define GB_HUC3		0x08
#define GB_HUC1		0x09

#define FALSE 0
#define TRUE 1
#define BANKSIZE 16*1024

typedef struct {
          char mapper;
          char ram;
          char battery;
          char rtc;
          char rumble;
          char sgb;
          char gbc;
          char title[12];
          char _romsize;
          char _ramsize;
          int romsize;
          int ramsize;
          short rombanks;
          short rambanks;
          short bank;
          short cpld;
} cart;



//externals
int init_gbpak(void);
int copy_gbRom_toRAM(uint8_t *rom_data, uint32_t *bankOffset, uint32_t size, char* _string);
int copy_gbRam_toRAM(uint8_t *ram_data, char* _string, bool isDump);
int copy_save_toGbRam(uint8_t *ram_data);
int disable_gbRam(void);


//internals
int _get_gbPower(void);
int _set_gbPower(int);
int _get_gbAccessState(void);
int _set_gbAccessState(int);
int _set_gbRomBank(int bank);
int _set_gbRamBank(int bank);
int _get_gbRomAddr(unsigned long addr, uint8_t *rdata);
int _get_gbRamAddr(unsigned long addr, uint8_t *rdata);
int _set_gbRamAddr(unsigned long addr, uint8_t *sdata);



size_t get_size_of_cart_rom(void);
