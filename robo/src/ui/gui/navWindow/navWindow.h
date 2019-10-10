#ifndef NAV_WINDOW_H
#define NAV_WINDOW_H

#include "ui/gui/node/node.h"
#include "ui/gui/button/button.h"

typedef struct {
    node_t node;
    button_t nextButton;
    button_t pidButton1;
    button_t pidButton2;
    button_t incDecButton;
} navWindow_t;

navWindow_t navWindow_create(void);
void navWindow_draw(navWindow_t *window);

#endif

