#ifndef CHEATS_H__
#define CHEATS_H__


#define CHEAT_PAYLOAD_ADDRESS   (0x80700000)


#ifndef __ASSEMBLER__

#include <stdint.h>
#include <stddef.h>

extern uint32_t cheat_payload __attribute__((section(".text")));
extern size_t cheat_payload_size __attribute__((section(".text")));

#endif


#endif
