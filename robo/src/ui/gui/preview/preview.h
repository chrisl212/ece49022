#ifndef PREVIEW_H
#define PREVIEW_H

#include <stdint.h>
#include "fat/fat.h"
#include "ui/gui/node/node.h"

typedef struct {
    node_t node;
    fatFile_t file;
} preview_t;

preview_t preview_create(fatFile_t file);
void preview_draw(preview_t *prev, uint16_t x, uint16_t y);

#endif
