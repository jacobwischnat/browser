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

typedef struct timeval timeval_t;
typedef struct addrinfo addrinfo_t;
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

int _connect(int socket, sockaddr_t* address, socklen_t address_length) {
    return connect(socket, address, address_length);
}

class SecureSocket {
    public:
        SecureSocket();
        ~SecureSocket();

        SSL* ssl;
        SSL_CTX* sslContext;
        const SSL_METHOD* sslMethod;

        addrinfo_t* pHostInfo;
        addrinfo_t hostInfo;

        int socketHandle;
        unsigned long bytesSent = 0;
        unsigned long bytesReceived = 0;

        SecureSocketState state = SecureSocketState::PENDING;

        inline void connect(std::string host, std::string port) {
            this->state = SecureSocketState::CONNECTING;

            memset(&this->hostInfo, 0, sizeof(addrinfo_t));
            this->pHostInfo = (addrinfo_t*) calloc(1, sizeof(addrinfo_t*));
            int getAddrRes = getaddrinfo(
                host.c_str(),
                port.c_str(),
                &this->hostInfo,
                &this->pHostInfo);

            if (getAddrRes < 0) {
                this->state = SecureSocketState::ERROR;
                return;
            }

            this->socketHandle = socket(
                AF_INET,
                SOCK_STREAM,
                0);

            if (this->socketHandle < 0) {
                this->state = SecureSocketState::ERROR;
                return;
            }

            int connRes = _connect(
                this->socketHandle,
                this->pHostInfo->ai_addr,
                (int) this->pHostInfo->ai_addrlen);

            if (connRes > 0) {
                this->state = SecureSocketState::ERROR;
                return;
            }

            this->sslMethod = TLS_client_method();
            this->sslContext = SSL_CTX_new(this->sslMethod);
            this->ssl = SSL_new(this->sslContext);

            SSL_set_fd(this->ssl, this->socketHandle);

            auto sslConnStatus = SSL_connect(this->ssl);
            auto cipher = SSL_get_cipher(this->ssl);

            if (sslConnStatus <= 0) {
                this->state = SecureSocketState::ERROR;
                return;
            }

            this->state = SecureSocketState::CONNECTED;
        }

        inline unsigned int write(void* data, int length) {
            int bytes = SSL_write(this->ssl, data, length);
            this->bytesSent += bytes;

            return bytes;
        }

        inline unsigned int read(void* data, unsigned int byteCount, int timeout = 1) {
            timeval_t _timeout;
            _timeout.tv_sec = timeout;

            setsockopt(
                this->socketHandle,
                SOL_SOCKET,
                SO_RCVTIMEO,
                &_timeout,
                sizeof(timeval_t));

            int bytes = SSL_read(this->ssl, data, byteCount);

            if (bytes < 0) return 0;

            this->bytesReceived += bytes;

            return bytes;
        }

        inline void close() {
            SSL_shutdown(this->ssl);
            SSL_free(this->ssl);
            SSL_CTX_free(this->sslContext);
        }
};

SecureSocket::SecureSocket() {
    SSL_library_init();
    SSL_load_error_strings();
    OpenSSL_add_all_algorithms();
}

SecureSocket::~SecureSocket() {
    ERR_free_strings();
    EVP_cleanup();
}