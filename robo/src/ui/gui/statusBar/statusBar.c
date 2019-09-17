#include "ui/gui/shapes/shapes.h"
#include "ui/text/text.h"
#include "ui/ili9341/ili9341.h"
#include "statusBar.h"

statusBar_t statusBar_create(char *title) {
    statusBar_t statusBar;

    statusBar.title = title;
    statusBar.batteryPct = 75;
    statusBar.node.width = WIDTH;
    statusBar.node.height = 20;

    return statusBar;
}

void statusBar_draw(statusBar_t *statusBar, int x, int y) {
    shapeColor_t color = {0};
    color.fillEnabled = 1;
    color.fillColor = color16(0xEFEFEF);
    statusBar->node.x = x;
    statusBar->node.y = y;

    shape_drawRect(shape_rect(x, y, statusBar->node.width, statusBar->node.height, color));
    text_pushColors();

    text_setColors(BLACK, color.fillColor);
    text_writeTextAtPoint(f_08x08, statusBar->title, x, y+6, CENTER);

    text_popColors();

    color.strokeColor = BLACK;
    color.strokeWidth = 2;
    color.fillEnabled = 0;
    shape_drawRect(shape_rect(x+4, y+4, 20, 12, color));

    color.fillColor = BLACK;
    color.strokeWidth = 0;
    color.fillEnabled = 1;
    shape_drawRect(shape_rect(x+24, y+7, 2, 6, color));

    color.fillColor = (statusBar->batteryPct > 20) ? GREEN : RED;
    color.fillEnabled = 1;
    color.strokeWidth = 0;
    shape_drawRect(shape_rect(x+6, y+6, 16*statusBar->batteryPct/100, 8, color));
}
