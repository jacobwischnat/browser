#pragma once

#include <SFML/Graphics.hpp>

typedef struct {
    int width;
    int height;
} Mode;

typedef struct {
    CURL* curl;
    sf::RenderWindow* window;
    // sf::VideoMode* mode;
    Mode* mode;
} Context;

typedef void (*READ_CALLBACK)(Context*, char*, size_t);

typedef struct {
    Context* ctx;
    size_t totalSize;
    size_t currentSize;
    char* data;
    READ_CALLBACK callback;
} Info;