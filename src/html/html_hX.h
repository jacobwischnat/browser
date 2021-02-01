#pragma once

#include <SFML/Graphics.hpp>

#include "../types.h"
// #include "../style/type"
#include "html_element.h"

class HTMLHX {
    public:
        Context* context;
        std::shared_ptr<HTMLElement> element;

        unsigned int fontSize;

        std::string text;

        void draw();
        void setText(std::string* text);
        void setFontSize(unsigned int fontSize);
        void setFontFamily(std::string fontFamily);

        HTMLHX(unsigned int fontSize, Context*, std::shared_ptr<HTMLElement> element);
};

HTMLHX::HTMLHX(unsigned int fontSize, Context* ctx, std::shared_ptr<HTMLElement> element) {
    this->context = ctx;
    this->element = element;
    this->element->setPosition(0, 0);
    this->element->setMargin(20, 20, 0, 0);
    this->element->setSize(this->context->mode->width, fontSize);

    this->setFontSize(fontSize);
};

void HTMLHX::setText(std::string* text) {
    this->text = *(text);
}

void HTMLHX::setFontFamily(std::string fontFamily) {
    this->element->style->fontFamily = fontFamily;
}

void HTMLHX::setFontSize(unsigned int fontSize) {
    this->element->style->fontSize = fontSize;
    this->element->style->height = fontSize;
}

void HTMLHX::draw() {
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
    std::string path = "" + this->element->style->fontFamily + "-Black.ttf";
    font.loadFromFile(path);

    text.setCharacterSize(this->element->style->fontSize);
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Bold);
    text.setString(sfstring);
    text.setPosition(pos);
    text.setFont(font);

    this->context->window->draw(text);
}
