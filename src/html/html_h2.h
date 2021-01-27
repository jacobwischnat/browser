#pragma once

#include "../types.h"
#include "html_hX.h"

class HTMLH2 : public HTMLHX {
    public:
        HTMLH2(Context* ctx,HTMLElement* element);
};

HTMLH2::HTMLH2(Context* ctx,HTMLElement* element):HTMLHX(44, ctx, element) {

}