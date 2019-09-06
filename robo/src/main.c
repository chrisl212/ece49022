#include "ui/ui.h"
#include "state/state.h"
#include "sd/sd.h"
#include "stm32f0xx.h"

int main(void) {
    ui_setup();
    state_init();
    sd_error(SD_INIT_FAIL);
    while (1) asm("wfi");
    return 0;
}
