#include "preview.h"
#include "ui/gui/shapes/shapes.h"

preview_t preview_create(fatFile_t file) {
    preview_t prev = {0};

    prev.node.width = 175;
    prev.node.height = 175;
    prev.file = file;

    return prev;
}

void preview_draw(preview_t *prev, uint16_t x, uint16_t y) {
    uint8_t buf[12] = {0};
    uint16_t xa, ya, paint;
    uint16_t last[2] = {0};
    int status;
    shapeColor_t color = {0};

    prev->node.x = x;
    prev->node.y = y;
    
    color.fillColor = WHITE;
    color.strokeColor = BLACK;
    color.strokeWidth = 1;
    color.fillEnabled = 1;
    shape_drawRect(shape_rect(x, y, prev->node.width, prev->node.height, color));

    do {
        status = fat_read(&prev->file, buf, 12);
        xa = (buf[1] << 8) | buf[0];
        ya = (buf[5] << 8) | buf[4];
        paint = (buf[9] << 8) | buf[8];
        xa = xa * prev->node.width / 1000 + x;
        ya = ya * prev->node.height / 1000 + y;
        if (paint) {
            shape_drawLine(shape_line(last[0], last[1], xa, ya, RED));
        }
        last[0] = xa;
        last[1] = ya;
    } while (status != FAT_EOF);
}
