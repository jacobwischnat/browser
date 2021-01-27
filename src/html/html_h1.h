#pragma once

#include "../types.h"
#include "html_hX.h"

class HTMLH1 : public HTMLHX {
    public:
        HTMLH1(Context* ctx,HTMLElement* element);
};

HTMLH1::HTMLH1(Context* ctx,HTMLElement* element):HTMLHX(52, ctx, element) {

}