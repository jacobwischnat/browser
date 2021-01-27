#pragma once

typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
} Color;

enum CSSDisplay {
    CSS_DISPLAY_BLOCK = 1,
    CSS_DISPLAY_INLINE
};