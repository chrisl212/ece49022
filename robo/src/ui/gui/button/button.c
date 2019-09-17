#include "std/std.h"
#include "button.h"

#define PADDING (20)

static void touchEvent(void *node, uint16_t x, uint16_t y) {
    button_t *button = (button_t *)node;

    text_pushColors();
    text_setColors(WHITE, button->color.fillColor);
    text_writeTextAtPoint(button->font, button->title, x + (PADDING/2), y + (PADDING/2), LEFT);
    text_popColors();
}

button_t button_create(char *title, fontHeader_t font, color_t color) {
    button_t button = {0};

    button.color.fillColor = color;
    button.color.strokeColor = BLACK;
    button.color.strokeWidth = 2;
    button.color.fillEnabled = 1;

    button.node.width = strlen(title) * font.cell_max_width + PADDING;
    button.node.height = font.cell_max_height + PADDING;
    button.node.touchEvent = touchEvent;
    button.font = font;
    button.title = title;
    return button;
}

void button_draw(button_t *button, uint16_t x, uint16_t y) {
    button->node.x = x;
    button->node.y = y;
    shape_drawRect(shape_rect(x, y, button->node.width, button->node.height, button->color));
    text_pushColors();
    text_setColors(WHITE, button->color.fillColor);
    text_writeTextAtPoint(button->font, button->title, x + (PADDING/2), y + (PADDING/2), LEFT);
    text_popColors();
}

