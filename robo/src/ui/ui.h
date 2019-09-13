#ifndef UI_H
#define UI_H

void ui_setup(void);
void ui_writeLine(int line, char *s);
void ui_writeFormat(int line, char *fmt, ...);
void ui_touchEvent(int x, int y);

#endif

