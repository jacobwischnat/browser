#pragma once

typedef unsigned long ELEMENT_KEY;

typedef struct {
    int width;
    int height;
} Mode;

typedef struct {
    sf::RenderWindow* window;
    sf::VideoMode* mode;
    // Mode* mode;
    void* elements;
} Context;