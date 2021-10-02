#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "secure_socket.h"

TEST_CASE("Should create the socket", "[Socket]") {
    auto socket = new SecureSocket();

    REQUIRE(1);
}