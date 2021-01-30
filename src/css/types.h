#pragma once

enum CSSUnitType {
    CSS_UNIT_TYPE_UNKNOWN,
    CSS_UNIT_TYPE_PX,
    CSS_UNIT_TYPE_EM,
    CSS_UNIT_TYPE_REM
};

class CSSUnit {
    public:
        int value;
        CSSUnitType type;

        static CSSUnitType unitByString(std::string);

        CSSUnit(int, CSSUnitType);
};

CSSUnit::CSSUnit(int value, CSSUnitType type) {
    this->value = value;
    this->type = type;
}

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

enum CSSListStyleType {
    CSS_LIST_STYLE_TYPE_DISC = 1,
    CSS_LIST_STYLE_TYPE_CIRCLE,
    CSS_LIST_STYLE_TYPE_SQUARE,
    CSS_LIST_STYLE_TYPE_DECIMAL
};