#pragma once

#include <string>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

typedef struct sockaddr sockaddr_t;
typedef struct sockaddr_in sockaddr_in_t;

enum SecureSocketState {
    PENDING,
    CONNECTING,
    CONNECTED,
    SENDING,
    RECEIVING,
    PAUSED,
    CLOSED,
    ERROR
};

ssize_t _read(int fd, void* buf, size_t count) {
    return read(fd, buf, count);
}

class SecureSocket {
    private:
        SSL* ssl;
        SSL_CTX* sslContext;
        sockaddr_in_t hostInfo;

        int socketHandle;

    public:
        SecureSocket();
        ~SecureSocket();

        SecureSocketState state = SecureSocketState::PENDING;

        inline void connect(std::string host, unsigned int port) {
            this->state = SecureSocketState::CONNECTING;

            this->hostInfo.sin_family = AF_INET;
            this->hostInfo.sin_port = htons(port);
            this->hostInfo.sin_addr.s_addr = inet_addr(host.c_str());

            this->socketHandle = socket(AF_INET, SOCK_STREAM, 0);

            if (this->socketHandle < 0) {
                this->state = SecureSocketState::ERROR;
                return;
            }

            this->state = SecureSocketState::CONNECTED;
        }

        inline unsigned int write(void* data, unsigned int length) {
            auto result = send(this->socketHandle, data, (size_t) length, 0);

            return 0;
        }

        inline unsigned int read(void* data, unsigned int byteCount) {
            auto result = _read(this->socketHandle, data, byteCount);

            return 0;
        }
};

SecureSocket::SecureSocket() {
    SSL_load_error_strings();
    SSL_library_init();
    OpenSSL_add_all_algorithms();
}

SecureSocket::~SecureSocket() {
    ERR_free_strings();
    EVP_cleanup();
    SSL_shutdown(this->ssl);
    SSL_free(this->ssl);
}