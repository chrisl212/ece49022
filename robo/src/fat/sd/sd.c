#include <stddef.h>
#include "stm32f0xx.h"
#include "ui/ui.h"
#include "sd.h"

#define TIMEOUT (100)
#define CS (12)
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

static uint32_t _spi_readWord(void) {
    uint32_t resp = 0;
    int i;

    for (i = 0; i < 4; i++) {
        resp |= _spi_sendByte(0xFF) << (8*(3-i));
    }
    return resp;
}

static void delay(int n) { while (n--); }

static uint8_t _spi_sendCommand(uint8_t idx, uint32_t arg, uint8_t a, uint32_t *ocr) {
    uint8_t crc = 0x00;
    uint8_t resp = 0xFF;

    if (a) {
        _spi_sendCommand(55, 0, 0, ocr);
    }

    _spi_sendByte(0xFF);
    CLR_CS;
    _spi_sendByte(0xFF);
    
    _spi_sendByte(idx | 0x40); //send index, prefixed with '01'
    _spi_sendWord(arg); //send argument
    switch (idx) {
        case 0:
            crc = 0x95;
            break;
        case 1:
            crc = 0xF9;
            break;
        case 8:
            crc = 0x87;
            break;
    }
    _spi_sendByte(crc);
   
    while (resp == 0xFF && (idx != 55)) {
        resp = _spi_sendByte(0xFF); //read until get a proper response
    }
    if (idx == 8 || idx == 58) {
        *ocr = _spi_readWord();
    }
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

int sd_init(sdCard_t *card) {
    uint8_t res;
    uint8_t timeout = 0;
    uint32_t ocr;

    card->mode = SD_BYTE;

    _spi_init();
    _pulse();

    res = _spi_sendCommand(0, 0, 0, NULL);
    if (res != 0x01) {
        return SD_UNRECOGNIZED;
    }
    
    res = _spi_sendCommand(8, 0x01AA, 0, &ocr);
    if (res != 0x01 || ocr != 0x01AA) {
        return SD_UNRECOGNIZED;
    }
    res = _spi_sendCommand(41, 0x40000000, 1, NULL);
    if (res != 0x00) {
        if (res == 0x01) {
            do {
                res = _spi_sendCommand(41, 0x40000000, 1, NULL);
                timeout++;
                delay(100000);
            } while (res != 0x00 && timeout < TIMEOUT);
            if (timeout == TIMEOUT) {
                return SD_TIMEOUT;
            }
        } else if (res == 0x05) {
            do {
                res = _spi_sendCommand(1, 0, 0, NULL);
                timeout++;
                delay(100000);
            } while (res != 0x00 && timeout < TIMEOUT);
            if (timeout == TIMEOUT) {
                return SD_TIMEOUT;
            }
        } else {
            return SD_UNKNOWN_VERSION;
        } 
    }
    res = _spi_sendCommand(58, 0, 0, &ocr);
    if (ocr & 0x40000000) {
        card->mode = SD_BLOCK;
    }

    res = _spi_sendCommand(16, 0x200, 0, NULL);
    if (res != 0x00) {
        return 1;
    }

    return SD_OKAY;
}

int sd_readSector(sdCard_t card, uint32_t addr, uint8_t *buf) {
    int i;
    uint8_t byte, resp;

    if (card.mode == SD_BYTE) {
        addr *= 512;
    }

    _spi_sendByte(0xFF);
    CLR_CS;
    _spi_sendByte(0xFF);

    _spi_sendByte(17 | 0x40); //send CMD17, prefixed with '01'
    _spi_sendWord(addr); //send addr
    _spi_sendByte(0x00); //send CRC (N/A)

    resp = _spi_sendByte(0xFF);
    while (resp == 0xFF) {
        resp = _spi_sendByte(0xFF);
    }
    if (resp == 0x00) {
        //2 extra bytes for CRC
        byte = _spi_sendByte(0xFF);
        while (byte == 0xFF) {
            byte = _spi_sendByte(0xFF);
        }
        for (i = 0; i < SECTOR+2; i++) {
            byte = _spi_sendByte(0xFF);
            if (i < SECTOR) {
                buf[i] = byte;
            }
        }
    }

    SET_CS;
    _spi_sendByte(0xFF);

    return (resp == 0x00) ? SD_OKAY : SD_READ_ERR;
}

void sd_error(sdStatus_t err) {
    switch (err) {
        case SD_OKAY:
//            ui_writeLine(0, "SD: no error");
            break;
        case SD_UNRECOGNIZED:
//            ui_writeLine(0, "SD: unrecognized");
            break;
        case SD_UNKNOWN_VERSION:
//            ui_writeLine(0, "SD: version err");
            break;
        case SD_TIMEOUT:
//            ui_writeLine(0, "SD: timeout");
            break;
        case SD_READ_ERR:
//            ui_writeLine(0, "SD: read err");
            break;
    }
}

