#include <stdint.h>
#include <string.h>


typedef struct {
    char *name;
    uint8_t *data;
    int size;
} asset_t;


#define ASSET_IMPORT(a) \
    extern uint8_t *_binary_assets_##a##_start __attribute__((section(".data"))); \
    extern int _binary_assets_##a##_size __attribute__((section(".data")));
#define ASSET(n, a) {  n, (uint8_t *) (&_binary_assets_##a##_start), (int) (&_binary_assets_##a##_size) }


ASSET_IMPORT(FiraMono_Bold_font64);

static asset_t assets[] = {
    ASSET("assets:/font", FiraMono_Bold_font64),
};


extern void *__real_asset_load (char *fn, int *sz);

void *__wrap_asset_load (char *fn, int *sz) {
    for (int i = 0; i < sizeof(assets) / sizeof(assets[0]); i++) {
        asset_t *asset = &assets[i];
        if (strcmp(asset->name, fn) == 0) {
            *sz = asset->size;
            return asset->data;
        }
    }

    return __real_asset_load(fn, sz);
}
