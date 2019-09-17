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

fatFile_t root;
welcomeWindow_t welcomeWindow;
selectWindow_t selectWindow;
navWindow_t navWindow;

void ui_setup(void) {
    ili9341_init();

    fat_init(&root);
    
    welcomeWindow = welcomeWindow_create();
    selectWindow = selectWindow_create(root);
    navWindow = navWindow_create();

    ui_draw();
    touch_init();
}

void ui_draw(void) {
    if (state_get() == STATE_WELCOME) {
        welcomeWindow_draw(&welcomeWindow);
    } else if (state_get() == STATE_SELECT) {
        selectWindow_draw(&selectWindow);
    } else if (state_get() == STATE_NAV) {
        navWindow_draw(&navWindow);
    } else if (state_get() == STATE_DONE) {
        ili9341_fillLCD(WHITE);
        text_writeTextAtPoint(f_12x16, "Done!", 0, 160, CENTER);
    }
}

void ui_touchEvent(int x, int y) {
    if (state_get() == STATE_WELCOME) {
        welcomeWindow.node.touchEvent((void *)&welcomeWindow, x, y);
    } else if (state_get() == STATE_SELECT) {
        selectWindow.node.touchEvent((void *)&selectWindow, x, y);
    } else if (state_get() == STATE_NAV) {
        navWindow.node.touchEvent((void *)&navWindow, x, y);
    } else if (state_get() == STATE_DONE) {
        state_set(STATE_WELCOME);
        ui_draw();
    }
}

