#ifndef SD_H
#define SD_H

#include <stdint.h>

typedef enum {
    SD_OKAY,
    SD_INIT_FAIL
} sdStatus_t;

int sd_init(void);
int sd_readBlock(uint8_t *buf);

#endif

