#include "welcomeWindow.h"
#include "ui/ili9341/ili9341.h"
#include "ui/text/text.h"
#include "ui/ui.h"
#include "state/state.h"

static void touchEvent(void *node, uint16_t x, uint16_t y) {
    state_set(STATE_SELECT);
    ui_draw();
}

welcomeWindow_t welcomeWindow_create(void) {
    welcomeWindow_t window;

    window.node.width = WIDTH;
    window.node.height = HEIGHT;
    window.node.touchEvent = touchEvent;

    return window;
}

void welcomeWindow_draw(welcomeWindow_t *window) {
    ili9341_fillLCD(WHITE);

    text_pushColors();
    text_setColors(BLACK, WHITE);
    text_writeTextAtPoint(f_12x16, "RoboPicasso", 0, 40, CENTER);
    text_writeTextAtPoint(f_08x08, "Tap anywhere to continue", 0, 100, CENTER);
    text_popColors();
}
