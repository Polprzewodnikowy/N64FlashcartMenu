#ifndef MENU_H__
#define MENU_H__


#include "boot/boot.h"


typedef struct {
    boot_params_t boot_params;
} menu_t;


void menu_restore (menu_t *menu);
void menu_run (menu_t *menu);


#endif
