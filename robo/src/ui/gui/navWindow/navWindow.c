#include "navWindow.h"
#include "ui/text/text.h"
#include "ui/ili9341/ili9341.h"
#include "nav/nav.h"
#include "drive/drive.h"

int8_t incDec = 1;

static void touchEvent(void *node, uint16_t x, uint16_t y) {
    navWindow_t *window = (navWindow_t *)node;
    uint16_t angle; 
    uint8_t speed, paint;

    if (node_containsPoint((node_t *)&window->nextButton, x, y)) {
        nav_move(&angle, &speed, &paint);
        text_pushColors();
        text_setColors(BLACK, WHITE);
        text_writeFormatAtPoint(f_12x16, 0, 0, LEFT, "(%d, %d, %d)    ", angle, speed, paint);
        text_popColors();
    } else if (node_containsPoint((node_t *)&window->pidButton1, x, y)) {
        headPID.Kd += 1 * incDec;
    } else if (node_containsPoint((node_t *)&window->pidButton2, x, y)) {
        headPID.Ki += 1 * incDec;
    } else if (node_containsPoint((node_t *)&window->incDecButton, x, y)) {
        incDec = (incDec == 1) ? -1 : 1;
        text_writeFormatAtPoint(f_12x16, 0, 90, LEFT, "%s", (incDec == 1) ? "inc" : "dec");
    }
    text_writeFormatAtPoint(f_12x16, 0, 60, LEFT, "(Kd, Ki) = (%d, %d)    ", headPID.Kd, headPID.Ki);
}

navWindow_t navWindow_create(void) {
    navWindow_t window = {0};

    window.node.width = WIDTH;
    window.node.height = HEIGHT;
    window.node.touchEvent = touchEvent;
    window.nextButton = button_create("Next", f_12x16, color16(0xDDDDDD));
    window.pidButton1 = button_create("  Kd  ", f_12x16, color16(0xDDDDDD));
    window.pidButton2 = button_create("  Ki  ", f_12x16, color16(0xDDDDDD));
    window.incDecButton = button_create("inc/dec", f_12x16, color16(0xDDDDDD));

    return window;
}

void navWindow_draw(navWindow_t *window) {
    ili9341_fillLCD(WHITE);
    button_draw(&window->nextButton, 0, HEIGHT - window->nextButton.node.height);
    button_draw(&window->pidButton1, WIDTH - window->pidButton1.node.width, HEIGHT - 3*window->nextButton.node.height);
    button_draw(&window->pidButton2, WIDTH - window->pidButton2.node.width, HEIGHT - window->nextButton.node.height);
    button_draw(&window->incDecButton, 0, HEIGHT - 3*window->nextButton.node.height);
}
