#include "ui/ui.h"
#include "stm32f0xx.h"

int main(void) {
    ui_setup();
    ui_writeLine(0, "Hello, world!");
    ui_writeLine(1, "ECE 49022");
    while (1) asm("wfi");
    return 0;
}
