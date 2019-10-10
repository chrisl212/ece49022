#include "stm32f0xx.h"
#include "stack.h"

#define GRN (0)
#define YEL (1)
#define RED (2)
#define BUZ (3)

void stack_init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIOC->MODER |= (0x1 << (2*GRN)) + (0x1 << (2*YEL)) + (0x1 << (2*RED)) + (0x1 << (2*BUZ));
    stack_off(STACK_GRN);
    stack_off(STACK_RED);
    stack_off(STACK_YEL);
    buzzer_off();
}

void stack_off(stackLight_t light) {
    switch (light) {
        case STACK_GRN:
            GPIOC->ODR &= ~(1 << GRN);
            break;
        case STACK_YEL:
            GPIOC->ODR &= ~(1 << YEL);
            break;
        case STACK_RED:
            GPIOC->ODR &= ~(1 << RED);
            break;
    }
}

void stack_on(stackLight_t light) {
    switch (light) {
        case STACK_GRN:
            GPIOC->ODR |= 1 << GRN;
            break;
        case STACK_YEL:
            GPIOC->ODR |= 1 << YEL;
            break;
        case STACK_RED:
            GPIOC->ODR |= 1 << RED;
            break;
    }
}

void buzzer_on(void) {
    GPIOC->ODR |= 1 << BUZ;
}

void buzzer_off(void) {
    GPIOC->ODR &= ~(1 << BUZ);
}

