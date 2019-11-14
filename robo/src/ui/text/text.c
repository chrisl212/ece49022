#include <stdint.h>
#include <stdarg.h>
#include "std/std.h"
#include "math/math.h"
#include "text.h"
#include "ui/colors/colors.h"
#include "font.h"
#include "ui/ili9341/ili9341.h"

uint16_t foregroundColor = WHITE;
uint16_t backgroundColor = RED;
uint16_t fg, bg;

void text_writeCharacterAtPoint(fontHeader_t font, char c, uint16_t x0, uint16_t y0) {
    fontCharacter_t ch = font.fc[c-font.start_char];
    const uint16_t *bitmap_data = &font.fd[ch.data_index];
    uint32_t line_data, mask;
    int x, y;

    for (y = 0; y < font.cell_max_height; y++) {
        ili9341_setDisplayWindow(x0, y0 + y, WIDTH-1, HEIGHT-1);
        if (y < ch.top_rows_blank  || (ch.top_rows_blank + ch.rows_high) <= y) {
            for (x = 0; x < font.cell_max_width; x++) {
                ili9341_writePixel(backgroundColor);
            }
        } else {
            for (x = 0; x < ch.left_cols_blank; x++) {
                ili9341_writePixel(backgroundColor);
            }
            line_data = *bitmap_data++;
            mask = 0x80;
            if (ch.cols_wide > 8) {
                line_data <<= 8;
                line_data |= *bitmap_data++;
                mask = 0x8000;
            }
            for (x = ch.left_cols_blank; x < (ch.left_cols_blank + ch.cols_wide); x++, mask >>= 1) {
                if (line_data & mask) {
                    ili9341_writePixel(foregroundColor);
                } else {
                    ili9341_writePixel(backgroundColor);
                }
            }
            for (x = ch.left_cols_blank + ch.cols_wide; x < font.cell_max_width; x++) {
                ili9341_writePixel(backgroundColor);
            }
        }
    }
}

void text_clearLine(fontHeader_t font, uint16_t x0, uint16_t y0) {
    int x,y;
    for (y = 0; y < font.cell_max_height; y++) {
        ili9341_setDisplayWindow(x0, y0 + y, WIDTH - 1, HEIGHT - 1);
        for (x = x0; x < WIDTH - 1; x++) {
            ili9341_writePixel(backgroundColor);
        }
    }
}

void text_writeTextAtPoint(fontHeader_t font, char *str, uint16_t x, uint16_t y, textAlignment_t align) {
    text_writeFormatAtPoint(font, x, y, align, str);
}

static void _writeChar(fontHeader_t font, char c, uint16_t *x, uint16_t *y, uint16_t x0) {
    if (c == '\n') {
        *y += font.cell_max_height + 2;
        *x = x0;
        text_clearLine(font, *x, *y);
        return;
    }

    if (*x > WIDTH - 1 - font.cell_max_width) {
        *y += font.cell_max_height + 2;
        *x = x0;
        text_clearLine(font, *x, *y);
    }
    text_writeCharacterAtPoint(font, c, *x, *y);
    *x += font.cell_max_width;
}

static int _writeInt(fontHeader_t font, int i, int b, uint16_t *x, uint16_t *y, uint16_t x0) {
    int digs, tmp, div, j, quo;
   
    j = 0;
    if (i < 0) {
        _writeChar(font, '-', x, y, x0);
        i = -i;
        j++;
    } 
    digs = (i == 0);
    tmp = i;

    while (tmp) {
        tmp /= b;
        digs++;
    }

    tmp = i;
    while (digs) {
        div = pow(b, --digs);
        quo = tmp/div;
        _writeChar(font, ((quo > 9) ? 'A' + (quo-10) : '0' + quo), x, y, x0);
        tmp %= div;
        j++;
    }
    return j;
}

void text_writeFormatAtPoint(fontHeader_t font, uint16_t x, uint16_t y, textAlignment_t align, char *fmt, ...) {
    va_list lst;
    int i = 0;
    char *s;
    uint16_t x0;

    va_start(lst, fmt);

    switch (align) {
        case LEFT:
                x0 = x;
                break;
        case CENTER:
                x0 = x + ((WIDTH - 1) - strlen(fmt)*font.cell_max_width)/2;
                x = x0;
                break;
        case RIGHT:
                x0 = x + ((WIDTH - 1) - strlen(fmt)*font.cell_max_width);
                x = x0;
                break;
        default:
                x0 = x;
                break;
    }

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 'd':
                    i += _writeInt(font, va_arg(lst, int), 10, &x, &y, x0);
                    break;
                case 'x':
                    i += _writeInt(font, va_arg(lst, int), 16, &x, &y, x0);
                    break;
                case 's':
                    s = va_arg(lst, char *);
                    while (*s) {
                        _writeChar(font, *s++, &x, &y, x0);
                        i++;
                    }
                    break;
                default:
                    _writeChar(font, *fmt, &x, &y, x0);
            }
        } else {
            _writeChar(font, *fmt, &x, &y, x0);
            i++;
        }
        fmt++;
    }    
    va_end(lst);    
}

void text_writeTextOnLine(fontHeader_t font, char *str, uint16_t line, textAlignment_t align) {
    text_writeTextAtPoint(font, str, 0, font.cell_max_height*line, align);
}

void text_setColors(color_t foreground, color_t background) {
    foregroundColor = foreground;
    backgroundColor = background;
}

void text_pushColors(void) {
    fg = foregroundColor;
    bg = backgroundColor;
}

void text_popColors(void) {
    foregroundColor = fg;
    backgroundColor = bg;
}
