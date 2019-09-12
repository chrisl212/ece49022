#include "stm32f0xx.h"
#include "ui/ui.h"
#include "sd.h"

#define CS (9)
#define SCK (13)
#define MISO (14)
#define MOSI (15)
#define CLR_CS (GPIOB->ODR &= ~(1 << CS))
#define SET_CS (GPIOB->ODR |= 1 << CS)
#define SECTOR (512)

static void _spi_init(void) {
    //PB09 = NSS, PB13 = SCK, PB14 = MISO, PB15 = MOSI
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
    GPIOB->MODER |= (0x1 << (2*CS)) + (0x2 << (2*SCK)) + (0x2 << (2*MISO)) + (0x2 << (2*MOSI));
    SET_CS;
    //set SPI2 to 375 kHz
    RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
    SPI2->CR1 |= SPI_CR1_BR_2 + SPI_CR1_BR_1 + SPI_CR1_MSTR + SPI_CR1_SSM + SPI_CR1_SSI;
    SPI2->CR2 |= SPI_CR2_FRXTH + SPI_CR2_SSOE;
    SPI2->CR1 |= SPI_CR1_SPE;
}

static uint8_t _spi_sendByte(uint8_t byte) {
    while (!(SPI2->SR & SPI_SR_TXE)); //wait for TX to finish
    *((uint8_t *)&SPI2->DR) = (uint8_t)byte; //data packing
    while (!(SPI2->SR & SPI_SR_RXNE)); //wait for TX to finish
    return *(uint8_t *)&(SPI2->DR);
}

static void _spi_sendWord(uint32_t word) {
    int i;

    for (i = 0; i < 4; i++) {
        //write MSB first
        _spi_sendByte((word >> ((3 - i) * 8)) & 0xFF);
    }
}

static void delay(int n) { while (n--); }

static uint8_t _spi_sendCommand(uint8_t idx, uint32_t arg) {
    uint8_t resp;
    
    _spi_sendByte(0xFF);
    CLR_CS;
    _spi_sendByte(0xFF);
    
    _spi_sendByte(idx | 0x40); //send index, prefixed with '01'
    _spi_sendWord(arg); //send argument
    if (idx == 0) {
        resp = _spi_sendByte(0x95);
    } else {
        _spi_sendByte(0x00); //send CRC (N/A)
    }
    
    _spi_sendByte(0xFF);
    SET_CS;
    _spi_sendByte(0xFF);
    return resp;
}

void _pulse(void) {
    int i;

    //send at least 74 (80) clock pulses following power on
    //NSS remains high
    for (i = 0; i < 10; i++) {
        _spi_sendByte(0xFF);
    }
}

int sd_init(void) {
    int res;

    _spi_init();
    while (1) {
        _pulse();
        res = _spi_sendCommand(0, 0);
        ui_writeFormat(1, "%d", res);
        delay(10000);
    }
    if (res != 0x01) {
        ui_writeFormat(1, "%d", res);
        return SD_INIT_FAIL;
    }

    return SD_OKAY;
}

int sd_readSector(uint32_t addr, uint8_t *buf) {
    int i;
    uint8_t byte;

    CLR_CS;
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

    SET_CS;

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

