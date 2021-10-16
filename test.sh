#!/bin/bash

mkdir -p tests

# clang++ \
#     -std=c++11 \
#     src/secure_socket_test.cc \
#     /usr/local/Cellar/openssl@1.1/1.1.1l/lib/libssl.dylib \
#     /usr/local/Cellar/openssl@1.1/1.1.1l/lib/libcrypto.dylib \
#     -I vendor/catch2/single_include \
#     -I /usr/local/Cellar/openssl@1.1/1.1.1l/include \
#     -o tests/secureSocketTest

# clang++ \
#     -std=c++17 \
#     src/fetch_test.cc \
#     /usr/local/Cellar/openssl@1.1/1.1.1l/lib/libssl.dylib \
#     /usr/local/Cellar/openssl@1.1/1.1.1l/lib/libcrypto.dylib \
#     -I vendor/catch2/single_include \
#     -I /usr/local/Cellar/openssl@1.1/1.1.1l/include \
#     -o tests/fetchTest

clang++ \
    -std=c++11 \
    src/url_test.cc \
    -I vendor/catch2/single_include \
    -o tests/urlTest

# chmod +x ./tests/secureSocketTest
# ./tests/secureSocketTest