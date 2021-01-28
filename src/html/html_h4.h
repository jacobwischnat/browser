#pragma once

#include "../types.h"
#include "html_hX.h"

class HTMLH4 : public HTMLHX {
    public:
        HTMLH4(Context* ctx, std::shared_ptr<HTMLElement> element);
};

HTMLH4::HTMLH4(Context* ctx, std::shared_ptr<HTMLElement> element):HTMLHX(52, ctx, element) {

}