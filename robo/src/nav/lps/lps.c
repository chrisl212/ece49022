#include "stm32f0xx.h"
#include "ui/text/text.h"
#include "lps.h"

#define TX (14)
#define RX (15)
#define TEST (11)
	
static uint8_t buf[8];

void USART2_IRQHandler(void) {
	GPIOB->ODR |= 1 << TEST;
}

void lps_setup(void) {
    //PA15 AF1 = RX
    //DMA channel 5
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN + RCC_AHBENR_GPIOBEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

	GPIOB->MODER |= (0x1 << (2*TEST));
	GPIOB->ODR &= ~(1 << TEST);

    DMA1_Channel5->CCR |= DMA_CCR_MINC + DMA_CCR_CIRC;
    DMA1_Channel5->CNDTR = sizeof(buf)/sizeof(buf[0]);
    DMA1_Channel5->CPAR = (uint32_t)&(USART2->RDR);
    DMA1_Channel5->CMAR = (uint32_t)&(buf[0]);
    DMA1_Channel5->CCR |= DMA_CCR_EN;

    GPIOA->MODER |= (0x2 << (2*RX));// + (0x2 << (2*TX));
    GPIOA->AFR[1] |= (0x1 << (4*(RX-8)));// + (0x1 << (4*(TX-8)));

	NVIC->ISER[0] |= 1 << USART2_IRQn;
    USART2->BRR = 0x1388;
    USART2->CR3 |= USART_CR3_DMAR;
    USART2->CR1 |= USART_CR1_RE + USART_CR1_UE + USART_CR1_RXNEIE;
}

void lps_getCoords(int16_t *x, int16_t *y, uint16_t *head) {
    int i = 0;
	int len;

    len = sizeof(buf)/sizeof(buf[0]);
	if (buf[i] == 0x7F && buf[i+1] == 0xFF) {
		*x = *y = *head = 0;
		*x |= buf[i+2] << 8;
		*x |= buf[i+3];
		*y |= buf[i+4] << 8;
		*y |= buf[i+5];
		*head |= buf[i+6] << 8;
		*head |= buf[i+7];
	}
}

