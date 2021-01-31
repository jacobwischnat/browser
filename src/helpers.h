#pragma once

#include "types.h"

std::string trim(std::string value) {
    auto newValue = std::string(value);
    if (newValue.find(" ", 0) == 0) {
        newValue = newValue.substr(1, value.length() - 1);
    }

    if (newValue.find(" ", newValue.length() - 1) == (newValue.length() - 1)) {
        newValue = newValue.substr(1, newValue.length() - 2);
    }

    return newValue;
}

std::vector<std::string> split(std::string text, const char* needle) {
    std::vector<std::string> parts;
    int index = 0;
    int last = 0;
    while ((index = text.find(needle, index)) != std::string::npos) {
        auto part = text.substr(last, index - last);
        parts.push_back(part);

        index += strlen(needle);
        last = index;
    }
    auto part = text.substr(last, text.length() - last);
    parts.push_back(part);

    return parts;
}