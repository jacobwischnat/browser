#pragma once

#include <SFML/Graphics.hpp>

#include "../types.h"
#include "html_element.h"

class HTMLBody {
    public:
        Context* context;
        HTMLElement* element;

        void draw();
        HTMLBody(Context*,HTMLElement*);
};

HTMLBody::HTMLBody(Context* ctx, HTMLElement* element) {
    this->context = ctx;
    this->element = element;

    this->element->setPosition(8, 8);
};

void HTMLBody::draw() {
    sf::Vector2f pos(this->element->style->left, this->element->style->top);
    sf::Vector2f size(
        this->context->mode->width - (this->element->style->left * 2),
        this->context->mode->height - (this->element->style->top * 2));

    sf::RectangleShape rect(size);

    rect.setPosition(pos);
    rect.setOutlineThickness(1.0f);
    rect.setOutlineColor(sf::Color(0, 0, 0));
    rect.setFillColor(sf::Color::Transparent);

    this->context->window->draw(rect);
}
