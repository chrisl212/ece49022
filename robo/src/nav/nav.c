#include <stddef.h>
#include "stm32f0xx.h"
#include "ui/text/text.h"
#include "nav.h"
#include "lps/lps.h"
#include "state/state.h"
#include "ui/ui.h"
#include "math/math.h"
#include "drive/drive.h"
#include "sensors/sensors.h"

#define SAMPLES (10)

fatFile_t design;
uint16_t initialHead, desiredHead;
int16_t head, tmpHead;
int16_t desiredX, desiredY;
uint8_t avgCnt;

void TIM14_IRQHandler(void) {
    uint8_t p;

    tmpHead += lsm9ds0_getHeading() - initialHead;

/*    
    if (battery_low() && state_get() != STATE_ERR) {
        state_set(STATE_ERR);
        state_setErrorMessage("BATTERY LOW!!!");
        ui_draw();
        drive_stop();
        return;
    } else if (!battery_low() && state_get() == STATE_ERR) {
        state_restore();
        ui_draw();
        drive_start();
    } else if (state_get() == STATE_ERR) {
        return;
    } */

    if (++avgCnt == SAMPLES) {
        head = tmpHead/SAMPLES;
        avgCnt = 0;
        if (head < 0) {
            head += 360;
        }
        text_writeFormatAtPoint(f_12x16, 0, 20, LEFT, "(%d, %d)     ", head, desiredHead);
        tmpHead = 0;
    }
    nav_move(NULL, NULL, &p);
    drive_move();
    TIM14->SR &= ~(TIM_SR_UIF);
}

int nav_setup(fatFile_t file) {
    int s;

    design = file;
    lps_setup();
    if ((s = lsm9ds0_setup()) != LSM9DS0_OK) {
        state_set(STATE_ERR);
        state_setErrorMessage(lsm9ds0_error(s));
        ui_draw();
        return s;
    }

    RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
    TIM14->PSC = 4800 - 1;
    TIM14->ARR = 100 - 1;
    TIM14->DIER |= TIM_DIER_UIE;
    NVIC->ISER[0] |= 1 << TIM14_IRQn;
    TIM14->CR1 |= TIM_CR1_CEN;

    initialHead = lsm9ds0_getHeading();
    battery_setup();
    return 0;
}

static int _getNextCoords(uint16_t *x, uint16_t *y, uint8_t *p) {
    uint8_t buf[12];
    int status;

    status = fat_read(&design, buf, 12);
    *x = ((buf[1] << 8) | buf[0]);
    *y = 1000 - ((buf[5] << 8) | buf[4]);
    if (p) {
        *p = (buf[9] << 8) | buf[8];
    }
    return status;
}

static int _abs(int x) {
    return (x < 0) ? -x : x;
}

void nav_move(uint16_t *angle, uint8_t *speed, uint8_t *paint) {
    uint16_t currentX, currentY, currentHead, nextX, nextY;
    int16_t nextHead;
    int status;

    lps_getCoords(&currentX, &currentY, &currentHead);
    text_writeFormatAtPoint(f_12x16, 0, 140, LEFT, "(%d, %d)     ", currentX, currentY);
    if (_abs(desiredHead - head) < 10) {//_abs(currentX - desiredX) < 10 && _abs(currentY - desiredY) < 10) {
        status = _getNextCoords(&nextX, &nextY, paint);
        desiredX = nextX;
        desiredY = nextY;
    } else {
        nextX = desiredX;
        nextY = desiredY;
    }

    nextHead = atan2(nextY-currentY, nextX-currentX) * 180 / PI;
    if (nextHead < 0) {
        nextHead += 360;
    }

    if (angle) {
        *angle = nextHead;
    }
    if (speed) {
        *speed = (status != FAT_EOF);
    }

    if (status == FAT_EOF) {
        state_set(STATE_DONE);
        ui_draw();
    }

    desiredHead = nextHead;
    //debug
    //lps_setCoords(nextX, nextY, nextHead);
}

