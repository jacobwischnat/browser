#pragma once

#include <SFML/Graphics.hpp>

#include "../context.h"
#include "html_element.h"

class HTMLBody : public HTMLElement {
    public:
        void draw();
        HTMLBody(Context*);
};

HTMLBody::HTMLBody(Context* ctx):HTMLElement(ctx) {
    this->setPosition(8, 8);
};

void HTMLBody::draw() {
    sf::Vector2f pos(this->left, this->top);
    sf::Vector2f size(
        this->context->mode->width - (this->left * 2),
        this->context->mode->height - (this->top * 2));

    sf::RectangleShape rect(size);

    rect.setPosition(pos);
    rect.setOutlineThickness(1.0f);
    rect.setOutlineColor(sf::Color(0, 0, 0));
    rect.setFillColor(sf::Color::Transparent);

    this->context->window->draw(rect);
}
