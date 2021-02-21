#pragma once

#include "../types.h"
#include "html_hX.h"

class HTMLH3 : public HTMLHX {
    public:
        HTMLH3(Context* ctx, std::shared_ptr<HTMLElement> element);
};

HTMLH3::HTMLH3(Context* ctx, std::shared_ptr<HTMLElement> element):HTMLHX(68, ctx, element) {

}