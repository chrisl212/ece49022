#ifndef SELECT_WINDOW_H
#define SELECT_WINDOW_H

#include "fat/fat.h"
#include "ui/gui/gui.h"

typedef struct {
    node_t node;
    button_t prevButton;
    button_t nextButton;
    statusBar_t statusBar;
    preview_t prev;
    fatFile_t dir;
    fatFile_t currentFile;
} selectWindow_t;

selectWindow_t selectWindow_create(fatFile_t dir);
void selectWindow_draw(selectWindow_t *window);

#endif
