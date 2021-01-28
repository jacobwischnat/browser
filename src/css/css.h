#pragma once

#include <vector>

#include "types.h"
#include "../helpers.h"

class CSS {
    public:
        int top;
        int left;

        unsigned int width;
        unsigned int height;
        unsigned int fontSize;

        int marginTop;
        int marginLeft;
        int marginRight;
        int marginBottom;

        std::string fontFamily;

        Color* color;
        Color* backgroundColor;

        CSSDisplay display;
        CSSListStyleType listStyleType;

        void apply(std::string);
};

void CSS::apply(std::string attributes) {
    printf("%s\n", attributes.c_str());

    std::vector<std::string> properties;

    if (attributes.find(";") == std::string::npos) {
        properties.push_back(attributes);
    } else {
        std::string::size_type pos = 0, prev_pos = 0;
        while ((pos = attributes.find(";", pos)) != std::string::npos) {
            std::string property(attributes.substr(prev_pos, pos - prev_pos));
            properties.push_back(property);

            prev_pos = ++pos;
        }

        std::string property(attributes.substr(prev_pos, attributes.length() - prev_pos));
        properties.push_back(property);
    }

    printf("done...\n");

    printf("no of props = %lu\n", properties.size());
    for (auto& property : properties) {
        auto prop = trim(property);
        printf("%s\n", prop.c_str());
    }

}