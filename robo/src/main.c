#include "ui/ui.h"
#include "state/state.h"
#include "drive/drive.h"
#include "sensors/battery/battery.h"

int main(void) {
    state_setup();
    drive_setup();
    ui_setup();
    battery_setup();
/*    if (battery_low() && state_get() != STATE_ERR) {
        state_set(STATE_ERR);
        state_setErrorMessage("BATTERY LOW!!!");
        ui_draw();
	}*/
//    collision_setup();
    
    while (1) {
        asm("wfi");
    }
    return 0;
}
