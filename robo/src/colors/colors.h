#ifndef COLORS_H
#define COLORS_H

#include <stdint.h>

typedef uint16_t color_t;

#define BLACK (0x0000)
#define WHITE (0xFFFF)
#define BLUE  (0xF800)
#define RED   (0x001F)

color_t color12(uint32_t rgb);
color_t color16(uint32_t rgb);

#endif
