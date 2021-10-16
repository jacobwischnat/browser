#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "fetch.h"

TEST_CASE("Should create the Fetch", "[Fetch]") {
    auto fetch = new Fetch();

    SECTION("Default Method should be GET") {
        REQUIRE(strcmp(fetch->method.c_str(), "GET") == 0);
    }

    SECTION("Should set method to POST") {
        fetch->setMethod("POST");
        REQUIRE(strcmp(fetch->method.c_str(), "POST") == 0);
    }

    SECTION("Should add a header") {
        fetch->addHeader("Content-Type", "application/json");
        REQUIRE(fetch->headers.size() == 1);
        REQUIRE(fetch->headers.find("Content-Type") != 0);
    }
}