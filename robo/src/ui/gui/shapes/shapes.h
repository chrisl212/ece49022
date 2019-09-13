#ifndef SHAPES_H
#define SHAPES_H

#include "colors/colors.h"

typedef struct {
    color_t fillColor;
    color_t strokeColor;
    color_t accentColor;
    uint16_t strokeWidth;
    uint16_t fillEnabled;
} shapeColor_t;

typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    shapeColor_t color;
} shapeRect_t;

typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t radius;
    shapeColor_t color;
} shapeCirc_t;

typedef struct {
    uint16_t x0;
    uint16_t y0;
    uint16_t x1;
    uint16_t y1;
    color_t color;
} shapeLine_t;

shapeColor_t shape_color(color_t fillColor, color_t strokeColor, uint16_t strokeWidth, uint16_t fillEnabled);
shapeRect_t shape_rect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, shapeColor_t color);
shapeLine_t shape_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, color_t color);
shapeCirc_t shape_circ(uint16_t x, uint16_t y, uint16_t radius, shapeColor_t color);
void shape_drawRect(shapeRect_t rect);
void shape_drawCirc(shapeCirc_t circ);
void shape_drawLine(shapeLine_t line);
void shape_clearLine(shapeLine_t line, color_t color);

#endif
