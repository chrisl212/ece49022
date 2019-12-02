#include "stm32f0xx.h"
#include "servo.h"

static int on = 0;

void servo_init(void) { //tim3 ch4 corresponds to pb1, pc9
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB->MODER |= (0x2 << 2*8); //pb8 af
    GPIOB->AFR[1] |= (0x2 << 4*(8-8)); //tim16ch1
    RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;
    TIM16->ARR = 999; //count from 0 to 999
    TIM16->PSC = 959; //48MHz / 1000 / 960 = 50 Hz
    TIM16->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1; //pwm mode 1: logic high when cnt < ccr
    TIM16->CCER |= TIM_CCER_CC1E; //capture compare 4 oe; oc1 signal output on corresponding output pin
    TIM16->CCR1 = 80; //1.5 ms pulse; pulse duration = 20 * ccr μs
    TIM16->BDTR |= TIM_BDTR_MOE;

    TIM16->CR1 |= TIM_CR1_CEN;
}

void servo_off(void) { //do not set ccr < 30
    if (on) {
        TIM16->CCR1 = 115;
        on = 0;
    }
}

void servo_on(void) { //do not set ccr > 125
    if (!on) {
        TIM16->CCR1 = 80;
        on = 1;
    }
}

