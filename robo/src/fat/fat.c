#include "stm32f0xx.h"
#include "fat.h"
#include "sd/sd.h"
#include "ui/ui.h"

#define SECTOR (512)
#define SHORTNAME (11)
#define ARCHIVE (0x20)
#define DIRECTORY (0x10)
#define VOL_ID (0x08)
#define OS (0x04)
#define HIDDEN (0x02)
#define RO (0x01)

struct _fatFile {
    char name[SHORTNAME + 1];
    uint32_t offset;
    uint8_t attrib;
    uint32_t firstCluster;
    uint32_t size;
};

struct {
    uint16_t bytesPerSector;
    uint8_t sectorsPerCluster;
    uint16_t reservedSectors;
    uint8_t FATs;
    uint32_t sectorsPerFAT;
    uint32_t rootDirectoryCluster;
    uint16_t signature;
} _volumeID;

struct {
    uint8_t type;
    uint32_t lba;
} _partInfo;

uint8_t buf[SECTOR];

static uint32_t _combine(uint8_t bits, uint8_t *buf, uint16_t start) {
    int i;
    int shifts = bits/8;
    uint32_t res = 0;

    for (i = 0; i < shifts; i++) {
        res |= ((uint32_t)buf[start+i])  << (8 * ((shifts - 1) - i));
    }
    return res;
}

static int _readMBR(void) {
    sd_readSector(0x0, buf);
    _partInfo.type = buf[450];
    _partInfo.lba = _combine(32, buf, 454);

    if (_partInfo.type != 0x0B && _partInfo.type != 0x0C) {
        return FAT_MBR;
    }
    if (buf[510] != 0x55 || buf[511] != 0xAA) {
        return FAT_MBR;
    }

    return FAT_OKAY;
}

static int _readVolumeID(void) {
    sd_readSector(_partInfo.lba, buf);
    _volumeID.bytesPerSector = _combine(16, buf, 0x0B);
    _volumeID.sectorsPerCluster = buf[0x0D];
    _volumeID.reservedSectors = _combine(16, buf, 0x0E);
    _volumeID.FATs = buf[0x10];
    _volumeID.sectorsPerFAT = _combine(32, buf, 0x24);
    _volumeID.rootDirectoryCluster = _combine(32, buf, 0x2C);
    _volumeID.signature = _combine(16, buf, 0x1FE);
    if (_volumeID.signature != 0x55AA) {
        return FAT_VOLID;
    }
    return FAT_OKAY;
}

int fat_init(fatFile_t *root) {
    sd_init();
    if (_readMBR()) {
        return FAT_MBR;
    }
    if (_readVolumeID()) {
        return FAT_VOLID;
    }
    root->name[0] = '/';
    root->name[1] = 0;
    root->offset = 0;
    root->attrib = DIRECTORY;
    root->firstCluster = _volumeID.rootDirectoryCluster;
    root->size = 0;
    return FAT_OKAY;
}

int fat_getNextFile(fatFile_t *directory, fatFile_t *next) {
    return FAT_OKAY;
}

int fat_read(fatFile_t *f, uint8_t *buf, uint8_t len) {
    return FAT_OKAY;
}

void fat_error(fatStatus_t err) {
    switch (err) {
        case FAT_OKAY:
            ui_writeLine(0, "FAT: no error");
            break;
        case FAT_MBR:
            ui_writeLine(0, "FAT: invalid MBR");
            break;
        case FAT_VOLID:
            ui_writeLine(0, "FAT: invalid VOL");
            break;
    }
}

