#pragma once

#include <SFML/Graphics.hpp>

#include "../types.h"
// #include "../style/type"
#include "html_element.h"

class HTMLHX {
    public:
        Context* context;
        HTMLElement* element;

        unsigned int fontSize;

        std::string text;

        void draw();
        void setText(std::string* text);
        void setFontSize(unsigned int fontSize);
        void setFontFamily(std::string fontFamily);

        HTMLHX(unsigned int fontSize, Context*, HTMLElement*);
};

HTMLHX::HTMLHX(unsigned int fontSize, Context* ctx, HTMLElement* element) {
    this->context = ctx;
    this->element = element;
    this->element->setPosition(0, 0);
    this->element->setSize(this->context->mode->width, fontSize);

    this->setFontSize(fontSize);

    printf(
        "t: %u, l: %u, w: %u x h: %u\n",
        this->element->style->top,
        this->element->style->left,
        this->element->style->width,
        this->element->style->height);
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
    if (this->element->parent) {
        auto hasParent = this->context->elements->contains(this->element->parent);

        if (hasParent) {
            auto parentElementPair = this->context->elements->find(this->element->parent);
            auto element = (HTMLElement*) parentElementPair->second;

            printf("parent->style->left = %u\n", element->style->left);

            // TODO: This will have to change depending on style->display / style->position.
            this->element->style->left = element->style->left;
            this->element->style->top = element->style->top + element->style->height;
        }
    }

    printf("this->element->parent = %lu\n", this->element->parent);
    printf("this->element->previousSibling = %lu\n", this->element->previousSibling);

    printf("this->element->style->left = %u\n", this->element->style->left);

    if (this->element->previousSibling) {
        auto hasPrevSibling = this->context->elements->contains(this->element->previousSibling);

        if (hasPrevSibling) {
            auto prevSiblingElementPair = this->context->elements->find(this->element->previousSibling);
            auto element = (HTMLElement*) prevSiblingElementPair->second;

            // printf("element->style->top = %u\n", element->style->top);
            // printf("element->style->height = %u\n", element->style->height);

            this->element->style->top += element->style->top + element->style->height;

            printf("this->element->style->top = %u\n", this->element->style->top);
            printf("this->element->style->height = %u\n", this->element->style->height);
        }
    }

    sf::Text text;
    sf::Font font;
    sf::Vector2f pos(this->element->style->left, this->element->style->top);
    sf::String sfstring(this->text);
    std::string path = "fonts/" + this->element->style->fontFamily + ".ttf";
    font.loadFromFile(path);

    text.setCharacterSize(this->element->style->fontSize);
    text.setFillColor(sf::Color::Black);
    text.setStyle(sf::Text::Bold);
    text.setString(sfstring);
    text.setPosition(pos);
    text.setFont(font);

    this->context->window->draw(text);
}
