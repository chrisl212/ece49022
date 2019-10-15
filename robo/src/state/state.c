#include "stm32f0xx.h"
#include "state.h"
#include "ui/ui.h"

state_t currentState;
state_t previousState;

void state_setup(void) {
    currentState = STATE_WELCOME;
}

state_t state_get(void) {
    return currentState;
}

void state_restore(void) {
    state_set(previousState);
}

void state_set(state_t state) {
    if (state != currentState) {
        previousState = currentState;
    }
    currentState = state;
}

