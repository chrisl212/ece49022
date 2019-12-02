#ifndef LPS_H
#define LPS_H

#include <stdint.h>

void lps_setup(void);
void lps_getCoords(int8_t *x, int8_t *y, uint16_t *head);
//debug only
void lps_setCoords(uint16_t x, uint16_t y, uint16_t head);

#endif

