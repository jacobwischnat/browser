#include "secure_socket.h"

const int CHUNK = 4096;

int main(int argc, char** argv) {
    auto tlsSocket = new SecureSocket();
    std::string data("GET / HTTP/1.1\r\nHOST: www.google.com\r\n\r\n");

    tlsSocket->connect("www.google.com", "443");
    tlsSocket->write((void*) data.c_str(), data.length());

    int totalBytesReceived = 0;
    char* receivedData = (char*) malloc(CHUNK);
    int bytesReceived = tlsSocket->read(&receivedData[totalBytesReceived], CHUNK);

    while (bytesReceived > 0) {
        receivedData = (char*) realloc(receivedData, totalBytesReceived + CHUNK);
        bytesReceived = tlsSocket->read(&receivedData[totalBytesReceived], CHUNK);
        totalBytesReceived += bytesReceived;
    }

    printf("RX: %s\n", receivedData);

    return 0;
}
