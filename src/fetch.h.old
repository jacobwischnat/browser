#pragma once

#include <curl/curl.h>
#include <string.h>
#include <stdio.h>

#include "types.h"

size_t handle_read(char* buffer, size_t size, size_t nitems, Info* info) {
    memcpy(info->data + info->currentSize, buffer, (size * nitems));

    info->currentSize += size * nitems;

    if (info->currentSize == info->totalSize) {
        info->callback(info->ctx, info->data, info->currentSize, info->userData);
    }

    return size * nitems;
};

size_t handle_headers(char *buffer, size_t size, size_t nitems, Info* info) {
    auto header = std::string(buffer);
    auto splitidx = header.find(": ", 0);
    if (header.rfind("content-length", 0) == 0 && splitidx > 0) {
        auto ssize = header.substr(splitidx + 2);

        info->totalSize = stoi(ssize);
        info->currentSize = 0;
        info->data = (char*) calloc(1, info->totalSize);
    }

    return size * nitems;
}

void fetch_data(Context* ctx, std::string url, void* userData, READ_CALLBACK callback) {
    printf("Fetching %s\n", url.c_str());

    Info info;
    info.ctx = ctx;
    info.callback = callback;
    info.userData = userData;

    CURL* curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handle_read);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, handle_headers);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &info);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &info);

    int res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return;
}