#ifndef NODE_H
#define NODE_H

#include <stdint.h>

typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    void (*touchEvent)(void *, uint16_t, uint16_t);
} node_t;

int node_containsPoint(node_t *node, uint16_t x, uint16_t y);

#endif
