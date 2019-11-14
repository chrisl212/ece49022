#ifndef SD_H
#define SD_H

#include <stdint.h>

typedef enum {
    SD_OK,
    SD_UNRECOGNIZED,
    SD_UNKNOWN_VERSION,
    SD_TIMEOUT,
    SD_READ_ERR,
    SD_UNKNOWN,
    SD_SPI
} sdStatus_t;

typedef enum {
    SD_BYTE,
    SD_BLOCK
} sdAddressingMode_t;

typedef struct {
    sdAddressingMode_t mode;
} sdCard_t;

int sd_init(sdCard_t *card);
int sd_readSector(sdCard_t card, uint32_t addr, uint8_t *buf);
char* sd_error(sdStatus_t err);

#endif

