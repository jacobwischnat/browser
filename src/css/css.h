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

void CSS::apply(std::string attrs) {
    std::vector<std::string> properties;

    auto split = [](std::shared_ptr<std::string> string) -> std::vector<std::string> {
        std::vector<std::string> parts;

        auto key = string->substr(0, string->find(": ", 0));
        auto value = string->substr(string->find(": ", 0) + 2, string->length());

        parts.push_back(key);
        parts.push_back(value);

        return parts;
    };

    auto getUnit = [](std::shared_ptr<std::string> string) -> CSSUnitType {
        if (string->ends_with("px")) return CSS_UNIT_TYPE_PX;
        if (string->ends_with("em")) return CSS_UNIT_TYPE_EM;
        if (string->ends_with("rem")) return CSS_UNIT_TYPE_REM;

        return CSS_UNIT_TYPE_UNKNOWN;
    };

    auto getValueByUnit = [](std::shared_ptr<std::string> string, CSSUnitType unitType) -> int {
        if (unitType == CSS_UNIT_TYPE_PX) {
            return stoi(string->substr(0, string->length() - 2));
        }

        return 0;
    };

    auto attributes = attrs.substr(0, attrs.length() - 1);
    printf("attributes = %s\n", attributes.c_str());

    if (attributes.find(";") == std::string::npos) {
        properties.push_back(attributes);
    } else {
        std::string::size_type pos = 0, prev_pos = 0;
        while ((pos = attributes.find(";", pos)) != std::string::npos) {
            std::string property(attributes.substr(prev_pos, pos - prev_pos));
            properties.push_back(trim(property));

            prev_pos = ++pos;
        }

        std::string property(attributes.substr(prev_pos, attributes.length() - prev_pos));
        properties.push_back(trim(property));
    }

    for (auto& property : properties) {
        auto sharedProp = std::make_shared<std::string>(trim(property));
        auto prop = split(sharedProp);
        auto key = prop[0];
        auto value = std::make_shared<std::string>(prop[1]);

        if (key.compare("width") == 0) {
            auto unit = getUnit(value);
            auto unitValue = getValueByUnit(value, unit);

            this->width = unitValue;
        }
    }
}