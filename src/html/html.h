#pragma once

#include <gumbo.h>

#include "../network.h"

#include "html_element.h"

class HTMLParser {
    public:
        void parse(std::string document);
        void handleNode(GumboNode* node);

        HTMLParser(std::shared_ptr<Network>);

    private:
        std::shared_ptr<Network> network;
};

HTMLParser::HTMLParser(std::shared_ptr<Network> network) {
    this->network = network;
}

void HTMLParser::handleNode(GumboNode* node) {
    switch (node->type) {
        case GUMBO_NODE_ELEMENT:
            HTMLElement element(node);
            break;
    }
}

void HTMLParser::parse(std::string document) {
    GumboOutput* output = gumbo_parse(document.c_str());

    this->handleNode(output->root);
}