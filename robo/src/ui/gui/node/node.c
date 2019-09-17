#include "node.h"
#include "ui/text/text.h"

int node_containsPoint(node_t *node, uint16_t x, uint16_t y) {
    if (x > node->x && x < (node->x + node->width)) {
        if (y > node->y && y < (node->y + node->height)) {
            return 1;
        }
    }
    return 0;
}
