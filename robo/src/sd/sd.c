#include "stm32f0xx.h"
#include "ui/ui.h"
#include "sd.h"

#define SECTOR (512)

static void _spi_init(void) {
    //PB12 = NSS, PB13 = SCK, PB14 = MISO, PB15 = MOSI
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB->MODER |= (0x1 << (2*12)) + (0x2 << (2*13)) + (0x2 << (2*14)) + (0x2 << (2*15));
    GPIOB->ODR |= 1 << 12;
    //set SPI2 to 375 kHz
    SPI2->CR1 |= SPI_CR1_BR_2 + SPI_CR1_BR_1 + SPI_CR1_MSTR;
    SPI2->CR1 |= SPI_CR1_SPE;
}

static void _spi_sendByte(uint8_t byte) {
    while (!(SPI2->SR & SPI_SR_TXE)); //wait for TX to finish
    *((uint8_t *)&SPI2->DR) = (uint8_t)byte; //data packing
}

static void _spi_sendWord(uint32_t word) {
    int i;

    for (i = 0; i < 4; i++) {
        //write MSB first
        _spi_sendByte((word >> ((3 - i) * 8)) & 0xFF);
    }
}

static uint8_t _spi_sendCommand(uint8_t idx, uint32_t arg) {
    uint8_t resp;

    GPIOB->ODR &= ~(1 << 12);
    _spi_sendByte(idx | 0x40); //send index, prefixed with '01'
    _spi_sendWord(arg); //send argument
    _spi_sendByte(0x00); //send CRC (N/A)
    while (!(SPI2->SR & SPI_SR_RXNE)); //wait for byte to be received
    resp = (uint8_t)SPI2->DR;
    GPIOB->ODR |= 1 << 12;
    return resp;
}

void _pulse(void) {
    int i;

    //send at least 74 (80) clock pulses following power on
    //NSS remains high
    for (i = 0; i < 10; i++) {
        _spi_sendByte(0x00);
    }
}

int sd_init(void) {
    _spi_init();
    _pulse();
    if (_spi_sendCommand(0, 0) != 0x01) {
        return SD_INIT_FAIL;
    }

    return SD_OKAY;
}

int sd_readSector(uint32_t addr, uint8_t *buf) {
    int i;
    uint8_t byte;

    GPIOB->ODR &= ~(1 << 12);
    _spi_sendByte(0x17 | 0x40); //send CMD17, prefixed with '01'
    _spi_sendWord(addr); //send addr
    _spi_sendByte(0x00); //send CRC (N/A)
    while (!(SPI2->SR & SPI_SR_RXNE)); //wait for byte to be received
    byte = (uint8_t)SPI2->DR;

    //3 extra bytes for token, CRC
    for (i = 0; i < SECTOR+3; i++) {
        while (!(SPI2->SR & SPI_SR_RXNE));
        byte = (uint8_t)SPI2->DR;
        if (i >= 1 && i <= SECTOR) {
            buf[i-1] = byte;
        }
    }

    GPIOB->ODR |= 1 << 12;

    return SD_OKAY;
}

void sd_error(sdStatus_t err) {
    switch (err) {
        case SD_OKAY:
            ui_writeLine(0, "SD: no error");
            break;
        case SD_INIT_FAIL:
            ui_writeLine(0, "SD: init fail");
            break;
    }
}

