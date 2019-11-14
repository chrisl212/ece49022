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

typedef enum {
    SPI_OK,
    SPI_SEND,
    SPI_RECV
} spiStatus_t;

static int spiError;

//wait for flag to be set or unset
static int _spi_timeout(int flag, int set) {
    int timeout = 0;

    if (set) {
        while ((SPI2->SR & flag) != flag) {
            if (++timeout > TIMEOUT) {
                return -1;
            }
        }
    } else {
        while (SPI2->SR & flag) {
            if (++timeout > TIMEOUT) {
                return -1;
            }
        }
    }
    return 0;
}

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

static int _spi_sendByte(uint8_t byte, uint8_t *ret) {
    if (_spi_timeout(SPI_SR_TXE, 1)) {
        return SPI_SEND;
    }
    *((uint8_t *)&SPI2->DR) = (uint8_t)byte; //data packing
    if (_spi_timeout(SPI_SR_RXNE, 1)) {
        return SPI_RECV;
    }
    if (ret) {
        *ret = *(uint8_t *)&(SPI2->DR);
    }
    return SPI_OK;
}

static int _spi_sendWord(uint32_t word) {
    int i, s;

    for (i = 0; i < 4; i++) {
        //write MSB first
        if ((s = _spi_sendByte((word >> ((3 - i) * 8)) & 0xFF, NULL)) != SPI_OK) {
            return s;
        }
    }
    return SPI_OK;
}

static int _spi_readWord(uint32_t *ret) {
    uint32_t resp = 0;
    uint8_t tmp;
    int i, s;

    for (i = 0; i < 4; i++) {
        if ((s = _spi_sendByte(0xFF, &tmp)) != SPI_OK) {
            return s;
        }
        resp |= tmp << (8*(3-i));
    }
    if (ret) {
        *ret = resp;
    }
    return SPI_OK;
}

static void delay(int n) { 
    while (n--); 
}

static int _spi_sendCommand(uint8_t idx, uint32_t arg, uint8_t a, uint32_t *ocr, uint8_t *ret) {
    uint8_t crc = 0x00;
    uint8_t resp = 0xFF;
    int s;

    if (a) {
        if ((s =_spi_sendCommand(55, 0, 0, ocr, NULL)) != SPI_OK) {
            return s;
        }
    }

    if ((s = _spi_sendByte(0xFF, NULL)) != SPI_OK) {
        return s;
    }
    CLR_CS;
    if ((s = _spi_sendByte(0xFF, NULL)) != SPI_OK) {
        return s;
    }

    //send index, prefixed with '01'
    if ((s = _spi_sendByte(idx | 0x40, NULL)) != SPI_OK) {
        return s;
    }
    
    //send argument
    if ((s =_spi_sendWord(arg)) != SPI_OK) {
        return s;  
    }
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
    if ((s =_spi_sendByte(crc, NULL)) != SPI_OK) {
        return s;
    }
   
    while (resp == 0xFF && (idx != 55)) {
        if ((s = _spi_sendByte(0xFF, &resp)) != SPI_OK) {
            return s;  
        } //read until get a proper response
    }
    if (idx == 8 || idx == 58) {
        if ((s =_spi_readWord(ocr)) != SPI_OK) {
            return s;
        }
    }
    SET_CS;
    if ((s = _spi_sendByte(0xFF, NULL)) != SPI_OK) {
        return s;
    }
    if (ret) {
        *ret = resp;
    }
    return SPI_OK;
}

int _pulse(void) {
    int i, s;

    //send at least 74 (80) clock pulses following power on
    //NSS remains high
    for (i = 0; i < 10; i++) {
        if ((s = _spi_sendByte(0xFF, NULL)) != SPI_OK) {
            return s;
        }
    }
    return SPI_OK;
}

