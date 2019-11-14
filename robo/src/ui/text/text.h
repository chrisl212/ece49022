#ifndef TEXT_H
#define TEXT_H

#include "ui/colors/colors.h"
#include "font.h"

typedef enum {
    LEFT,
    CENTER,
    RIGHT
} textAlignment_t;

void text_clearLine(fontHeader_t font, uint16_t x0, uint16_t y0);
void text_writeCharacterAtPoint(fontHeader_t font, char c, uint16_t x0, uint16_t y0);
void text_writeTextAtPoint(fontHeader_t font, char *str, uint16_t x, uint16_t y, textAlignment_t align);
void text_writeFormatAtPoint(fontHeader_t font, uint16_t x, uint16_t y, textAlignment_t align, char *fmt, ...);
void text_writeTextOnLine(fontHeader_t font, char *str, uint16_t line, textAlignment_t align);
void text_pushColors(void);
void text_popColors(void);
void text_setColors(color_t foreground, color_t background);

#endif

