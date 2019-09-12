#include "stm32f0xx.h"
#include "fat.h"
#include "ui/ui.h"

struct {
    uint16_t bytesPerSector;
    uint8_t sectorsPerCluster;
    uint16_t reservedSectors;
    uint8_t FATs;
    uint32_t sectorsPerFAT;
    uint32_t rootDirectoryCluster;
    uint16_t signature;
    uint32_t lbaFAT;
    uint32_t lbaCluster;
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
        res |= ((uint32_t)buf[start+i])  << (8 * i);
    }
    return res;
}

static int _readMBR(sdCard_t card) {
    int status;

    status = sd_readSector(card, 0x0, buf);
    if (status != SD_OKAY) {
        sd_error(status);
        return FAT_MBR;
    }

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

static int _readVolumeID(sdCard_t card) {
    int status;

    status = sd_readSector(card, _partInfo.lba, buf);
    if (status != SD_OKAY) {
        sd_error(status);
        return FAT_VOLID;
    }

    _volumeID.bytesPerSector = _combine(16, buf, 0x0B);
    _volumeID.sectorsPerCluster = buf[0x0D];
    _volumeID.reservedSectors = _combine(16, buf, 0x0E);
    _volumeID.FATs = buf[0x10];
    _volumeID.sectorsPerFAT = _combine(32, buf, 0x24);
    _volumeID.rootDirectoryCluster = _combine(32, buf, 0x2C);
    _volumeID.signature = _combine(16, buf, 0x1FE);
    _volumeID.lbaFAT = _partInfo.lba + _volumeID.reservedSectors;
    _volumeID.lbaCluster = _volumeID.lbaFAT + (_volumeID.FATs * _volumeID.sectorsPerFAT);
    if (_volumeID.signature != 0xAA55) {
        return FAT_VOLID;
    }
    return FAT_OKAY;
}

int fat_init(fatFile_t *root) {
    sd_init(&root->card);
    if (_readMBR(root->card)) {
        return FAT_MBR;
    }
    if (_readVolumeID(root->card)) {
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
    int status, i;
    int idx;
    uint32_t lba = _volumeID.lbaCluster + (directory->firstCluster - 2) * _volumeID.sectorsPerCluster;

    status = sd_readSector(directory->card, lba, buf);

    do {
        idx = directory->offset++ * 32;
    } while (buf[idx] == 0 || buf[idx] == 0xE5);

    for (i = 0; i < 11; i++) {
        next->name[i] = buf[idx + i];
    }
    next->name[11] = 0;
    next->attrib = buf[idx + 11];

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

