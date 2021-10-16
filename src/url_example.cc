#include <stdio.h>

#include "url.hpp"

int main(int argc, char** argv) {
    if (argc < 1) {
        printf("usage: url_example [url]\n");

        return 0;
    }

    std::string urlString(argv[1]);
    BrowserURL url(urlString);

    printf("protocol: %s\n", url.protocol.c_str());
    printf("hostname: %s\n", url.hostname.c_str());
    printf("query: %s\n", url.query.c_str());
    printf("path: %s\n", url.path.c_str());
    printf("port: %s\n", url.port.c_str());

    return 0;
}