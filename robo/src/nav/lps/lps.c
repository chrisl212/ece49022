#include "lps.h"

uint16_t currentX = 0;
uint16_t currentY = 0;
uint16_t currentHead = 0;

void lps_setup(void) {
    //setup i2c
}

void lps_getCoords(uint16_t *x, uint16_t *y, uint16_t *head) {
    if (x) {
        *x = currentX;
    }
    if (y) {
        *y = currentY;
    }
    if (head) {
        *head = currentHead;
    }
}

//debug only
void lps_setCoords(uint16_t x, uint16_t y, uint16_t head) {
    currentX = x;
    currentY = y;
    currentHead = head;
}

