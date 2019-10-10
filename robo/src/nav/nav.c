#include "stm32f0xx.h"
#include "ui/text/text.h"
#include "nav.h"
#include "lps/lps.h"
#include "state/state.h"
#include "ui/ui.h"
#include "math/math.h"
#include "lsm9ds0/lsm9ds0.h"

fatFile_t design;
uint16_t initialHead;

void TIM14_IRQHandler(void) {
    int16_t head = lsm9ds0_getHeading() - initialHead;
    if (head < 0) {
        head += 360;
    }

    text_writeFormatAtPoint(f_12x16, 0, 20, LEFT, "Heading: %d  ", head);

    TIM14->SR &= ~(TIM_SR_UIF);
}

void nav_setup(fatFile_t file) {
    design = file;
//    lps_setup();
    lsm9ds0_setup();

    RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
    TIM14->PSC = 4800 - 1;
    TIM14->ARR = 100 - 1;
    TIM14->DIER |= TIM_DIER_UIE;
    NVIC->ISER[0] |= 1 << TIM14_IRQn;
    TIM14->CR1 |= TIM_CR1_CEN;

    initialHead = lsm9ds0_getHeading();
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

