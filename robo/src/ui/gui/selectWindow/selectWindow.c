#include "selectWindow.h"
#include "ui/text/text.h"
#include "ui/ili9341/ili9341.h"
#include "state/state.h"
#include "ui/ui.h"
#include "nav/nav.h"

static void touchEvent(void *node, uint16_t x, uint16_t y) {
    selectWindow_t *window = (selectWindow_t *)node;

    if (node_containsPoint((node_t *)&window->nextButton, x, y)) {
        fat_getNextFile(&window->dir, &window->currentFile);
        
        window->prev = preview_create(window->currentFile);
        preview_draw(&window->prev, (WIDTH - window->prev.node.width)/2, (HEIGHT - window->prev.node.height)/2+15);
    
        text_writeTextAtPoint(f_12x16, window->currentFile.name, 0, window->statusBar.node.height+5, CENTER);
    } else if (node_containsPoint((node_t *)&window->prevButton, x, y)) {
        fat_getPreviousFile(&window->dir, &window->currentFile);
        
        window->prev = preview_create(window->currentFile);
        preview_draw(&window->prev, (WIDTH - window->prev.node.width)/2, (HEIGHT - window->prev.node.height)/2+15);
    
        text_writeTextAtPoint(f_12x16, window->currentFile.name, 0, window->statusBar.node.height+5, CENTER);
    } else if (node_containsPoint((node_t *)&window->prev, x, y)) {
        if (nav_setup(window->currentFile) == 0) {
            state_set(STATE_NAV);
            ui_draw();
        }
        return;
    }
}

selectWindow_t selectWindow_create(fatFile_t dir) {
    selectWindow_t window = {0};

    window.node.width = WIDTH;
    window.node.height = HEIGHT;
    window.node.touchEvent = touchEvent;
    window.statusBar = statusBar_create("RoboPicasso");
    window.statusBar.batteryPct = 25;
    window.prevButton = button_create("Prev", f_12x16, color16(0xDDDDDDD));
    window.nextButton = button_create("Next", f_12x16, color16(0xDDDDDDD));
    window.dir = dir;

    return window;
}

void selectWindow_draw(selectWindow_t *window) {
    ili9341_fillLCD(WHITE);
    text_setColors(BLACK, WHITE);
    statusBar_draw(&window->statusBar, 0, 0);
    button_draw(&window->prevButton, 0, HEIGHT - window->prevButton.node.height);
    button_draw(&window->nextButton, WIDTH - window->nextButton.node.width, HEIGHT - window->nextButton.node.height);
    touchEvent(window, window->nextButton.node.x + 1, window->nextButton.node.y + 1);
}

