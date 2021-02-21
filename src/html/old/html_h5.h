#pragma once

#include "../types.h"
#include "html_hX.h"

class HTMLH5 : public HTMLHX {
    public:
        HTMLH5(Context* ctx, std::shared_ptr<HTMLElement> element);
};

HTMLH5::HTMLH5(Context* ctx, std::shared_ptr<HTMLElement> element):HTMLHX(36, ctx, element) {

}