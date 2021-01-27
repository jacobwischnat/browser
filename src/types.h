#pragma once

#include <SFML/Graphics.hpp>

typedef unsigned long ELEMENT_KEY;

typedef struct {
    int width;
    int height;
} Mode;

typedef struct {
    ELEMENT_KEY parent;
    ELEMENT_KEY previousSibling;
} Tracking;

typedef struct {
    CURL* curl;
    sf::RenderWindow* window;
    // sf::VideoMode* mode;
    Mode* mode;
    std::map<ELEMENT_KEY,void*>* elements;
} Context;

typedef void (*READ_CALLBACK)(Context*, char*, size_t, void*);

typedef struct {
    Context* ctx;
    size_t totalSize;
    size_t currentSize;
    char* data;
    void* userData;
    READ_CALLBACK callback;
} Info;