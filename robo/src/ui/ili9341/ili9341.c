#include <stdint.h>
#include "stm32f0xx.h"
#include "ili9341.h"

#define DC (2)
#define RST (3)
#define CS (4)
#define SCK (5)
#define MOSI (7)

#define CLR_RS (GPIOA->ODR &= ~(1 << DC))
#define SET_RS (GPIOA->ODR |= 1 << DC)
#define CLR_CS (GPIOA->ODR &= ~(1 << CS), SPI1->CR1 &= ~(SPI_CR1_SSI))
#define SET_CS (GPIOA->ODR |= (1 << CS), SPI1->CR1 |= SPI_CR1_SSI)

#define MS 1000000
#define US 1000

int lastX0 = -1;
int lastX1 = -1;
int lastY0 = -1;
int lastY1 = -1;

extern void nano_wait(uint32_t n);

static inline void delay(uint32_t ms) {
    nano_wait(ms * MS);
}

static void _reset(void) {
    GPIOA->ODR &= ~(1 << RST);
    delay(10);
    GPIOA->ODR |= 1 << RST;
    delay(150);
}

static void _setupSPI(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    GPIOA->MODER |= (0x1 << (2*RST)) + (0x2 << (2*MOSI)) + (0x2 << (2*SCK)) +
                    (0x1 << (2*CS)) + (0x1 << (2*DC));
    SET_CS;

    SPI1->CR1 |= SPI_CR1_BIDIMODE + SPI_CR1_BIDIOE + SPI_CR1_MSTR + SPI_CR1_SSM + SPI_CR1_SSI;
    SPI1->CR2 |= SPI_CR2_SSOE;
    SPI1->CR1 |= SPI_CR1_SPE;
}

static void _sendCommand(uint8_t command) {
    CLR_RS;
    CLR_CS;
    while ((SPI1->SR & SPI_SR_BSY));
    *((uint8_t*)&SPI1->DR) = (uint8_t) command;
    while ((SPI1->SR & SPI_SR_BSY));
    SET_CS;
}

static void _sendData(uint8_t data) {
    SET_RS;
    CLR_CS;
    while ((SPI1->SR & SPI_SR_BSY));
    *((uint8_t*)&SPI1->DR) = (uint8_t) data;
    while ((SPI1->SR & SPI_SR_BSY));
    SET_CS;
}

void ili9341_setDisplayWindow(int x0, int y0, int x1, int y1) {
    if (x0 == lastX0+1 && x1 == lastX1+1 && y0 == lastY0 && y1 == lastY1) {
        return;
    }
    lastX0 = x0;
    lastX1 = x1;
    lastY0 = y0;
    lastY1 = y1;

    _sendCommand(ILI9341_CASET);   // Column Address Set
    _sendData(x0>>8); // X address start:
    _sendData(x0);        // 0 <= XS <= X
    _sendData(x1>>8); // X address end:
    _sendData(x1);        // S <= XE <= X

    _sendCommand(ILI9341_RASET); //Row address set
    _sendData(y0>>8); // Y address start:
    _sendData(y0);        // 0 <= YS <= Y
    _sendData(y1>>8); // Y address start:
    _sendData(y1);        // S <= YE <= Y

    _sendCommand(ILI9341_RAMWR); //write data
}

void ili9341_exitSleep(void) {
    _sendCommand(ILI9341_SLPOUT); // Exit Sleep Mode
    delay(120);
    _sendCommand(ILI9341_DISPON); // Display on
}

void ili9341_enterSleep(void) {
    _sendCommand(ILI9341_SLPIN);
    delay(120);
    _sendCommand(ILI9341_DLPOFFSAVE); // Display on
}

void ili9341_init(void) {
    _setupSPI();
    _reset();

    //-----------------------------Display setting--------------------------------
    _sendCommand(ILI9341_MADCTL); //Page 215
    _sendData(0xE0); //DEFAULT
//    _sendData(0x20);

    //Address control
    _sendCommand(ILI9341_COLMOD); //Interface pixel format Pg 224
    _sendData(0x55);

    _sendCommand(ILI9341_INVOFF);

    ili9341_setDisplayWindow(0, 0, WIDTH, HEIGHT);
    ili9341_exitSleep();
}

void ili9341_fillLCD(uint16_t color) {
    register int h, w;

    ili9341_setDisplayWindow(0, 0, WIDTH, HEIGHT);

    for (h = 0; h < HEIGHT; h++) {
        for (w = 0; w < WIDTH; w++) {
            ili9341_writePixel(color);
        }
    }
}

void ili9341_writePixel(uint16_t color) {
    _sendData(color >> 8);
    _sendData(color & 0xFF);
}

void ili9341_writePixelAtPoint(uint16_t x, uint16_t y, uint16_t color) {
    ili9341_setDisplayWindow(x, y, WIDTH, HEIGHT);
    ili9341_writePixel(color);
}

void ili9341_setBrightness(uint16_t bright) {
    _sendCommand(ILI9341_WRDISBV);
    _sendData(bright);
}
