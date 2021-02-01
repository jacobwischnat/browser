#pragma once

#include <queue>
#include <tuple>
#include <string>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <openssl/ssl.h>

enum NET_EVENT {
    NET_EVENT_NONE,
    NET_EVENT_RESOLVING_HOST,
    NET_EVENT_CONNECTING,
    NET_EVENT_WRITING,
    NET_EVENT_READING,
    NET_EVENT_DONE
};

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

        std::mutex mutex;

        char* responseData;
        int responseSize;

        void addRequestHeader(std::string, std::string);
        void addResponseHeader(std::string, std::string);
        void sendRequest();

        void queueEvent(NET_EVENT event);
        NET_EVENT readEvent();

        Request(std::string);

    private:
        int queueIndex;
        std::queue<NET_EVENT> messageQueue;
};

Request::Request(std::string url) {
    this->queueIndex = 0;
    std::queue<NET_EVENT>messageQueue;
    this->messageQueue = messageQueue;

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

void Request::queueEvent(NET_EVENT event) {
    std::lock_guard<std::mutex> guard(this->mutex);

    this->messageQueue.push(event);
}

NET_EVENT Request::readEvent() {
    std::lock_guard<std::mutex> guard(this->mutex);

    if (this->messageQueue.size() == 0) return NET_EVENT_NONE;

    auto event = this->messageQueue.front();

    this->messageQueue.pop();

    return event;
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

    this->queueEvent(NET_EVENT_RESOLVING_HOST);

    struct hostent *hostEntry = gethostbyname(this->host.c_str());
    // TODO: Add failed to resolve error.
    this->ip = std::string(inet_ntoa(*(struct in_addr*) hostEntry->h_addr_list[0]));

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(stoi(this->port));
    serverAddr.sin_addr.s_addr = inet_addr(this->ip.c_str());

    this->queueEvent(NET_EVENT_CONNECTING);

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

    this->queueEvent(NET_EVENT_WRITING);

    if (this->isSecure) {
        SSL_write(ssl, request.c_str(), request.length());
    } else {
        write(sockFd, request.c_str(), request.length());
    }

    this->responseSize = 0;
    this->responseData = (char*) calloc(1, 1024);
    const int startChunkSize = 1024;
    int chunkSize = 0;
    int nextChunkSize = startChunkSize;

    auto doRead = [=](int chunkSize) -> unsigned long {
        if (this->isSecure) {
            printf("Waiting for chunk\n");
            auto res = (unsigned long) SSL_read(ssl, &this->responseData[this->responseSize], chunkSize);

            printf("Got chunk\n");

            return res;
        }

        return (unsigned long) read(sockFd, &this->responseData[this->responseSize], chunkSize);
    };

    this->queueEvent(NET_EVENT_READING);

    auto findHeaders = [](std::string data) -> std::map<std::string, std::string> {
        std::map<std::string, std::string> headers;

        int row = 0;
        for (auto const& part : split(data, "\r\n")) {
            if (row > 0 && (part.find(":") != std::string::npos)) {
                auto headerParts = split(part, ": ");

                headers.insert({headerParts[0], headerParts[1]});
            } else if (row > 0) break;

            row++;
        }

        return headers;
    };

    auto findBodyOffset = [](std::string data) -> int {
        int row = 0;
        int offset = 0;
        for (auto const& part : split(data, "\r\n")) {
            if (part.compare("") == 0) {
                return offset + 2;
            }

            row++;
            offset += part.length() + 2;
        }

        return 0;
    };

    auto getChunkSize = [](std::string data) -> int {
        int index;
        if ((index = data.find("\n")) != std::string::npos) {
            auto strHex = data.substr(0, index);

            auto sizeBE = std::stol(strHex, nullptr, 16);
            auto sizeLE = (sizeBE >> 8) + ((sizeBE & 0xFF) << 8);

            printf("BE: %X %d\n", sizeBE, sizeBE);
            printf("LE: %X %d\n", sizeLE, sizeLE);

            return sizeBE;
        }

        return 0;
    };

    auto getChunkDataOffset = [](std::string data) -> int {
        int index;
        if ((index = data.find("\r\n", 0)) != std::string::npos) {
            return index + 2;
        }

        return 0;
    };

    bool haveHeaders = false;
    bool isChunked = false;
    bool isFirstChunk = true;

    int i = 0;

    while ((chunkSize = doRead(nextChunkSize)) > 0) {
        this->responseSize += chunkSize;

        printf("%d\n", i);

        this->responseData = (char*) realloc(this->responseData, this->responseSize);

        if (haveHeaders == false) {
            auto headers = findHeaders(std::string(this->responseData));

            if (headers.find("Transfer-Encoding") != headers.end()) {
                auto transferEncoding = headers.at("Transfer-Encoding");

                if (transferEncoding.compare("chunked") == 0) {
                    isChunked = true;
                }

                printf("Transfer Encoding is: %s\n", transferEncoding.c_str());
            }

            if (headers.find("content-length") != headers.end() ||
                headers.find("Content-Length") != headers.end()) {

            }

            haveHeaders = true;

            if (isChunked) {
                auto bodyOffset = findBodyOffset(std::string(this->responseData));

                printf("body is: %s\n", &this->responseData[bodyOffset]);

                chunkSize = getChunkSize(&this->responseData[bodyOffset]);

                auto offset = getChunkDataOffset(&this->responseData[bodyOffset]) + bodyOffset;

                printf("chunkSize = %d\n", chunkSize);
                printf("offset = %d\n", offset);
                printf("chunk body is: %s\n", &this->responseData[offset]);

                nextChunkSize = chunkSize - strlen(&this->responseData[offset]);

                isFirstChunk = false;
            }
        } else if (isChunked) {
            chunkSize = getChunkSize(&this->responseData[this->responseSize]);

            printf("chunkSize = %d\n", chunkSize);
            printf("chunk body is: %s\n", &this->responseData[this->responseSize]);

            if (chunkSize == 0) break;
        }

        this->responseData = (char*) realloc(this->responseData, nextChunkSize + this->responseSize);

        printf("chunkSize = %d\n", chunkSize);

        i++;
        // if (i > 3) break;
    }

    printf("Done\n");

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

    this->queueEvent(NET_EVENT_DONE);
}
