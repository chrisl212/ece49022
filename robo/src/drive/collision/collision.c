#include "stm32f0xx.h"
#include "drive/drive.h"
#include "ui/text/text.h"
#include "collision.h"

void setupTIM17(void) {
    //TRIG signal
    RCC->APB2ENR |= RCC_APB2ENR_TIM17EN;
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB->MODER |= (0x2 << (2*9)); //enable AF for PB9
    GPIOB->AFR[1] |= (0x2 << (4*(9-8)));
    TIM17->PSC = 150 - 1;
    TIM17->ARR = 20000 - 1;
    TIM17->CCMR1 |= 0x0060;
    TIM17->CCER |= TIM_CCER_CC1E;
    TIM17->BDTR |= TIM_BDTR_MOE;
    TIM17->CCR1 = 3;
    TIM17->CR1 |= TIM_CR1_CEN;
}

void setupTIM3(void) {
    //ECHO capture
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIOC->MODER |= /*(0x2 << (2*6))*/ + (0x2 << (2*7)); //enable AF for PC6 and PC7 (TIM3)
    TIM3->PSC = 8 - 1;
    TIM3->ARR = 0xFFFFFFFF;
    TIM3->CCMR1 |= TIM_CCMR1_CC2S_0;// + TIM_CCMR1_CC1S_0;
    TIM3->CCER |= TIM_CCER_CC2E + TIM_CCER_CC2P + TIM_CCER_CC2NP;// + TIM_CCER_CC1E + TIM_CCER_CC1P + TIM_CCER_CC1NP;
    TIM3->DIER |= TIM_DIER_CC2IE;//+ TIM_DIER_CC1IE;
    NVIC->ISER[0] |= 1 << TIM3_IRQn;
    TIM3->CR1 |= TIM_CR1_CEN;
}

volatile uint32_t signalPolarity = 0;
volatile uint32_t lastCaptured, sum, cnt;

void TIM3_IRQHandler(void) {
    uint32_t currentCaptured, width, currentDistance;

    if (TIM3->SR & TIM_SR_CC2IF) { // Check interrupt flag is set
        currentCaptured = TIM3->CCR2; // Clears interrupt flag.
        signalPolarity ^= 1; // Toggle polarity.
        if (signalPolarity == 0) {
            width = currentCaptured - lastCaptured;
            if (width > 40000 || currentCaptured < lastCaptured) {
                signalPolarity ^= 1;
            } else {
                currentDistance = 100 * width / (48*58*8);
                sum += currentDistance;
                if (++cnt == 10) {
                    drive_collision((sum/cnt) < 20);
                    sum = cnt = 0;
                }
            }
        }
        lastCaptured = currentCaptured;
    }

    if ((TIM3->SR & TIM_SR_UIF) != 0) { // Check for overflow.
        // Clear the UIF to prevent endless IRQ.
        TIM3->SR &= ~TIM_SR_UIF;
    }
}

void collision_setup(void) {
    setupTIM17();
    setupTIM3();
}
