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
#define OFFSET (15)

fatFile_t design;
uint16_t initialHead, desiredHead;
int16_t head, tmpHead;
int8_t desiredX = OFFSET;
int8_t desiredY = OFFSET;
uint8_t avgCnt;

void TIM14_IRQHandler(void) {
    uint8_t p;

    tmpHead += lsm9ds0_getHeading() - initialHead;
/*
    if (load_empty()) {
        state_setErrorMessage("PAINT LOW!!!");
        if (state_get() != STATE_ERR) {
            state_set(STATE_ERR);
            ui_draw();
            drive_stop();
        }
        return;
    } else if (state_get() == STATE_ERR) {
        state_restore();
        ui_draw();
        drive_start();
    }*/
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
        tmpHead = 0;
    }
    if (state_get() != STATE_ERR) {
        nav_move(NULL, NULL, &p);
        servo_on();
        drive_move();
    }
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

    NVIC_SetPriority(USART2_IRQn, 0);
    NVIC_SetPriority(TIM14_IRQn, 4);
    RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
    TIM14->PSC = 4800 - 1;
    TIM14->ARR = 10 - 1;
    TIM14->DIER |= TIM_DIER_UIE;
    NVIC->ISER[0] |= 1 << TIM14_IRQn;
    TIM14->CR1 |= TIM_CR1_CEN;

    initialHead = lsm9ds0_getHeading();
    battery_setup();
    load_init();
    servo_init();
    return 0;
}

static int _getNextCoords(int8_t *x, int8_t *y, uint8_t *p) {
    uint8_t buf[12];
    uint16_t tmpX, tmpY;
    int status;

    status = fat_read(&design, buf, 12);
    tmpX = ((buf[1] << 8) | buf[0]);
    tmpY = 1000 - ((buf[5] << 8) | buf[4]);
    if (p) {
        *p = (buf[9] << 8) | buf[8];
    }
    tmpX /= 8;
    tmpY /= 8;
    if (tmpX > 255) {
        tmpX = 255;
    }
    if (tmpY > 255) {
        tmpY = 255;
    }
    *x = tmpX;
    *y = tmpY;
    return status;
}

static int _abs(int x) {
    return (x < 0) ? -x : x;
}

void nav_move(uint16_t *angle, uint8_t *speed, uint8_t *paint) {
    int8_t currentX, currentY, nextX, nextY;
    uint16_t currentHead;
    int16_t nextHead;
    int status;

    lps_getCoords(&currentX, &currentY, &currentHead);
    if (_abs(currentX - desiredX) < 10 && _abs(currentY - desiredY) < 10) {
        status = _getNextCoords(&nextX, &nextY, paint);
        if (desiredX < 127-OFFSET) {
            desiredX += OFFSET;
        }
        if (desiredY < 127-OFFSET) {
            desiredY += OFFSET;
        }
        nextX = desiredX;
        nextY = desiredY;
    } else {
        nextX = desiredX;
        nextY = desiredY;
    }

    nextHead = atan2(nextY-currentY, nextX-currentX) * 180 / PI;
    if (nextHead < 0) {
        nextHead += 360;
    }
    nextHead = 360 - nextHead;

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

    if (state_get() != STATE_ERR) {
        text_writeFormatAtPoint(f_12x16, 0, 20, LEFT, "(%d, %d) => (%d, %d)          ", currentX, currentY, desiredX, desiredY);
        text_writeFormatAtPoint(f_12x16, 0, 50, LEFT, "%d deg => %d deg    ", head, nextHead);
    }

    desiredHead = nextHead;
    //debug
    //lps_setCoords(nextX, nextY, nextHead);
}

