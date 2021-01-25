#pragma once

#include <SFML/Graphics.hpp>

#include "../context.h"
#include "html_element.h"

class HTMLImage : public HTMLElement {
    public:
        void setImageData(char* data, size_t size);
        void display();
        HTMLImage(Context*);

    private:
        char* data;
        size_t size;
};

HTMLImage::HTMLImage(Context* ctx):HTMLElement(ctx) {

}

void HTMLImage::setImageData(char* data, size_t size) {
    this->data = data;
    this->size = size;
}

void HTMLImage::display() {
    sf::Image image;
    sf::Sprite sprite;
    sf::Texture texture;

    auto loadResult = image.loadFromMemory(this->data, this->size);
    printf("loadResult=%d\n", loadResult);

    texture.loadFromImage(image);
    sprite.setTexture(texture);

    this->context->window->draw(sprite);
    // this->context->window->display();
}