#include "ui/ui.h"
#include "state/state.h"
#include "stm32f0xx.h"
#include "ui/text/text.h"
#include "fat/fat.h"

int main(void) {
    state_setup();
    ui_setup();
    
    while (1) asm("wfi");
    return 0;
}
