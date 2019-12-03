#ifndef LPS_H
#define LPS_H

#include <stdint.h>

void lps_setup(void);
void lps_getCoords(int16_t *x, int16_t *y, uint16_t *head);

#endif

