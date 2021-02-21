#pragma once

#include <SFML/Graphics.hpp>

#include "../types.h"
#include "html_element.h"

class HTMLUL {
    public:
        Context* context;
        std::shared_ptr<HTMLElement> element;

        void draw();

        HTMLUL(Context*, std::shared_ptr<HTMLElement> element);
};

HTMLUL::HTMLUL(Context* ctx, std::shared_ptr<HTMLElement> element) {
    this->context = ctx;
    this->element = element;
    this->element->setPosition(0, 0);
    this->element->setMargin(0, 0, 16, 0);
};

void HTMLUL::draw() {
    if (this->element->parent) {
        auto hasParent = this->element->elements()->contains(this->element->parent);

        if (hasParent) {
            auto parentElementPair = this->element->elements()->find(this->element->parent);
            std::shared_ptr<HTMLElement> element(parentElementPair->second);

            // TODO: This will have to change depending on style->display / style->position.
            this->element->style->left = element->style->left + element->style->marginLeft;
            this->element->style->top = element->style->top + element->style->height;
            this->element->style->width = element->style->width;

            printf("this->parent->element->style->width = %d\n", element->style->width);
        } else printf("no parent\n");
    } else printf("no parent\n");

    if (this->element->previousSibling) {
        auto hasPrevSibling = this->element->elements()->contains(this->element->previousSibling);

        if (hasPrevSibling) {
            auto prevSiblingElementPair = this->element->elements()->find(this->element->previousSibling);
            std::shared_ptr<HTMLElement> element(prevSiblingElementPair->second);

            this->element->style->top += element->style->top + element->style->height;
        }
    }

    this->element->style->left += this->element->style->marginLeft;
}
