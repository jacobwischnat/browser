#pragma once

#include "request.h"

typedef void(*ON_REQUEST_CB)(Request*, void*);

class Network {
    public:
        std::vector<std::shared_ptr<Request>> requests;
        std::vector<std::tuple<ON_REQUEST_CB,void*>> onRequestCallbacks;

        void makeRequest(std::string url);

        void onRequest(ON_REQUEST_CB, void*);

        Network();
};

Network::Network() {
    std::vector<std::shared_ptr<Request>> requests;
    this->requests = requests;

    std::vector<std::tuple<ON_REQUEST_CB,void*>> callbacks;
    this->onRequestCallbacks = callbacks;
}