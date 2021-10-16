#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "url.hpp"

TEST_CASE("URL Parser tests", "[URL]") {
    SECTION("Should parse a URL") {
        BrowserURL url("https://www.github.com:8443/jacobwischnat/browser/readme.mp4?raw=true");

        REQUIRE(url.protocol.compare("https://") == 0);
        REQUIRE(url.hostname.compare("www.github.com") == 0);
        REQUIRE(url.query.compare("?raw=true") == 0);
        REQUIRE(url.path.compare("/jacobwischnat/browser/readme.mp4") == 0);
        REQUIRE(url.port.compare("8443") == 0);
    }

}