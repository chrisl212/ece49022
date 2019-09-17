#ifndef STATUS_H
#define STATUS_H

typedef struct {
    char *title;
    uint8_t batteryPct;
} statusBar_t;

statusBar_t statusBar_create(char *title);
void statusBar_draw(statusBar_t statusBar, int x, int y);

#endif

