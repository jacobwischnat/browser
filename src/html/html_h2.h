#pragma once

#include "../types.h"
#include "html_hX.h"

class HTMLH2 : public HTMLHX {
    public:
        HTMLH2(Context* ctx, std::shared_ptr<HTMLElement> element);
};

HTMLH2::HTMLH2(Context* ctx, std::shared_ptr<HTMLElement> element):HTMLHX(84, ctx, element) {

}