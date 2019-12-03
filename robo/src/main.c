#include "ui/ui.h"
#include "state/state.h"
#include "drive/drive.h"

int main(void) {
    state_setup();
    drive_setup();
    ui_setup();
    collision_setup();
    
    while (1) {
        asm("wfi");
    }
    return 0;
}
