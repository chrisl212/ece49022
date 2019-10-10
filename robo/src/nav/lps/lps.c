#include "stm32f0xx.h"
#include "lps.h"

#define RX (7)

uint16_t currentX = 0;
uint16_t currentY = 0;
uint16_t currentHead = 0;
uint16_t nextX, nextY, nextHead;
int byte;

void USART1_IRQHandler(void) {
    uint16_t in = USART1->RDR & 0xFF;
    switch (byte) {
        case 0:
            nextX |= (in << 8);
            break;
        case 1:
            nextX |= in;
            break;
        case 2:
            nextY |= (in << 8);
            break;
        case 3:
            nextY |= in;
            break;
        case 4:
            nextHead |= (in << 8);
            break;
        case 5:
            nextHead |= in;
            break;
    }

    if (++byte > 5) {
        currentX = nextX;
        currentY = nextY;
        currentHead = nextHead;
        byte = nextX = nextY = nextHead = 0;
    }
}

void lps_setup(void) {
    //PB7 AF0 = RX
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    GPIOB->MODER |= (0x2 << (2*RX));

    USART1->CR1 |= USART_CR1_RXNEIE;
    NVIC->ISER[0] |= 1 << USART1_IRQn;
    USART1->BRR = 0x1388;

    USART1->CR1 |= USART_CR1_RE + USART_CR1_UE;
}

void lps_getCoords(uint16_t *x, uint16_t *y, uint16_t *head) {
    if (x) {
        *x = currentX;
    }
    if (y) {
        *y = currentY;
    }
    if (head) {
        *head = currentHead;
    }
}

//debug only
void lps_setCoords(uint16_t x, uint16_t y, uint16_t head) {
    currentX = x;
    currentY = y;
    currentHead = head;
}

