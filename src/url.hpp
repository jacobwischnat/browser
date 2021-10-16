#pragma once

#include <map>
#include <string>

class BrowserURL {
    public:
        BrowserURL(std::string url);

        std::string protocol;
        std::string hostname;
        std::string query;
        std::string path;
        std::string port;
};

BrowserURL::BrowserURL(std::string url) {
    bool hasProtocol = false;
    if (url.rfind("https://") == 0) {
        this->protocol = "https://";
        this->port = std::string("443");
        hasProtocol = true;
    } else if (url.rfind("http://") == 0) {
        this->protocol = "http://";
        this->port = std::string("80");
        hasProtocol = true;
    } else {
        this->protocol = "https://";
        this->port = std::string("443");
    }

    auto pathIndex = url.find("/", (size_t) this->protocol.length());
    if (pathIndex == std::string::npos) {
        this->path = "/";
        this->hostname = url.substr(hasProtocol ? this->protocol.length() : 0, url.length());
    } else {
        this->hostname = url.substr(
            hasProtocol ? this->protocol.length() : 0,
            pathIndex - (hasProtocol ? this->protocol.length() : 0));

        this->path = url.substr(pathIndex);
    }

    auto queryIndex = this->path.find("?");
    if (queryIndex != std::string::npos) {
        this->query = this->path.substr(queryIndex);
        this->path = this->path.substr(0, queryIndex);
    }

    auto colonIndex = this->hostname.find(":");
    if (colonIndex != std::string::npos) {
        this->port = this->hostname.substr(colonIndex + 1);
        this->hostname = this->hostname.substr(0, colonIndex);
    }
}

