#include "stm32f0xx.h"
#include "drive/drive.h"
#include "ui/text/text.h"
#include "collision.h"

void setupTIM17(void) {
    //TRIG signal
    RCC->APB2ENR |= RCC_APB2ENR_TIM17EN;
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB->MODER |= (0x2 << (2*9)); //enable AF for PA8
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
    GPIOC->MODER |= (0x2 << (2*6)) + (0x2 << (2*7)); //enable AF for PB3 (TIM3_CH2)
    TIM3->PSC = 1 - 1;
    TIM3->ARR = 0xFFFFFFFF;
    TIM3->CCMR1 |= TIM_CCMR1_CC2S_0 + TIM_CCMR1_CC1S_0;
    TIM3->CCER |= TIM_CCER_CC2E + TIM_CCER_CC2P + TIM_CCER_CC2NP + TIM_CCER_CC1E + TIM_CCER_CC1P + TIM_CCER_CC1NP;
    TIM3->DIER |= TIM_DIER_CC2IE;//+ TIM_DIER_CC1IE;
    NVIC->ISER[0] |= 1 << TIM3_IRQn;
    TIM3->CR1 |= TIM_CR1_CEN;
}

volatile uint32_t signalPolarity = 1;
volatile uint32_t lastCaptured;
int cnt = 0;
double avg = 0.0;

void TIM3_IRQHandler(void) {
    if ((TIM3->SR & TIM_SR_CC2IF) != 0 || (TIM3->SR & TIM_SR_CC1IF) != 0) { // Check interrupt flag is set
        uint32_t currentCaptured = TIM3->CCR2; // Clears interrupt flag.
        signalPolarity ^= 1; // Toggle polarity.
        if (signalPolarity == 0) {
            uint32_t width = currentCaptured - lastCaptured;
            if (width > 2784000) {
                signalPolarity ^= 1;
            } else {
                double currentDistance = (width / 48.0) / 58.0;
                avg += currentDistance;
                if (++cnt == 10) {
                    currentDistance = avg / cnt;
                    cnt = 0;
                    avg = 0.0;
                    if (currentDistance < 20.0) {
                        //stop
                        drive_collision(1);
                        text_writeFormatAtPoint(f_12x16, 0, 30, LEFT, "%d", (int)(currentDistance * 100));
                    } else {
                        //go
                        drive_collision(0);
                    }
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
