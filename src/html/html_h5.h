#pragma once

#include "../types.h"
#include "html_hX.h"

class HTMLH5 : public HTMLHX {
    public:
        HTMLH5(Context* ctx,HTMLElement* element);
};

HTMLH5::HTMLH5(Context* ctx,HTMLElement* element):HTMLHX(24, ctx, element) {

}