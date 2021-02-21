#pragma once

#include "../types.h"
#include "html_hX.h"

class HTMLH6 : public HTMLHX {
    public:
        HTMLH6(Context* ctx, std::shared_ptr<HTMLElement> element);
};

HTMLH6::HTMLH6(Context* ctx, std::shared_ptr<HTMLElement> element):HTMLHX(28, ctx, element) {

}