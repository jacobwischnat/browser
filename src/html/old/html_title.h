#pragma once

#include <SFML/Graphics.hpp>

#include "../types.h"
#include "html_element.h"

class HTMLTitle : public HTMLElement {
    public:
        std::string text;
        void setText(std::string* text);
        HTMLTitle(Context*);
};

HTMLTitle::HTMLTitle(Context* ctx):HTMLElement(ctx) {

};

void HTMLTitle::setText(std::string* text) {
    this->text = *(text);
    this->context->window->setTitle(this->text);
};
