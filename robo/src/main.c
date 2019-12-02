#include "ui/ui.h"
#include "state/state.h"
#include "drive/drive.h"
#include "sensors/mpu6050/mpu6050.h"

int main(void) {
//    state_setup();
//    drive_setup();
//    ui_setup();
//    collision_setup();
    mpu6050_setup();
    
    while (1) {
        asm("wfi");
    }
    return 0;
}
