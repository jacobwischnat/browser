#pragma once

#include <map>
#include <string>

class FetchResponse {
    private:
        inline void handleResponseLine(std::string responseLine) {
            auto spaceIndex = responseLine.find(" ");
            this->protocol = responseLine.substr(0, spaceIndex);
            this->response = responseLine.substr(spaceIndex + 1);
        }

        inline void handleHeaderLine(std::string headerLine) {
            auto spaceIndex = headerLine.find(": ");
            auto name = headerLine.substr(0, spaceIndex);
            auto value = headerLine.substr(spaceIndex + 1);

            this->headers.insert_or_assign(name, value);
        }

        inline void handleChunkedBody() {
            std::string chunkedBody((char*) this->rawBody);

            int lastChunkOffset = 0;
            int totalSize = 0;
            int eolIndex = 0;
            int bodySize = 0;
            do {
                eolIndex = chunkedBody.find("\r\n", totalSize);

                auto sChunkSize = chunkedBody.substr(0, eolIndex);
                auto chunkSize = std::stol(sChunkSize, nullptr, 16);
                auto chunk = malloc(chunkSize);
                auto chunkPointer = &((char*)this->rawBody)[sChunkSize.length() + 2];

                totalSize += chunkSize;

                this->body = realloc(this->body, totalSize);

                memcpy(&((char*)this->body)[bodySize], chunkPointer, chunkSize);

                bodySize += chunkSize;
            } while (eolIndex != std::string::npos);
        }

    public:
        FetchResponse(std::string metaData, void* body);

        std::string protocol;
        std::string response;
        std::map<std::string, std::string> headers;

        void* body;
        void* rawBody;

        inline bool hasHeader(std::string name) {
            return this->headers.find(name) != this->headers.end() ? true : false;
        }

        inline std::shared_ptr<std::string> getHeader(std::string name) {
            auto it = this->headers.find(name);
            if (it != this->headers.end()) {
                return std::make_shared<std::string>(it->second);
            }

            return nullptr;
        }
};

FetchResponse::FetchResponse(std::string metaData, void* rawBody) {
    int newLineIndex = 0;
    int lastIndex = 0;
    do {
        newLineIndex = metaData.find("\n", lastIndex);

        if (lastIndex == 0) {
            this->handleResponseLine(metaData.substr(0, newLineIndex));
        } else {
            this->handleHeaderLine(metaData.substr(lastIndex, newLineIndex));
        }

        lastIndex = newLineIndex + 1;
    } while (newLineIndex != std::string::npos);

    this->body = malloc(1024);
    this->rawBody = rawBody;

    this->handleChunkedBody();
}