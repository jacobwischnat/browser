#pragma once

#include <SFML/Graphics.hpp>

#include "context.h"
#include "html/html_element.h"

typedef struct {
    ELEMENT_KEY parent;
    ELEMENT_KEY previousSibling;
    GumboNode* node;
} Tracking;

typedef void (*READ_CALLBACK)(Context*, char*, size_t, void*);

typedef struct {
    Context* ctx;
    size_t totalSize;
    size_t currentSize;
    char* data;
    void* userData;
    READ_CALLBACK callback;
} Info;