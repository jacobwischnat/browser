#pragma once

#include "../types.h"
#include "html_hX.h"

class HTMLH6 : public HTMLHX {
    public:
        HTMLH6(Context* ctx,HTMLElement* element);
};

HTMLH6::HTMLH6(Context* ctx,HTMLElement* element):HTMLHX(16, ctx, element) {

}