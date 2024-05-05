#ifndef REBOOT_H__
#define REBOOT_H__


#ifndef __ASSEMBLER__

#include <stdint.h>
#include <stddef.h>


extern uint32_t reboot_start __attribute__((section(".text")));
extern size_t reboot_size __attribute__((section(".text")));

#endif


#endif
