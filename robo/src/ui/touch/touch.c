#include <stdint.h>
#include "stm32f0xx.h"
#include "touch.h"
#include "ui/text/text.h"
#include "ui/ili9341/ili9341.h"
#include "ui/ui.h"

#define IRQ (1)
#define CS (6)
#define CLR_CS (GPIOA->ODR &= ~(1 << CS))
#define SET_CS (GPIOA->ODR |= (1 << CS))

#define XMIN (360)
#define XMAX (3900)
#define YMIN (230)
#define YMAX (3840)

#define X(x) ((x-XMIN)*WIDTH/(XMAX-XMIN))
#define Y(y) ((y-YMIN)*HEIGHT/(YMAX-YMIN))

static uint8_t _spi_writeByte(uint8_t byte) {
    while (!(SPI2->SR & SPI_SR_TXE));
    *((uint8_t *)&SPI2->DR) = (uint8_t)byte; //data packing
    while (!(SPI2->SR & SPI_SR_RXNE)); //wait for TX to finish
    return *(uint8_t *)&(SPI2->DR);
}

static uint16_t _spi_writeWord(uint8_t word) {
    uint16_t res = 0;
    _spi_writeByte(word);
    res |= _spi_writeByte(0x00) << 8;
    res |= _spi_writeByte(0x00);
    return res;
}

static void _readCoord() {
    int16_t z, z1, z2, data[6] = {0};
    while (SPI2->SR & SPI_SR_BSY);

    CLR_CS;

    _spi_writeWord(0xB1);
    z1 = _spi_writeWord(0xC1) >> 3;
    z = z1 + 4095;
    z2 = _spi_writeWord(0x91) >> 3;
    z -= z2;

    _spi_writeWord(0x91);
    data[0] = _spi_writeWord(0xD1) >> 3;
    data[1] = _spi_writeWord(0x91) >> 3;
    data[2] = _spi_writeWord(0xD1) >> 3;
    data[3] = _spi_writeWord(0x91) >> 3;
    data[4] = _spi_writeWord(0xD0) >> 3;
    data[5] = _spi_writeWord(0x00) >> 3;

    SET_CS;

    text_writeFormatAtPoint(f_12x16, 0, 150, LEFT, "(x, y) = (%d, %d)", X(data[3]), Y(data[2]));
    ui_touchEvent(X(data[3]), Y(data[2]));
}

void TIM2_IRQHandler(void) {
    if (!(GPIOA->IDR & (1 << IRQ))) {
        _readCoord();
    }
    TIM2->SR &= ~(TIM_SR_UIF);
}

void touch_init(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    GPIOA->MODER |= (0x1 << (2*CS));
    SET_CS;
    //rest of io/spi2 set up in SD

    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    TIM2->PSC = 48000 - 1;
    TIM2->ARR = 100 - 1;
    TIM2->DIER |= TIM_DIER_UIE;
    NVIC->ISER[0] |= 1 << TIM2_IRQn;
    TIM2->CR1 |= TIM_CR1_CEN;
    _readCoord();
}
