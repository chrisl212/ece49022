#include "stm32f0xx.h"
#include "state.h"
#include "ui/ui.h"

static state_t currentState;
static state_t previousState;
static char *errorMessage;

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

void state_setErrorMessage(char *s) {
    errorMessage = s;
}

char* state_getErrorMessage(void) {
    return errorMessage;
}

