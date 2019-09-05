#ifndef STATE_H
#define STATE_H

typedef enum {
    STATE_SELECT,
    STATE_CONFIRM,
    STATE_MOVE,
    STATE_BAT,
    STATE_EMPTY
} state_t;

state_t state;

void state_init(void);

#endif

