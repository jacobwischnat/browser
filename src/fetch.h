#pragma once

#include <map>
#include <string>
#include <printf.h>

#include "url.hpp"
#include "secure_socket.h"
#include "fetch_response.hpp"

class Fetch {
    private:
        inline void handleResponse(std::string metadata, void* data) {
            this->response = std::make_shared<FetchResponse>(metadata, data);
        }

    public:
        Fetch(std::string url);
        bool synchronous = false;

        std::shared_ptr<BrowserURL> url;
        std::string method = std::string("GET");

        std::map<std::string, std::string> headers;

        std::shared_ptr<FetchResponse> response;

        void* body;
        unsigned long bodyLength;

        inline bool isSecure() {
            return this->url->protocol.compare("https://") == 0;
        }

        inline void setMethod(std::string method) {
            this->method = method;
        }

        inline bool hasHeader(std::string name) {
            return this->headers.find(name) == this->headers.end() ? true : false;
        }

        inline void addHeader(std::string name, std::string value) {
            this->headers.insert_or_assign(name, value);
        }

        inline void setBody(void* body, unsigned long length) {
            this->body = body;
            this->bodyLength = length;
        }

        inline void setSynchronous(bool synchronous) {
            this->synchronous = synchronous;
        }

        void makeRequest() {
            std::string hostKey("HOST");
            if (this->hasHeader(hostKey)) {
                this->addHeader(hostKey, this->url->hostname);
            }

            if (this->isSecure()) {
                SecureSocket socket;
                socket.connect(this->url->hostname, this->url->port);

                char request[4096];

                auto requestTemplate = "%s %s HTTP/1.1\r\n";
                sprintf(
                    request,
                    requestTemplate,
                    this->method.c_str(),
                    this->url->path.c_str());

                for (std::pair<std::string, std::string> header : this->headers) {
                    std::string name = header.first;
                    std::string value = header.second;

                    sprintf(&request[strlen(request)], "%s: %s\r\n", name.c_str(), value.c_str());
                }

                sprintf(&request[strlen(request)], "\r\n");

                socket.write((void*)request, strlen(request));

                void* data = calloc(1, 4096);

                int totalBytesRead = 0;
                int bytesRead = 0;
                do {
                    bytesRead = socket.read((char*)data + totalBytesRead, 4096);
                    totalBytesRead += bytesRead;
                    data = realloc(data, totalBytesRead + 4096);
                } while (bytesRead != 0);

                std::string dataString((char*) data);
                auto endMetaData = dataString.find("\r\n\r\n");
                if (endMetaData != std::string::npos) {
                    auto metaData = dataString.substr(0, endMetaData);

                    handleResponse(metaData, &((char*)data)[endMetaData + 4]);
                }
            }
        }
};

Fetch::Fetch(std::string url) {
    this->url = std::make_shared<BrowserURL>(url);
}