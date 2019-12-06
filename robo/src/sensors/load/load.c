#include <stdint.h>
#include "stm32f0xx.h"
#include "load.h"

extern void nano_wait(uint32_t n);

void load_init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; //enable adc1
    GPIOB->MODER |= GPIO_MODER_MODER0; //set b0 to analog
    ADC1->CHSELR |= ADC_CHSELR_CHSEL8;
    ADC1->CFGR1 &= ~ADC_CFGR1_RES; //12 bit resolution
}

int load_empty(void) {
    int i, data = 0;

    ADC1->CR |= ADC_CR_ADEN;
    for(i = 0; i < 10; i++) {
        nano_wait(10000);
        while(!(ADC1->ISR & ADC_ISR_ADRDY)); //wait for ready
        ADC1->CR |= ADC_CR_ADSTART; //take adc measurement
        while(!(ADC1->ISR & ADC_ISR_EOC));
        data += ADC1->DR;
    }
    return ((data / 10) < 1000);
}
