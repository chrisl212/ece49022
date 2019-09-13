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
uint32_t lbaLast = 0xFFFFFFFF;

static int _fat_readSector(sdCard_t card, uint32_t lba) {
    if (lba != lbaLast) {
        lbaLast = lba;
        return sd_readSector(card, lba, buf);
    }
    return SD_OKAY;
}

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

    status = _fat_readSector(card, 0x0);
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

    status = _fat_readSector(card, _partInfo.lba);
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

static uint32_t _clusterToLBA(uint32_t cluster) {
    return _volumeID.lbaCluster + (cluster - 2) * _volumeID.sectorsPerCluster;
}

static uint32_t _fat_getNthCluster(sdCard_t card, uint32_t first, uint16_t n) {
    uint32_t lba = _volumeID.lbaFAT;
    uint32_t cluster = first;

    if (cluster > SECTOR * _volumeID.sectorsPerFAT) {
        lba += 1;
    }
    _fat_readSector(card, lba);

    while (n--) {
        cluster = _combine(32, buf, cluster*4);
        if (cluster == 0xFFFFFFF) {
            break;
        }
        if (cluster > SECTOR * _volumeID.sectorsPerFAT) {
            lba += 1;
            _fat_readSector(card, lba);
        }
    }
    return cluster;
}

int fat_getPreviousFile(fatFile_t *dir, fatFile_t *next) {
    int status;
    uint32_t offsetInitial = dir->offset;
    int32_t offsetNew = 0;
    uint32_t cnt;
    
    if (offsetInitial == 0) {
        return fat_getNextFile(dir, next);
    }

    while (dir->offset == offsetInitial) {
        offsetNew -= 32;
        dir->offset = offsetInitial + offsetNew;
        status = fat_getNextFile(dir, next);
        if (-offsetNew >= offsetInitial) {
            return status;
        }
    }
    return status;
}

int fat_getNextFile(fatFile_t *dir, fatFile_t *next) {
    uint8_t entry[32];
    uint8_t valid = 0;
    int i = 0;

    while (!valid) {
        fat_read(dir, entry, 32);
        if (entry[0] == 0) {
            dir->offset = 0;
        } else if (entry[0] != 0xE5 && !(entry[SHORTNAME] & VOL_ID)) {
            valid = 1;
        }
    }
    for (i = 0; i < SHORTNAME; i++) {
        next->name[i] = entry[i];
    }
    next->name[SHORTNAME-1] = 0;
    next->offset = 0;
    next->attrib = entry[SHORTNAME];
    next->firstCluster = (_combine(16, entry, 0x14) << 16) | _combine(16, entry, 0x1A);
    next->size = _combine(32, entry, 0x1C);
    next->card = dir->card;

    return FAT_OKAY;
}

int fat_read(fatFile_t *f, uint8_t *b, uint8_t len) {
    uint32_t clusterIdx, sectorIdx, byteIdx, lba = _clusterToLBA(f->firstCluster);
    int status, i = 0;
    
    clusterIdx = f->offset / (SECTOR * _volumeID.sectorsPerCluster);
    sectorIdx = (f->offset / SECTOR) - (clusterIdx * _volumeID.sectorsPerCluster);
    byteIdx = f->offset % SECTOR;
    
    if (f->size == 0 && !(f->attrib & DIRECTORY)) {
        return FAT_EOF;
    }

    if (clusterIdx > 0) {
        lba = _clusterToLBA(_fat_getNthCluster(f->card, f->firstCluster, clusterIdx));
    }
    lba += sectorIdx;

    //ui_writeFormat(1, "%d %d %d %d", byteIdx, sectorIdx, clusterIdx, lba);
    status = _fat_readSector(f->card, lba);
    if (status != SD_OKAY) {
        return FAT_UNKNOWN;
    }
    for (i = 0; i < len; i++) {
        b[i] = buf[byteIdx++];
        f->offset++;

        if (f->offset >= f->size && !(f->attrib & DIRECTORY)) {
            f->offset = 0;
            return FAT_EOF;
        }
        if (byteIdx >= SECTOR) {
            sectorIdx++;
            if (sectorIdx >= _volumeID.sectorsPerCluster) {
                lba = _clusterToLBA(_fat_getNthCluster(f->card, clusterIdx++, 1));
                sectorIdx = 0;
            } else {
                lba += 1;
            }
            status = _fat_readSector(f->card, lba);
            if (status != SD_OKAY) {
                return FAT_UNKNOWN;
            }
            byteIdx = 0;
        }
    }
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
        case FAT_EOF:
            ui_writeLine(0, "FAT: eof");
            break;
        case FAT_UNKNOWN:
            ui_writeLine(0, "FAT: unknown");
            break;
    }
}

