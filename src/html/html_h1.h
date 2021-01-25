#pragma once

#include <SFML/Graphics.hpp>

#include "../context.h"
#include "html_element.h"

class HTMLH1 : public HTMLElement {
    public:
        std::string text;
        std::string fontFamily;

        void draw();
        void setText(std::string* text);
        void setFontFamily(std::string* fontFamily);

        HTMLH1(Context*);
};

HTMLH1::HTMLH1(Context* ctx):HTMLElement(ctx) {
    this->setPosition(0, 0);
    this->fontFamily = std::string("Roboto-Light");
};

void HTMLH1::setText(std::string* text) {
    this->text = *(text);
}

void HTMLH1::setFontFamily(std::string* fontFamily) {
    this->fontFamily = *(fontFamily);
}

void HTMLH1::draw() {
    sf::Text text;
    sf::Font font;
    sf::Vector2f pos(10, 10);
    sf::String sfstring(this->text);
    std::string path = "fonts/" + this->fontFamily + ".ttf";

    font.loadFromFile(path);

    text.setFont(font);
    text.setCharacterSize(48);
    text.setFillColor(sf::Color::Black);
    text.setString(sfstring);
    text.setPosition(pos);

    this->context->window->draw(text);
}
