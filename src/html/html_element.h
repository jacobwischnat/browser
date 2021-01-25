#pragma once

#include <map>
#include <string>
#include <SFML/Graphics.hpp>

#include "../css/css.h"

class HTMLElement {
    public:
        Context* context;

        int top;
        int left;
        int width;
        int height;

        CSS style;

        std::vector<HTMLElement*> children;
        std::map<std::string, std::string> attributes;

        HTMLElement* parent;

        void appendElement(HTMLElement* element);
        void setParent(HTMLElement* parent);
        void setSize(int width, int height);
        void setPosition(int top, int left);

        HTMLElement(Context*);
};

HTMLElement::HTMLElement(Context* ctx) {
    this->context = ctx;
}

void HTMLElement::setParent(HTMLElement* parent) {
    this->parent = parent;
}

void HTMLElement::setSize(int width, int height) {
    this->width = width;
    this->height = height;
}

void HTMLElement::setPosition(int top, int left) {
    this->top = top;
    this->left = left;
}

void HTMLElement::appendElement(HTMLElement* element) {
    this->children.push_back(element);
}