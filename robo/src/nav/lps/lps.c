#include "stm32f0xx.h"
#include "ui/text/text.h"
#include "lps.h"

#define RX (15)

uint16_t currentX = 0;
uint16_t currentY = 0;
uint16_t currentHead = 0;
uint16_t nextX, nextY, nextHead;
int byte;

void USART2_IRQHandler(void) {
    uint16_t in = USART2->RDR;
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
/*        case 4:
            nextHead |= (in << 8);
            break;
        case 5:
            nextHead |= in;
            break;*/
    }

    if (++byte == 4) {
        currentX = nextX;
        currentY = nextY;
        text_writeFormatAtPoint(f_12x16, 0, 60, LEFT, "(%d, %d, %d)    ", nextX, nextY, in);
//        currentHead = nextHead;
        byte = nextX = nextY = nextHead = 0;
    }
}

void lps_setup(void) {
    //PA15 AF1 = RX
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    GPIOA->MODER |= (0x2 << (2*RX));
    GPIOA->AFR[1] |= (0x1 << (4*(RX-8)));

    USART2->CR1 |= USART_CR1_RXNEIE;
    NVIC->ISER[0] |= 1 << USART2_IRQn;
    USART2->BRR = 0x1388;

    USART2->CR1 |= USART_CR1_RE + USART_CR1_UE;
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

