#pragma once

#include <SFML/Graphics.hpp>

#include "../types.h"
#include "html_element.h"

class HTMLImage {
    public:
        Context* context;
        HTMLElement* element;

        void setImageData(char* data, size_t size);
        void display();
        HTMLImage(Context*, HTMLElement*);

    private:
        char* data;
        size_t size;
};

HTMLImage::HTMLImage(Context* ctx, HTMLElement* element) {
    this->context = ctx;
    this->element = element;
}

void HTMLImage::setImageData(char* data, size_t size) {
    this->data = data;
    this->size = size;
}

void HTMLImage::display() {
    if (this->element->parent) {
        auto hasParent = this->context->elements->contains(this->element->parent);

        if (hasParent) {
            auto parentElementPair = this->context->elements->find(this->element->parent);
            auto element = (HTMLElement*) parentElementPair->second;

            // TODO: This will have to change depending on style->display / style->position.
            this->element->style->left = element->style->left;
            this->element->style->top = element->style->top + element->style->height;
        }
    }

    printf("this->element->parent = %lu\n", this->element->parent);
    printf("this->element->previousSibling = %lu\n", this->element->previousSibling);

    if (this->element->previousSibling) {
        auto hasPrevSibling = this->context->elements->contains(this->element->previousSibling);

        if (hasPrevSibling) {
            auto prevSiblingElementPair = this->context->elements->find(this->element->previousSibling);
            auto element = (HTMLElement*) prevSiblingElementPair->second;

            printf("element->style->top = %u\n", element->style->top);
            printf("element->style->height = %u\n", element->style->height);

            this->element->style->top += element->style->top + element->style->height;

            printf("this->element->style->top = %u\n", this->element->style->top);
            printf("this->element->style->height = %u\n", this->element->style->height);
        }
    }

    sf::Image image;
    sf::Sprite sprite;
    sf::Texture texture;

    auto loadResult = image.loadFromMemory(this->data, this->size);

    texture.loadFromImage(image);
    sprite.setTexture(texture);
    sprite.setPosition(this->element->style->left, this->element->style->top);

    this->context->window->draw(sprite);
}