int sd_init(sdCard_t *card) {
    uint8_t res;
    uint8_t timeout = 0;
    uint32_t ocr;

    card->mode = SD_BYTE;

    _spi_init();
    if ((spiError = _pulse()) != SPI_OK) {
        return SD_SPI;
    }

    if ((spiError = _spi_sendCommand(0, 0, 0, NULL, &res)) != SPI_OK) {
        return SD_SPI;
    }
    if (res != 0x01) {
        return SD_UNRECOGNIZED;
    }
    
    if ((spiError = _spi_sendCommand(8, 0x01AA, 0, &ocr, &res)) != SPI_OK) {
        return SD_SPI;
    }

    if (res != 0x01 || ocr != 0x01AA) {
        return SD_UNRECOGNIZED;
    }

    if ((spiError = _spi_sendCommand(41, 0x40000000, 1, NULL, &res)) != SPI_OK) {
        return SD_SPI;
    }

    if (res != 0x00) {
        if (res == 0x01) {
            do {
                if ((spiError = _spi_sendCommand(41, 0x40000000, 1, NULL, &res)) != SPI_OK) {
                    return SD_SPI;
                }
                if (++timeout > TIMEOUT) {
                    return SD_TIMEOUT;
                }
                delay(100000);
            } while (res != 0x00);
        } else if (res == 0x05) {
            do {
                if ((spiError = _spi_sendCommand(1, 0, 0, NULL, &res)) != SPI_OK) {
                    return SD_SPI;
                }
                if (++timeout > TIMEOUT) {
                    return SD_TIMEOUT;
                }
                delay(100000);
            } while (res != 0x00);
        } else {
            return SD_UNKNOWN_VERSION;
        } 
    }
    if ((spiError = _spi_sendCommand(58, 0, 0, &ocr, &res)) != SPI_OK) {
        return SD_SPI;
    }
    if (ocr & 0x40000000) {
        card->mode = SD_BLOCK;
    }

    if ((spiError = _spi_sendCommand(16, 0x200, 0, NULL, &res)) != SPI_OK) {
        return SD_SPI;
    }
    if (res != 0x00) {
        return SD_UNKNOWN;
    }

    //adjust SPI frequency
    SPI2->CR1 &= ~(SPI_CR1_BR);
    SPI2->CR1 |= SPI_CR1_BR_0;
    return SD_OK;
}

int sd_readSector(sdCard_t card, uint32_t addr, uint8_t *buf) {
    int i, timeout = 0;
    uint8_t byte, resp;

    if (card.mode == SD_BYTE) {
        addr *= 512;
    }

    if ((spiError = _spi_sendByte(0xFF, NULL)) != SPI_OK) {
        return SD_SPI;
    }
    CLR_CS;
    if ((spiError = _spi_sendByte(0xFF, NULL)) != SPI_OK) {
        return SD_SPI;
    }

    //send CMD17, prefixed with '01'
    if ((spiError = _spi_sendByte(17 | 0x40, NULL)) != SPI_OK) {
        return SD_SPI;
    }
    
    //send addr
    if ((spiError = _spi_sendWord(addr)) != SPI_OK) {
        return SD_SPI;  
    }

    //send CRC (N/A)
    if ((spiError = _spi_sendByte(0x00, NULL)) != SPI_OK) {
        return SD_SPI;
    } 

    if ((spiError = _spi_sendByte(0xFF, &resp)) != SPI_OK) {
        return SD_SPI;
    }

    while (resp == 0xFF) {
        if ((spiError = _spi_sendByte(0xFF, &resp)) != SPI_OK) {
            return SD_SPI;
        }
        if (++timeout > TIMEOUT) {
            return SD_TIMEOUT;
        }
    }

    timeout = 0;
    if (resp == 0x00) {
        //2 extra bytes for CRC
        if ((spiError = _spi_sendByte(0xFF, &byte)) != SPI_OK) {
            return SD_SPI;
        }
        while (byte == 0xFF) {
            if ((spiError = _spi_sendByte(0xFF, &byte)) != SPI_OK) {
                return SD_SPI;
            }
            if (++timeout > TIMEOUT) {
                return SD_TIMEOUT;
            }
        }
        for (i = 0; i < SECTOR+2; i++) {
            if ((spiError = _spi_sendByte(0xFF, &byte)) != SPI_OK) {
                return SD_SPI;
            }
            if (i < SECTOR) {
                buf[i] = byte;
            }
        }
    }

    SET_CS;
    if ((spiError = _spi_sendByte(0xFF, NULL)) != SPI_OK) {
        return SD_SPI;
    }

    return (resp == 0x00) ? SD_OK : SD_READ_ERR;
}

char* _spi_error(spiStatus_t s) {
    switch (s) {
        case SPI_OK:
            return "spi: no error";
        case SPI_SEND:
            return "spi: send error";
        case SPI_RECV:
            return "spi: receive error";
    }
    return "spi: unknown error";
}

char* sd_error(sdStatus_t err) {
    switch (err) {
        case SD_OK:
            return "sd: no error";
        case SD_UNRECOGNIZED:
            return "sd: unrecognized";
        case SD_UNKNOWN_VERSION:
            return "sd: unknown version";
        case SD_TIMEOUT:
            return "sd: timeout";
        case SD_READ_ERR:
            return "sd: read error";
        case SD_SPI:
            return _spi_error(spiError);
        default:
            return "sd: unknown error";
    }
    return "sd: unknown error";
}

