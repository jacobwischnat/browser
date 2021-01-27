#pragma once

#include "types.h"

class CSS {
    public:
        int top;
        int left;
        unsigned int width;
        unsigned int height;
        unsigned int fontSize;

        std::string fontFamily;

        Color* color;
        Color* backgroundColor;

        CSSDisplay display;
};