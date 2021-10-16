#include "fetch.h"


int main(int argc, char** argv) {
    if (argc < 2) {
        printf("usage: fetch_example [url]\n");

        return 0;
    }

    std::string requestUrl(argv[1]);
    Fetch request(requestUrl);
    request.makeRequest();
    printf("%s", (char*) request.response->body);

    return 0;
}