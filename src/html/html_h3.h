#pragma once

#include "../types.h"
#include "html_hX.h"

class HTMLH3 : public HTMLHX {
    public:
        HTMLH3(Context* ctx,HTMLElement* element);
};

HTMLH3::HTMLH3(Context* ctx,HTMLElement* element):HTMLHX(36, ctx, element) {

}