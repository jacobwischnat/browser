#include "event.h"

#include <string>
#include <stdio.h>

int main(int argc, char** argv) {
    EventEmitter<std::string> event;

    std::string boo("hii!\n");

    event.addListener([](std::string a, void* b) {
        printf("BOOM! %s\n", a.c_str());
    }, NULL);

    event.emit(boo);

    return 0;
}