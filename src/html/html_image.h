#pragma once

#include <SFML/Graphics.hpp>

#include "../types.h"
#include "html_element.h"

class HTMLImage {
    public:
        Context* context;
        std::shared_ptr<HTMLElement> element;

        void setImageData(char* data, size_t size);
        void display();
        HTMLImage(Context*, std::shared_ptr<HTMLElement> element);

    private:
        char* data;
        size_t size;
};

HTMLImage::HTMLImage(Context* ctx, std::shared_ptr<HTMLElement> element) {
    this->context = ctx;
    this->element = element;
}

void HTMLImage::setImageData(char* data, size_t size) {
    this->data = data;
    this->size = size;
}

void HTMLImage::display() {
    auto tagName = std::make_shared<std::string>("style");
    auto style = this->element->findAttribute(tagName);

    if (style) this->element->style->apply(std::string(style->value));

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

    sf::Image image;
    sf::Sprite sprite;
    sf::Texture texture;

    auto loadResult = image.loadFromMemory(this->data, this->size);
    auto size = image.getSize();

    auto rect = sf::IntRect(0, 0, size.x, size.y);
    texture.loadFromImage(image, rect);

    sprite.setTexture(texture, true);
    sprite.setPosition(
        this->element->style->left + parentMarginLeft,
        this->element->style->top);
    sprite.setTextureRect(rect);

    this->context->window->draw(sprite);
}