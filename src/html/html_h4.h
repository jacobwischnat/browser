#pragma once

#include "../types.h"
#include "html_hX.h"

class HTMLH4 : public HTMLHX {
    public:
        HTMLH4(Context* ctx,HTMLElement* element);
};

HTMLH4::HTMLH4(Context* ctx,HTMLElement* element):HTMLHX(30, ctx, element) {

}