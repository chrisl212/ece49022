#ifndef FAT_H
#define FAT_H

#include <stdint.h>

typedef enum {
    FAT_OKAY,
    FAT_MBR,
    FAT_VOLID
} fatStatus_t;

typedef struct _fatFile fatFile_t;

int fat_init(fatFile_t *root);
int fat_getNextFile(fatFile_t *directory, fatFile_t *next);
int fat_read(fatFile_t *f, uint8_t *buf, uint8_t len);
void fat_error(fatStatus_t err);

#endif

