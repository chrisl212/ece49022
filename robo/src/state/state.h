#ifndef STATE_H
#define STATE_H

typedef enum {
    STATE_WELCOME,
    STATE_SELECT,
    STATE_NAV,
    STATE_BAT,
    STATE_EMPTY,
    STATE_DONE
} state_t;

void state_setup(void);
state_t state_get(void);
void state_set(state_t state);

#endif

