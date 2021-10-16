#!/bin/bash

clang++ \
    -std=c++11 \
    src/secure_socket_example.cc \
    /usr/local/Cellar/openssl@1.1/1.1.1l/lib/libssl.dylib \
    /usr/local/Cellar/openssl@1.1/1.1.1l/lib/libcrypto.dylib \
    -I /usr/local/Cellar/openssl@1.1/1.1.1l/include \
    -o examples/secureSocketExample

clang++ \
    --std=c++2a \
    src/url_example.cc \
    -o examples/urlExample

clang++ \
    --std=c++2a \
    src/fetch_example.cc \
    /usr/local/Cellar/openssl@1.1/1.1.1l/lib/libssl.dylib \
    /usr/local/Cellar/openssl@1.1/1.1.1l/lib/libcrypto.dylib \
    -I /usr/local/Cellar/openssl@1.1/1.1.1l/include \
    -o examples/fetchExample