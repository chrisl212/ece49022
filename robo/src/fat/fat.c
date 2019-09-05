#include "stm32f0xx.h"
#include "fat.h"
#include "sd/sd.h"

void fat_init(void) {
    sd_init();
}

fatFile_t fat_getNextFile(void) {
    fatFile_t f = {0};

    return f;
}

void fat_read(fatFile_t *f, uint8_t *buf, uint8_t len) {

}
