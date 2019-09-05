#include "stm32f0xx.h"
#include "state.h"
#include "ui/ui.h"

void state_init(void) {
    state = STATE_SELECT;
}
