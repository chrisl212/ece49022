#include <stdarg.h>
#include <string.h>
#include "state/state.h"
#include "fat/fat.h"
#include "stm32f0xx.h"
#include "ili9341/ili9341.h"
#include "text/text.h"
#include "colors/colors.h"
#include "gui/gui.h"
#include "touch/touch.h"
#include "ui.h"

#define ROW (2)
#define COL (16)

fatFile_t root, currentFile;
char lines[ROW][COL] = {0};
int z = 0;
button_t prevButton, nextButton;
statusBar_t statusBar;
preview_t prev;

void ui_setup(void) {
    ili9341_init();
    ili9341_fillLCD(WHITE);
    text_setColors(BLACK, WHITE);
    statusBar = statusBar_create("RoboPicasso");
    statusBar.batteryPct = 25;
    statusBar_draw(&statusBar, 0, 0);
    
    prevButton = button_create("Prev", f_12x16, color16(0xDDDDDDD));
    button_draw(&prevButton, 0, HEIGHT - prevButton.node.height);

    nextButton = button_create("Next", f_12x16, color16(0xDDDDDDD));
    button_draw(&nextButton, WIDTH - nextButton.node.width, HEIGHT - nextButton.node.height);

    fat_init(&root);
    touch_init();
}

void ui_touchEvent(int x, int y) {
    if (state == STATE_SELECT) {
        if (node_containsPoint((node_t *)&nextButton, x, y)) {
            fat_getNextFile(&root, &currentFile);
        } else if (node_containsPoint((node_t *)&prevButton, x, y)) {
            fat_getPreviousFile(&root, &currentFile);
        }
        prev = preview_create(currentFile);
        preview_draw(&prev, (WIDTH - prev.node.width)/2, (HEIGHT - prev.node.height)/2);
        text_writeFormatAtPoint(f_12x16, 0, statusBar.node.height+5, CENTER, "Design: %s", currentFile.name);
    }
}

