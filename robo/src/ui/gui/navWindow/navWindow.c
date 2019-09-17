#include "navWindow.h"
#include "ui/text/text.h"
#include "ui/ili9341/ili9341.h"
#include "nav/nav.h"

static void touchEvent(void *node, uint16_t x, uint16_t y) {
    navWindow_t *window = (navWindow_t *)node;
    uint16_t angle; 
    uint8_t speed, paint;

    if (node_containsPoint((node_t *)&window->nextButton, x, y)) {
        nav_move(&angle, &speed, &paint);
        text_pushColors();
        text_setColors(BLACK, WHITE);
        text_writeFormatAtPoint(f_12x16, 0, 50, LEFT, "(%d, %d, %d)", angle, speed, paint);
        text_popColors();
    }
}

navWindow_t navWindow_create(void) {
    navWindow_t window = {0};

    window.node.width = WIDTH;
    window.node.height = HEIGHT;
    window.node.touchEvent = touchEvent;
    window.nextButton = button_create("Next point", f_12x16, color16(0xDDDDDD));

    return window;
}

void navWindow_draw(navWindow_t *window) {
    ili9341_fillLCD(WHITE);
    button_draw(&window->nextButton, 0, HEIGHT - window->nextButton.node.height);
}
