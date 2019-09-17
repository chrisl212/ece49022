#ifndef WELCOME_WINDOW_H
#define WELCOME_WINDOW_H

#include "ui/gui/gui.h"

typedef struct {
    node_t node;
} welcomeWindow_t;

welcomeWindow_t welcomeWindow_create(void);
void welcomeWindow_draw(welcomeWindow_t *window);

#endif
