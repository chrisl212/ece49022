#include "wu.h"
#include "ui/ili9341/ili9341.h"

inline static void _plot(uint16_t x, uint16_t y, float c, color_t color) {
    ili9341_writePixelAtPoint(x, y, color);
}

inline static uint16_t _ipart(float x) {
    return (uint16_t)x;
}

inline static uint16_t _round(float x) {
    return _ipart(x+0.5);
}

inline static float _fpart(float x) {
    return x - _ipart(x);
}

inline static float _rfpart(float x) {
    return 1-_fpart(x);
}

inline static float _abs(float x) {
    return (x > 0) ? x : -x;
}

void wu(int16_t x0, int16_t x1, int16_t y0, int16_t y1, color_t color) {
    int16_t steep;
    int16_t temp, xpxl1, ypxl1, xpxl2, ypxl2, x;
    float grad, xend, yend, xgap, intery, dx, dy;
    dx = x1 - x0;
    dy = y1 - y0;
    steep = _abs(dy) > _abs(dx);

    if (steep) {
        temp = x0;
        x0 = y0;
        y0 = temp;
        temp = x1;
        x1 = y1;
        y1 = temp;
    }
    if (x0 > x1) {
        temp = x0;
        x0 = x1;
        x1 = temp;
        temp = y0;
        y0 = y1;
        y1 = temp;
    }
    dx = x1 - x0;
    dy = y1 - y0;
    grad = dy/dx;
    if (!dx) grad = 1.0;

    xend = _round(x0);
    yend = y0 + grad*(xend-x0);
    xgap = _rfpart(x0+0.5);
    xpxl1 = xend;
    ypxl1 = _ipart(yend);
    if (steep) {
        _plot(ypxl1, xpxl1, _rfpart(yend) * xgap, color);
        _plot(ypxl1+1, xpxl1, _fpart(yend) * xgap, color);
    } else {
        _plot(xpxl1, ypxl1, _rfpart(yend) * xgap, color);
        _plot(xpxl1, ypxl1+1, _fpart(yend) * xgap, color);
    }
    intery = yend + grad;

    xend = _round(x1);
    yend = y1 + grad * (xend - x1);
    xgap = _fpart(x1 + 0.5);
    xpxl2 = xend;
    ypxl2 = _ipart(yend);
    if (steep) {
        _plot(ypxl2, xpxl2, _rfpart(yend) * xgap, color);
        _plot(ypxl2+1, xpxl2, _fpart(yend) * xgap, color);
    } else {
        _plot(xpxl2, ypxl2, _rfpart(yend) * xgap, color);
        _plot(xpxl2, ypxl2+1, _fpart(yend) * xgap, color);
    }

    if (steep) {
        for (x = xpxl1 + 1; x <= xpxl2-1; x++) {
            _plot(_ipart(intery), x, _rfpart(intery), color);
            _plot(_ipart(intery)+1, x, _fpart(intery), color);
            intery += grad;
        }
    } else {
        for (x = xpxl1 + 1; x <= xpxl2-1; x++) {
            _plot(x, _ipart(intery), _rfpart(intery), color);
            _plot(x, _ipart(intery)+1, _fpart(intery), color);
            intery += grad;
        }
    }
}
