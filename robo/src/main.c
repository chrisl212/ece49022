#include "ui/ui.h"
#include "state/state.h"
#include "stm32f0xx.h"

int main(void) {
    ui_setup();
    state_init();
    while (1) asm("wfi");
    return 0;
}
