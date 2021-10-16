#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "secure_socket.h"

TEST_CASE("Should create the socket", "[Socket]") {
    auto socket = new SecureSocket();

    SECTION("Socket state should be pending") {
        REQUIRE(socket->state == SecureSocketState::PENDING);
    }

    SECTION("Should connect") {
        socket->connect("www.google.com", "443");
        REQUIRE(socket->state == SecureSocketState::CONNECTED);
    }

    SECTION("Should send some data") {
        std::string data("GET / HTTP/1.1\r\nHOST: www.google.com\r\n\r\n");

        socket->connect("www.google.com", "443");
        socket->write((void*) data.c_str(), data.length());

        REQUIRE(socket->state == SecureSocketState::CONNECTED);
        REQUIRE(socket->bytesSent == data.length());
    }

    SECTION("Should read some data") {
        char* dataIn = (char*) calloc(1, 1024);

        std::string data("GET / HTTP/1.1\r\nHOST: www.google.com\r\n\r\n");

        socket->connect("www.google.com", "443");
        socket->write((void*) data.c_str(), data.length());
        socket->read((void*) dataIn, 1024);

        REQUIRE(socket->state == SecureSocketState::CONNECTED);
        REQUIRE(socket->bytesSent == data.length());
        REQUIRE(socket->bytesReceived == 1024);
    }
}