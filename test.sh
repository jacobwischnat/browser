#!/bin/bash

mkdir -p tests

clang++ \
    -std=c++11 \
    src/secure_socket_test.cc \
    -I vendor/catch2/single_include \
    -o tests/secureSocketTest

# chmod +x ./tests/secureSocketTest
# ./tests/secureSocketTest