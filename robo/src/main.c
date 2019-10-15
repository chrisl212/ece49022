#include "ui/ui.h"
#include "state/state.h"
#include "drive/drive.h"
#include "drive/collision/collision.h"
#include "stm32f0xx.h"
#include "ui/text/text.h"
#include "fat/fat.h"

int main(void) {
    state_setup();
    drive_setup();
    ui_setup();
//    collision_setup();
    
    while (1) asm("wfi");
    return 0;
}
