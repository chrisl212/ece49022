#include "nav.h"
#include "lps/lps.h"
#include "state/state.h"
#include "ui/ui.h"
#include "math/math.h"

fatFile_t design;

void nav_setup(fatFile_t file) {
    design = file;
    lps_setup();
}

static int _getNextCoords(uint16_t *x, uint16_t *y, uint8_t *p) {
    uint8_t buf[12];
    int status;

    status = fat_read(&design, buf, 12);
    *x = ((buf[1] << 8) | buf[0]);
    *y = 1000 - ((buf[5] << 8) | buf[4]);
    *p = (buf[9] << 8) | buf[8];
    return status;
}

void nav_move(uint16_t *angle, uint8_t *speed, uint8_t *paint) {
    uint16_t currentX, currentY, currentHead, nextX, nextY;
    int16_t nextHead;
    int status;

    lps_getCoords(&currentX, &currentY, &currentHead);
    status = _getNextCoords(&nextX, &nextY, paint);

    nextHead = atan2(nextY-currentY, nextX-currentX) * 180 / PI;
    if (nextHead < 0) {
        nextHead += 360;
    }

    *angle = nextHead;
    *speed = (status != FAT_EOF);

    if (status == FAT_EOF) {
        state_set(STATE_DONE);
        ui_draw();
    }

    //debug
    lps_setCoords(nextX, nextY, nextHead);
}

