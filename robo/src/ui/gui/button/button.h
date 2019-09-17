#ifndef BUTTON_H
#define BUTTON_H

#include "colors/colors.h"
#include "ui/text/text.h"
#include "ui/gui/shapes/shapes.h"
#include "ui/gui/node/node.h"

typedef struct {
    node_t node;
    char *title;
    shapeColor_t color;
    fontHeader_t font;
} button_t;

button_t button_create(char *title, fontHeader_t font, color_t color);
void button_draw(button_t *button, uint16_t x, uint16_t y);

#endif

