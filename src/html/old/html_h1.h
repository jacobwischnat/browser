#pragma once

#include "../types.h"
#include "html_hX.h"

class HTMLH1 : public HTMLHX {
    public:
        HTMLH1(Context* ctx, std::shared_ptr<HTMLElement> element);
};

HTMLH1::HTMLH1(Context* ctx, std::shared_ptr<HTMLElement> element):HTMLHX(100, ctx, element) {

}