#ifndef BUTTON_H
#define BUTTON_H

#include "colors/colors.h"
#include "ui/text/text.h"
#include "ui/gui/shapes/shapes.h"

typedef struct {
    char *title;
    shapeColor_t color;
    fontHeader_t font;
    uint16_t width;
    uint16_t height;
} button_t;

button_t button_create(char *title, fontHeader_t font, color_t color);
void button_draw(button_t *button, uint16_t x, uint16_t y);

#endif

