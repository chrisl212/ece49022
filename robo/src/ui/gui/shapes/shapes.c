#include "ui/ili9341/ili9341.h"
#include "shapes.h"
#include "wu.h"

shapeColor_t shape_color(color_t fillColor, color_t strokeColor, uint16_t strokeWidth, uint16_t fillEnabled) {
    shapeColor_t color;
    color.fillColor = fillColor;
    color.strokeColor = strokeColor;
    color.accentColor = strokeColor;
    color.strokeWidth = strokeWidth;
    color.fillEnabled = fillEnabled;
    return color;
}

shapeRect_t shape_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, shapeColor_t color) {
    shapeRect_t s;
    s.x = x;
    s.y = y;
    s.width = width;
    s.height = height;
    s.color = color;
    return s;
}

shapeCirc_t shape_circ(uint16_t x, uint16_t y, uint16_t radius, shapeColor_t color) {
    shapeCirc_t circ;
    circ.x = x;
    circ.y = y;
    circ.radius = radius;
    circ.color = color;
    return circ;
}

shapeLine_t shape_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, color_t color) {
    shapeLine_t line;
    line.x0 = x0;
    line.y0 = y0;
    line.x1 = x1;
    line.y1 = y1;
    line.color = color;
    return line;
}

void shape_drawRect(shapeRect_t rect) {
    uint16_t x, y;
    uint16_t border = rect.color.strokeWidth;

    for (y = 0; y < rect.height; y++) {
        ili9341_setDisplayWindow(rect.x, rect.y + y, WIDTH-1, HEIGHT-1);
        if (rect.color.fillEnabled) {
            for (x = 0; x < rect.width; x++) {
                if (y < border || x < border || y >= rect.height - border || x >= rect.width - border) {
                    ili9341_writePixel(rect.color.strokeColor);
                } else {
                    ili9341_writePixel(rect.color.fillColor);
                }
            }
        } else {
            if (y < border || y >= rect.height - border) {
                for (x = 0; x < rect.width; x++) {
                    ili9341_writePixel(rect.color.strokeColor);
                }
            } else {
                for (x = 0; x < border; x++) {
                    ili9341_writePixel(rect.color.strokeColor);
                }
                ili9341_setDisplayWindow(rect.x + rect.width - border, rect.y + y, WIDTH-1, HEIGHT-1);
                for (x = 0; x < border; x++) {
                    ili9341_writePixel(rect.color.strokeColor);
                }
            }
        }
    }
}

void shape_drawCirc(shapeCirc_t circ) {
    uint16_t x = circ.radius, y;
    int16_t radiusError = 1 - (int16_t)x;
    uint16_t radius;
    color_t color;

    for (radius = circ.radius; radius > 0; radius--) {
        x = radius;
        y = 0;
        if (circ.radius - radius < circ.color.strokeWidth) {
            color = circ.color.strokeColor;
        } else {
            if (circ.color.fillEnabled) {
                color = circ.color.fillColor;
            } else {
                break;
            }
        }
        while (x >= y) {
            ili9341_writePixelAtPoint(circ.x - y, circ.y + x, color);
            ili9341_writePixelAtPoint(circ.x + y, circ.y + x, color);
            ili9341_writePixelAtPoint(circ.x - x, circ.y + y, color);
            ili9341_writePixelAtPoint(circ.x + x, circ.y + y, color);
            ili9341_writePixelAtPoint(circ.x - x, circ.y - y, color);
            ili9341_writePixelAtPoint(circ.x + x, circ.y - y, color);
            ili9341_writePixelAtPoint(circ.x - y, circ.y - x, color);
            ili9341_writePixelAtPoint(circ.x + y, circ.y - x, color);
            y++;
            if (radiusError < 0) {
                radiusError += (int16_t)(2*y + 1);
            } else {
                x--;
                radiusError += 2 * (((int16_t)y - (int16_t)x) + 1);
            }
        }
    }
}

void shape_clearLine(shapeLine_t line, color_t color) {
    wu(line.x0, line.x1, line.y0, line.y1, color);
}

void shape_drawLine(shapeLine_t line) {
    wu(line.x0, line.x1, line.y0, line.y1, line.color);
}
