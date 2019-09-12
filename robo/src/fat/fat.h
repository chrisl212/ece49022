#ifndef FAT_H
#define FAT_H

#include <stdint.h>
#include "sd/sd.h"

#define SECTOR (512)
#define SHORTNAME (11)
#define ARCHIVE (0x20)
#define DIRECTORY (0x10)
#define VOL_ID (0x08)
#define OS (0x04)
#define HIDDEN (0x02)
#define RO (0x01)

typedef enum {
    FAT_OKAY,
    FAT_MBR,
    FAT_VOLID
} fatStatus_t;

typedef struct _fatFile {
    char name[SHORTNAME + 1];
    uint32_t offset;
    uint8_t attrib;
    uint32_t firstCluster;
    uint32_t size;
    sdCard_t card;
} fatFile_t;

int fat_init(fatFile_t *root);
int fat_getNextFile(fatFile_t *directory, fatFile_t *next);
int fat_read(fatFile_t *f, uint8_t *buf, uint8_t len);
void fat_error(fatStatus_t err);

#endif

