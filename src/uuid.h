#pragma once

#include <string>
#include <uuid/uuid.h>

char* generateUUID() {
    uuid_t uid;
    uuid_generate(uid);

    char suid[36];
    uuid_unparse(uid, suid);

    return suid;
}