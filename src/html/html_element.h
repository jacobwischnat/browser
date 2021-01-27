#pragma once

#include <map>
#include <string>
#include <uuid/uuid.h>
#include <SFML/Graphics.hpp>

#include "../css/css.h"
#include "../types.h"

class HTMLElement {
    public:
        ELEMENT_KEY id;
        ELEMENT_KEY parent;
        ELEMENT_KEY previousSibling;

        CSS* style;
        GumboTag type;
        Context* context;

        std::vector<HTMLElement*> children;
        std::map<std::string, std::string> attributes;

        void *extended;

        void appendElement(HTMLElement* element);
        void setParent(ELEMENT_KEY parent);
        void setPreviousSibling(ELEMENT_KEY previousSibling);

        void setTop(int top);
        void setLeft(int left);
        void setWidth(int width);
        void setHeight(int height);
        void setPosition(int top, int left);
        void setSize(int width, int height);

        HTMLElement(Context*);
};

HTMLElement::HTMLElement(Context* ctx) {
    std::string fontFamily("Roboto-Light");
    CSS* style = (CSS*) calloc(1, sizeof(CSS));
    style->display = CSS_DISPLAY_BLOCK;
    style->top = 0;
    style->left = 0;
    style->width = 0;
    style->height = 0;
    style->fontFamily = fontFamily;

    this->style = style;
    this->context = ctx;

    this->id = (*this->context->elements).size() + 1;
    this->parent = 0;
    this->previousSibling = 0;
}

void HTMLElement::setParent(ELEMENT_KEY parent) {
    printf("this->setParent = %lu\n", parent);
    this->parent = parent;
}

void HTMLElement::setPreviousSibling(ELEMENT_KEY previousSibling) {
    this->previousSibling = previousSibling;
}

void HTMLElement::appendElement(HTMLElement* element) {
    this->children.push_back(element);
}

void HTMLElement::setSize(int width, int height) {
    this->style->width = width;
    this->style->height = height;
}

void HTMLElement::setWidth(int width) {
    this->style->width = width;
}

void HTMLElement::setHeight(int height) {
    this->style->height = height;
}

void HTMLElement::setTop(int top) {
    this->style->top = top;
}

void HTMLElement::setLeft(int left) {
    this->style->left = left;
}

void HTMLElement::setPosition(int top, int left) {
    this->style->top = top;
    this->style->left = left;
}