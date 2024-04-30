#ifndef CHEATS_H__
#define CHEATS_H__

#define RELOCATED_EXCEPTION_HANDLER_ADDRESS (0x80000120)

#ifndef __ASSEMBLER__

#include <stddef.h>
#include <stdint.h>

#include "boot.h"
#include "cic.h"

extern uint32_t cheat_payload __attribute__((section(".text")));
extern size_t cheat_payload_size __attribute__((section(".text")));

bool cheats_install (cic_type_t cic_type, uint32_t *cheat_list);

#endif

#endif
