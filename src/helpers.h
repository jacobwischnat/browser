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