#ifndef FAT_H
#define FAT_H

typedef struct {
    char *name;
    int offset;
} fatFile_t;

void fat_init(void);
fatFile_t fat_getNextFile(void);
void fat_read(fatFile_t *f, uint8_t *buf, uint8_t len);

#endif

