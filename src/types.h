#pragma once

#include <SFML/Graphics.hpp>

#include "context.h"

typedef void (*READ_CALLBACK)(Context*, char*, size_t, void*);

typedef struct {
    Context* ctx;
    size_t totalSize;
    size_t currentSize;
    char* data;
    void* userData;
    READ_CALLBACK callback;
} Info;