#include <string.h>
#include "stm32f0xx.h"
#include "ui.h"

char lines[2][16] = {0};

static void _setupSPI(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
    GPIOA->MODER |= (0x2 << (2*4)) + (0x2 << (2*5)) + (0x2 << (2*7));
    SPI1->CR1 |= SPI_CR1_MSTR + SPI_CR1_BR;
    SPI1->CR1 |=  SPI_CR1_BIDIMODE + SPI_CR1_BIDIOE;
    SPI1->CR2 = SPI_CR2_SSOE | SPI_CR2_NSSP | SPI_CR2_DS_3 | SPI_CR2_DS_0;
    SPI1->CR1 |= SPI_CR1_SPE;
}

static void _SPIWrite(unsigned int data) {
    while ((SPI1->SR & SPI_SR_TXE) != 2);
    SPI1->DR = data;
}

static int _strlen(char *s) {
    int i = 0;
    
    while (*s++) i++;
    return i;
}

void ui_writeLine(int line, char *str) {
    _SPIWrite(0x0080 + 0x40*line);
    int l = _strlen(str);
    for (int i = 0; i < 32; i++) {
        if (i < l) _SPIWrite(0x0200 | str[i]);
        else _SPIWrite(0x0200 | ' ');
    }
}

void ui_setup(void) {
    _setupSPI();
    _SPIWrite(0x0038); //set function
    _SPIWrite(0x000C); //turn on display
    _SPIWrite(0x0001); //clear display
    _SPIWrite(0x0002); //return home
    _SPIWrite(0x0006); //set entry mode
    ui_writeLine(0, "");
}
