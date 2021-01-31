#pragma once

#include <SFML/Graphics.hpp>

#include "../types.h"
#include "html_element.h"

class HTMLA {
    public:
        Context* context;
        std::shared_ptr<HTMLElement> element;

        unsigned int fontSize;

        std::string text;

        void draw();
        void setText(std::string* text);
        void setFontSize(unsigned int fontSize);
        void setFontFamily(std::string fontFamily);

        HTMLA(Context*, std::shared_ptr<HTMLElement> element);
};

HTMLA::HTMLA(Context* ctx, std::shared_ptr<HTMLElement> element) {
    this->context = ctx;
    this->element = element;
    this->element->setPosition(0, 0);
    this->element->setSize(this->context->mode->width, 36);

    this->setFontSize(36);
};

void HTMLA::setText(std::string* text) {
    this->text = *(text);
}

void HTMLA::setFontFamily(std::string fontFamily) {
    this->element->style->fontFamily = fontFamily;
}

void HTMLA::setFontSize(unsigned int fontSize) {
    this->element->style->fontSize = fontSize;
    this->element->style->height = fontSize;
}

void HTMLA::draw() {
    int parentMarginLeft = 0;
    if (this->element->parent) {
        auto hasParent = this->element->elements()->contains(this->element->parent);

        if (hasParent) {
            auto parentElementPair = this->element->elements()->find(this->element->parent);
            std::shared_ptr<HTMLElement> element(parentElementPair->second);

            // TODO: This will have to change depending on style->display / style->position.
            this->element->style->left = element->style->left;
            this->element->style->top = element->style->top + element->style->height;

            parentMarginLeft = element->style->marginLeft;
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

    sf::Text text;
    sf::Font font;
    sf::Vector2f pos(
        this->element->style->left + this->element->style->marginLeft + parentMarginLeft,
        this->element->style->top + (this->element->previousSibling ? this->element->style->marginTop : 0));
    sf::String sfstring(this->text);
    std::string path("fonts/Roboto-Black.ttf");
    font.loadFromFile(path);

    text.setCharacterSize(this->element->style->fontSize);
    text.setFillColor(sf::Color::Blue);
    text.setStyle(sf::Text::Underlined);
    text.setString(sfstring);
    text.setPosition(pos);
    text.setFont(font);

    this->context->window->draw(text);
}
