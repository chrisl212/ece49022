#include "node.h"
#include "ui/text/text.h"

void node_touchEvent(node_t *node, uint16_t x, uint16_t y) {
    node->touchEvent(node, x, y);
}

int node_containsPoint(node_t *node, uint16_t x, uint16_t y) {
    if (x > (node->x-5) && x < (node->x + node->width + 5)) {
        if (y > (node->y-5) && y < (node->y + node->height + 5)) {
            return 1;
        }
    }
    return 0;
}
