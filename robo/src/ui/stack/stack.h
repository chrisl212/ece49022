#ifndef STACK_H
#define STACK_H

typedef enum {
    STACK_GRN,
    STACK_YEL,
    STACK_RED
} stackLight_t;

void stack_init(void);
void stack_on(stackLight_t light);
void stack_off(stackLight_t light);
void buzzer_on(void);
void buzzer_off(void);

#endif
