#include "colors.h"

color_t color12(uint32_t rgb) {
    uint16_t r = (rgb >> 16) & 0xFF;
    uint16_t g = (rgb >> 8) & 0xFF;
    uint16_t b = (rgb >> 0) & 0xFF;
    r = (r * 0xF) / 255;
    g = (g * 0xF) / 255;
    b = (b * 0xF) / 255;
    return (r << 8) | (b << 4) | b;
}

color_t color16(uint32_t rgb) {
    uint16_t r = (rgb >> 16) & 0xFF;
    uint16_t g = (rgb >> 8) & 0xFF;
    uint16_t b = (rgb >> 0) & 0xFF;
    uint16_t ro, go, bo;
    ro = (r * 0x1F) / 255;
    go = (g * 0x3F) / 255;
    bo = (b * 0x1F) / 255;
    return (ro << 11) | (go << 5) | bo;
}
