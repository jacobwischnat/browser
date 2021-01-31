#pragma once

#include <string>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <openssl/ssl.h>

class Request {
    public:
        bool isSecure;

        std::string contentType;
        std::string version;
        std::string method;
        std::string status;
        std::string prefix;
        std::string host;
        std::string path;
        std::string type;
        std::string size;
        std::string port;
        std::string body;
        std::string ip;

        std::vector<std::tuple<std::string, std::string>> requestHeaders;
        std::vector<std::tuple<std::string, std::string>> responseHeaders;

        char* responseData;
        int responseSize;

        void addRequestHeader(std::string, std::string);
        void addResponseHeader(std::string, std::string);
        void sendRequest();

        Request(std::string);
};

Request::Request(std::string url) {
    std::vector<std::tuple<std::string, std::string>> requestHeaders;

    this->path = std::string("/");
    this->method = std::string("GET");

    int prefixSize = 0;
    if (url.starts_with(std::string("http://"))) {
        this->prefix = std::string("http://");
        this->port = std::string("80");
        this->isSecure = false;
        prefixSize = std::string("http://").length();
    } else {
        this->prefix = std::string("https://");
        this->port = std::string("443");
        this->isSecure = true;
        prefixSize = std::string("https://").length();
    }

    int end = url.length();
    if (url.find("/", prefixSize) != std::string::npos) {
        end = url.find("/", prefixSize);

        this->path = url.substr(end);
    }

    if (url.find(":", prefixSize) != std::string::npos) {
        int index = url.find(":", prefixSize) + 1;

        this->port = url.substr(index, end - index);

        end = index - 1;
    }

    this->host = url.substr(prefixSize, end - prefixSize);

    requestHeaders.push_back(std::make_tuple(std::string("Host"), this->host));

    this->requestHeaders = requestHeaders;
}

void Request::addRequestHeader(std::string key, std::string value) {
    this->requestHeaders.push_back(std::make_tuple(key, value));
}

void Request::addResponseHeader(std::string key, std::string value) {
    this->responseHeaders.push_back(std::make_tuple(key, value));
}

void Request::sendRequest() {
    auto method = TLS_client_method();
    auto sslCtx = SSL_CTX_new(method);
    auto ssl = SSL_new(sslCtx);
    auto sockFd = socket(AF_INET, SOCK_STREAM, 0);

    struct hostent *hostEntry = gethostbyname(this->host.c_str());
    // TODO: Add failed to resolve error.
    this->ip = std::string(inet_ntoa(*(struct in_addr*) hostEntry->h_addr_list[0]));

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(stoi(this->port));
    serverAddr.sin_addr.s_addr = inet_addr(this->ip.c_str());

    connect(sockFd, (struct sockaddr*) &serverAddr, sizeof(serverAddr));

    if (this->isSecure) {
        SSL_set_fd(ssl, sockFd);
        auto sslConnStatus = SSL_connect(ssl);
        auto cipher = SSL_get_cipher(ssl);
    }

    auto request = this->method + std::string(" ") + this->path + std::string(" HTTP/1.1\r\n");

    for (auto const& headerTup : this->requestHeaders) {
        std::string key;
        std::string value;
        std::tie(key, value) = headerTup;

        request = request + key + ": " + value + "\r\n";
    }
    request = request + "\r\n";

    if (this->isSecure) {
        SSL_write(ssl, request.c_str(), request.length());
    } else {
        write(sockFd, request.c_str(), request.length());
    }

    this->responseSize = 0;
    this->responseData = (char*) malloc(1024);
    int chunkSize = 0;

    auto doRead = [=]() -> unsigned long {
        if (this->isSecure) {
            auto res = (unsigned long) SSL_read(ssl, &this->responseData[this->responseSize], 1024);
            return res;
        }

        return (unsigned long) read(sockFd, &this->responseData[this->responseSize], 1024);
    };

    while ((chunkSize = doRead()) > 0) {
        this->responseSize += chunkSize;
        this->responseData = (char*) realloc(this->responseData, 1024 + this->responseSize);
    }

    close(sockFd);

    if (this->isSecure) {
        SSL_free(ssl);
        SSL_CTX_free(sslCtx);
    }

    this->responseSize = strlen(this->responseData);

    std::string responseData(this->responseData);

    int row = 0;
    int headers = 0;
    std::string body("");
    for (auto const& part : split(responseData, "\r\n")) {
        if (row == 0) {
            auto responseParts = split(part, " ");

            this->version = responseParts[0];
            this->status = responseParts[1] + " " + responseParts[2];
        } else if (headers == 0 && part.find(":") != std::string::npos) {
            auto headerParts = split(part, ": ");

            this->responseHeaders.push_back(std::make_tuple(headerParts[0], headerParts[1]));
        } else {
            headers = 1;

            body += part;
        }

        row++;
    }

    this->body = body + "\0";

    for (auto const& n : this->responseHeaders) {
        std::string key;
        std::string value;
        std::tie(key, value) = n;

        if (key.compare("content-length") == 0) this->size = value;
        if (key.compare("Content-Length") == 0) this->size = value;
        if (key.compare("content-type") == 0) {
            this->contentType = value;
            if (value.find(";")) {
                this->contentType = value.substr(0, value.find(";"));
            }
        }
    }
}
