#pragma once

#include <SFML/Graphics.hpp>

#include "../types.h"
// #include "../style/type"
#include "html_element.h"

class HTMLHR {
    public:
        Context* context;
        std::shared_ptr<HTMLElement> element;

        void draw();

        HTMLHR(Context*, std::shared_ptr<HTMLElement> element);
};

HTMLHR::HTMLHR(Context* ctx, std::shared_ptr<HTMLElement> element) {
    this->context = ctx;
    this->element = element;
    this->element->setPosition(0, 0);
    this->element->setMargin(16, 16, 0, 0);
    this->element->setSize(this->context->mode->width, 4);
};

void HTMLHR::draw() {
    if (this->element->parent) {
        auto hasParent = this->element->elements()->contains(this->element->parent);

        if (hasParent) {
            auto parentElementPair = this->element->elements()->find(this->element->parent);
            std::shared_ptr<HTMLElement> element(parentElementPair->second);

            // TODO: This will have to change depending on style->display / style->position.
            this->element->style->left = element->style->left + element->style->marginLeft;
            this->element->style->top = element->style->top + element->style->height;
            this->element->style->width = element->style->width;
        }
    }

    if (this->element->previousSibling) {
        auto hasPrevSibling = this->element->elements()->contains(this->element->previousSibling);

        if (hasPrevSibling) {
            auto prevSiblingElementPair = this->element->elements()->find(this->element->previousSibling);
            std::shared_ptr<HTMLElement> element(prevSiblingElementPair->second);

            this->element->style->top += element->style->top + element->style->height;
        }
    }

    sf::Vector2f size(
        (float) (this->element->style->width - this->element->style->marginRight),
        (float) this->element->style->height);

    sf::RectangleShape line;
    line.setPosition(
        (float) this->element->style->left + this->element->style->marginLeft,
        (float) this->element->style->top + this->element->style->marginTop);
    line.setSize(size);
    line.setFillColor(sf::Color::Red);
    line.setOutlineColor(sf::Color::Red);

    this->context->window->draw(line);
}
