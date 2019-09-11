#include "ui/ui.h"
#include "state/state.h"
#include "sd/sd.h"
#include "stm32f0xx.h"

int main(void) {
    int status;

    ui_setup();
    status = sd_init();
    if (status != SD_OKAY) {
        sd_error(status);
    } else {
        ui_writeLine(0, "SD initialized");
    }
    state_init();
    
    while (1) asm("wfi");
    return 0;
}
