#pragma once

#include <gumbo.h>

class HTMLElement {
    private:
        GumboNode* node;

    public:
        HTMLElement* parent;
        HTMLElement(GumboNode* node);
        inline void setParent(HTMLElement* parent) {
            this->parent = parent;
        };

        static inline HTMLElement* getElementFromKey(ELEMENT_KEY key) {
            return
        }
};

HTMLElement::HTMLElement(GumboNode* node) {
    this->node = node;
}