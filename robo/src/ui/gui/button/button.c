#include <string.h>
#include "button.h"

button_t button_create(char *title, fontHeader_t font, color_t color) {
    button_t button = {0};

    button.color.fillColor = color;
    button.color.strokeColor = BLACK;
    button.color.strokeWidth = 2;
    button.color.fillEnabled = 1;

    button.width = strlen(title) * font.cell_max_width + 10;
    button.height = font.cell_max_height + 10;
    button.font = font;
    button.title = title;
    return button;
}

void button_draw(button_t *button, uint16_t x, uint16_t y) {
    shape_drawRect(shape_rect(x, y, button->width, button->height, button->color));
    text_pushColors();
    text_setColors(WHITE, button->color.fillColor);
    text_writeTextAtPoint(button->font, button->title, x + 5, y + 5, LEFT);
    text_popColors();
}

