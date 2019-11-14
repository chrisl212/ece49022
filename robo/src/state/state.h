#ifndef STATE_H
#define STATE_H

typedef enum {
    STATE_WELCOME,
    STATE_SELECT,
    STATE_NAV,
    STATE_ERR,
    STATE_DONE
} state_t;

void state_setup(void);
state_t state_get(void);
void state_restore(void);
void state_set(state_t state);
void state_setErrorMessage(char *s);
char* state_getErrorMessage(void);

#endif

