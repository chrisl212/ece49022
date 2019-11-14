#include "stm32f0xx.h"
#include "ui/text/text.h"
#include "state/state.h"
#include "battery.h"

#define AVG (10)

void battery_setup(void) {
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
    RCC->CR2 |= RCC_CR2_HSI14ON;

    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB->MODER |= (0x3 << (2*0));

    ADC1->CR |= ADC_CR_ADEN;
}

static uint32_t cnt;
static uint32_t sum;
static int prev;

int battery_low(void) {
    ADC1->CHSELR |= 0x1 << 8; //PA0
    while (!(ADC1->ISR & ADC_ISR_ADRDY));
    ADC1->CR |= ADC_CR_ADSTART;
    while (!(ADC1->ISR & ADC_ISR_EOC));
    sum += ADC1->DR;
    if (++cnt == AVG) {
        sum /= cnt;
        if (sum < 3000) {
            sum = 0;
            cnt = 0;
            prev = 1;
            return 1;
        } else {
            sum = 0;
            cnt = 0;
            prev = 0;
            return 0;
        }
    }
    return prev;
}
