#include "stm32f0xx.h"
#include "ui/text/text.h"
#include "lps.h"

#define TX (14)
#define RX (15)

int8_t currentX = 0;
int8_t currentY = 0;
int16_t currentHead = 0;
int8_t nextX, nextY;
uint16_t nextHead;
int byte = 0;
int8_t buf[4];

void lps_setup(void) {
    //PA15 AF1 = RX
    //DMA channel 5
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    DMA1_Channel5->CCR |= DMA_CCR_MINC + DMA_CCR_CIRC;
    DMA1_Channel5->CNDTR = sizeof(buf)/sizeof(buf[0]);
    DMA1_Channel5->CPAR = (uint32_t)&(USART2->RDR);
    DMA1_Channel5->CMAR = (uint32_t)&(buf[0]);
    DMA1_Channel5->CCR |= DMA_CCR_EN;

    GPIOA->MODER |= (0x2 << (2*RX));// + (0x2 << (2*TX));
    GPIOA->AFR[1] |= (0x1 << (4*(RX-8)));// + (0x1 << (4*(TX-8)));

    USART2->BRR = 0x1388;
    USART2->CR3 |= USART_CR3_DMAR;
    USART2->CR1 |= USART_CR1_RE + USART_CR1_UE;
}

void lps_getCoords(int8_t *x, int8_t *y, uint16_t *head) {
    int i, len;

    len = sizeof(buf)/sizeof(buf[0]);
    for (i = 0; i < len; i++) {
        if (buf[i] == -2 && i+1 < len && x) {
            *x = buf[i+1];
        }
        if (buf[i] == -1 && i+1 < len && y) {
            *y = buf[i+1];
        }
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

