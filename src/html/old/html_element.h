#pragma once

#include <map>
#include <string>
#include <gumbo.h>
#include <uuid/uuid.h>
#include <SFML/Graphics.hpp>

#include "../css/css.h"
#include "../types.h"

typedef unsigned long ELEMENT_KEY;

class HTMLElement {
    public:
        ELEMENT_KEY id;
        ELEMENT_KEY parent;
        ELEMENT_KEY previousSibling;

        GumboTag type;
        Context* context;

        std::vector<ELEMENT_KEY> children;

        std::shared_ptr<CSS> style;
        std::shared_ptr<GumboVector> attributes;

        void *extended;

        void appendElement(ELEMENT_KEY element);
        void setParent(ELEMENT_KEY parent);
        void setPreviousSibling(ELEMENT_KEY previousSibling);
        void setAttributes(std::shared_ptr<GumboVector>);

        void setTop(int top);
        void setLeft(int left);
        void setType(GumboTag tag);
        void setSize(int width, int height);
        void setWidth(int width);
        void setHeight(int height);
        void setMargin(int top, int bottom, int left, int right);
        void setPosition(int top, int left);

        void display();

        std::map<ELEMENT_KEY,std::shared_ptr<HTMLElement>>* elements();

        GumboAttribute* findAttribute(std::shared_ptr<std::string>);

        HTMLElement(Context*);
};

HTMLElement::HTMLElement(Context* ctx) {
    std::string fontFamily("OpenSans");

    auto style = std::make_shared<CSS>();

    style->display = CSS_DISPLAY_BLOCK;
    style->fontFamily = fontFamily;

    style->top = 0;
    style->left = 0;
    style->width = 0;
    style->height = 0;
    style->marginTop = 0;
    style->marginLeft = 0;
    style->marginRight = 0;
    style->marginBottom = 0;

    this->style = style;
    this->context = ctx;

    auto elements = (std::map<ELEMENT_KEY,std::shared_ptr<HTMLElement>>*) this->context->elements;

    this->parent = 0;
    this->previousSibling = 0;
    this->id = elements->size() + 1;
}

std::map<ELEMENT_KEY,std::shared_ptr<HTMLElement>>* HTMLElement::elements() {
    return (std::map<ELEMENT_KEY,std::shared_ptr<HTMLElement>>*) this->context->elements;
}


void HTMLElement::setAttributes(std::shared_ptr<GumboVector> attributes) {
    this->attributes = attributes;
}

void HTMLElement::setType(GumboTag type) {
    this->type = type;
}

void HTMLElement::setParent(ELEMENT_KEY parent) {
    this->parent = parent;
}

void HTMLElement::setPreviousSibling(ELEMENT_KEY previousSibling) {
    this->previousSibling = previousSibling;
}

void HTMLElement::appendElement(ELEMENT_KEY element) {
    this->children.push_back(element);

    auto hasChild = this->elements()->contains(element);
    if (hasChild) {
        auto childElementPair = this->elements()->find(element);
        std::shared_ptr<HTMLElement> element(childElementPair->second);

        this->style->height += element->style->top + element->style->height;
    }
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

void HTMLElement::setMargin(int top, int bottom, int left, int right) {
    this->style->marginTop = top;
    this->style->marginLeft = left;
    this->style->marginRight = right;
    this->style->marginBottom = bottom;
}

void HTMLElement::display() {
    if (this->parent) {
        auto hasParent = this->elements()->contains(this->parent);
        if (hasParent) {
            auto parentElementPair = this->elements()->find(this->parent);
            std::shared_ptr<HTMLElement> element(parentElementPair->second);

            // TODO: This will have to change depending on style->display / style->position.
            this->style->left = element->style->left;
            this->style->top = element->style->top; // + element->style->height;
        }
    }

    if (this->previousSibling) {
        auto hasPrevSibling = this->elements()->contains(this->previousSibling);
        if (hasPrevSibling) {
            auto prevSiblingElementPair = this->elements()->find(this->previousSibling);
            std::shared_ptr<HTMLElement> element(prevSiblingElementPair->second);

            this->style->top = element->style->top + element->style->height;
        }
    }
}

GumboAttribute* HTMLElement::findAttribute(std::shared_ptr<std::string> name) {
    for (int i = 0; i < this->attributes->length; i += 1) {
        GumboAttribute* attribute = (GumboAttribute*) attributes->data[i];

        if (strcmp(attribute->name, name->c_str()) == 0) {
            return attribute;
        }
    }

    return NULL;
}