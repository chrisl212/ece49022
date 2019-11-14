#ifndef NAV_H
#define NAV_H

#include <stdint.h>
#include "fat/fat.h"

int nav_setup(fatFile_t file);
void nav_move(uint16_t *angle, uint8_t *speed, uint8_t *paint);

#endif

