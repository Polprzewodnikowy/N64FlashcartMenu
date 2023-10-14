#ifndef ASSETS_H__
#define ASSETS_H__


#include <stdint.h>


#define ASSET(n) \
    extern uint8_t assets_##n[] __attribute__((section(".data"))); \
    extern int assets_##n##_size[] __attribute__((section(".data")))


ASSET(FiraMonoBold);


#endif
