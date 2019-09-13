#ifndef FONT_H
#define FONT_H

#include <stdint.h>

typedef struct {
    uint16_t cols_wide;
    uint16_t rows_high;
    uint16_t left_cols_blank;
    uint16_t top_rows_blank;
    uint16_t data_index;
} fontCharacter_t;

typedef struct {
    uint16_t start_char;
    uint16_t number_of_chars;
    uint16_t cell_max_width;
    uint16_t cell_max_height;
    uint16_t prop_space;
    const fontCharacter_t *fc;
    const uint16_t *fd;
} fontHeader_t;

extern const fontHeader_t f_08x08;
extern const fontHeader_t f_12x16;

#endif
