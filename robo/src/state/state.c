#include "stm32f0xx.h"
#include "state.h"
#include "ui/ui.h"

state_t currentState;

void state_setup(void) {
    currentState = STATE_WELCOME;
}

state_t state_get(void) {
    return currentState;
}

void state_set(state_t state) {
    currentState = state;
}

