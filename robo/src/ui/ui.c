#include <stdarg.h>
#include <string.h>
#include "stack/stack.h"
#include "state/state.h"
#include "fat/fat.h"
#include "stm32f0xx.h"
#include "ili9341/ili9341.h"
#include "text/text.h"
#include "colors/colors.h"
#include "gui/gui.h"
#include "sensors/sensors.h"
#include "ui.h"

#define ROW (2)
#define COL (16)

fatFile_t root;
welcomeWindow_t welcomeWindow;
selectWindow_t selectWindow;
navWindow_t navWindow;

void ui_setup(void) {
    int err;

    ili9341_init();

    fat_init(&root);
    
    welcomeWindow = welcomeWindow_create();
    selectWindow = selectWindow_create(root);
    navWindow = navWindow_create();

    stack_init();
    ui_draw();
    touch_init();
}

void ui_draw(void) {
    stack_off(STACK_GRN);
    stack_off(STACK_YEL);
    stack_off(STACK_RED);
    buzzer_off();
    if (state_get() == STATE_WELCOME) {
        stack_on(STACK_GRN);
        stack_on(STACK_YEL);
        welcomeWindow_draw(&welcomeWindow);
    } else if (state_get() == STATE_SELECT) {
        stack_on(STACK_YEL);
        selectWindow_draw(&selectWindow);
    } else if (state_get() == STATE_NAV) {
        stack_on(STACK_GRN);
        navWindow_draw(&navWindow);
    } else if (state_get() == STATE_DONE) {
        stack_on(STACK_GRN);
        stack_on(STACK_YEL);
        ili9341_fillLCD(WHITE);
        text_writeTextAtPoint(f_12x16, "Done!", 0, 160, CENTER);
    } else if (state_get() == STATE_ERR) {
        stack_on(STACK_RED);
        buzzer_on();
        ili9341_fillLCD(RED);
        text_pushColors();
        text_setColors(WHITE, RED);
        text_writeTextAtPoint(f_12x16, state_getErrorMessage(), 0, 130, CENTER);
        text_writeTextAtPoint(f_08x08, "Tap anywhere to acknowledge", 0, 160, CENTER);
        text_popColors();
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
    } else if (state_get() == STATE_ERR) {
        buzzer_off();
    }
}

