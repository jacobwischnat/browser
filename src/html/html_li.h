#pragma once

#include <SFML/Graphics.hpp>

#include "../types.h"
#include "html_element.h"

class HTMLLI {
    public:
        Context* context;
        std::shared_ptr<HTMLElement> element;

        unsigned int fontSize;

        std::wstring text;
        std::wstring decorator;

        void draw();
        void setText(std::wstring* text);
        void setDecorate(std::wstring* decorator);
        void setFontSize(unsigned int fontSize);
        void setFontFamily(std::string fontFamily);

        HTMLLI(Context*, std::shared_ptr<HTMLElement> element);
};

HTMLLI::HTMLLI(Context* ctx, std::shared_ptr<HTMLElement> element) {
    this->decorator = std::wstring(L"\u2022 ");
    // std::shared_ptr<std::wstring> decorator;
    this->context = ctx;
    this->element = element;
    this->element->setPosition(0, 0);
    this->element->setMargin(20, 20, 0, 0);
    this->element->setSize(this->context->mode->width, 40);

    // printf("%ls\n", decorator->c_str());

    this->setFontSize(40);
};

void HTMLLI::setText(std::wstring* text) {
    this->text = *(text);
}

void HTMLLI::setDecorate(std::wstring* decorator) {
    this->decorator = *(decorator);
}

void HTMLLI::setFontFamily(std::string fontFamily) {
    this->element->style->fontFamily = fontFamily;
}

void HTMLLI::setFontSize(unsigned int fontSize) {
    this->element->style->fontSize = fontSize;
    this->element->style->height = fontSize;
}

void HTMLLI::draw() {
    int parentMarginLeft = 0;
    if (this->element->parent) {
        auto hasParent = this->element->elements()->contains(this->element->parent);

        if (hasParent) {
            auto parentElementPair = this->element->elements()->find(this->element->parent);
            std::shared_ptr<HTMLElement> element(parentElementPair->second);

            parentMarginLeft = element->style->marginLeft;

            // TODO: This will have to change depending on style->display / style->position.
            this->element->style->left = element->style->left;
        }
    }

    if (this->element->previousSibling) {
        auto hasPrevSibling = this->element->elements()->contains(this->element->previousSibling);

        if (hasPrevSibling) {
            auto prevSiblingElementPair = this->element->elements()->find(this->element->previousSibling);
            std::shared_ptr<HTMLElement> element(prevSiblingElementPair->second);

            this->element->style->top = element->style->top + element->style->height;
        }
    }

    printf("parentMarginLeft = %d\n", parentMarginLeft);

    sf::Text text;
    sf::Font font;
    sf::Vector2f pos(
        (this->element->style->left + this->element->style->marginLeft + parentMarginLeft),
        this->element->style->top + this->element->style->marginTop);

    auto wtext = this->decorator + L" " + this->text;
    sf::String sfstring(wtext);
    std::string path = "fonts/" + this->element->style->fontFamily + "-Regular.ttf";
    font.loadFromFile(path);

    text.setCharacterSize(this->element->style->fontSize);
    text.setFillColor(sf::Color::Black);
    text.setString(sfstring);
    text.setPosition(pos);
    text.setFont(font);

    this->context->window->draw(text);
}
