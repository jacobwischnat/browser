#pragma once

#include <thread>
#include <vector>
#include <utility>
#include <functional>

#include "request.h"

#define NET_EVENT_CALLBACK_SIGNATURE        std::function<void(std::shared_ptr<Request>,NET_EVENT,void*)>
#define NET_REQUEST_CALLBACK_SIGNATURE      std::function<void(std::shared_ptr<Request>,void*)>

class Network {
    public:

        std::shared_ptr<Request> makeRequest(std::string url);

        void onRequest(NET_REQUEST_CALLBACK_SIGNATURE);
        void onRequest(NET_REQUEST_CALLBACK_SIGNATURE, void*);

        void onEvent(NET_EVENT_CALLBACK_SIGNATURE);
        void onEvent(NET_EVENT_CALLBACK_SIGNATURE, void*);

        void processTick();

        std::vector<std::shared_ptr<Request>> requests;

        Network();

    private:
        int requestIndex;

        std::vector<std::thread> threads;

        std::vector<std::tuple<NET_EVENT_CALLBACK_SIGNATURE,void*>> onEventCallbacks;
        std::vector<std::tuple<NET_REQUEST_CALLBACK_SIGNATURE,void*>> onRequestCallbacks;
};

std::shared_ptr<Request> Network::makeRequest(std::string url) {
    auto request = new Request(url);

    std::shared_ptr<Request> sharedRequest(request);

    this->requests.push_back(sharedRequest);

    for (auto const& req : this->onRequestCallbacks) {
        NET_REQUEST_CALLBACK_SIGNATURE callback;
        void* userData;
        std::tie(callback, userData) = req;

        callback(sharedRequest, userData);
    }

    this->threads.push_back(std::move(std::thread(&Request::sendRequest, sharedRequest)));

    return sharedRequest;
}

void Network::onEvent(NET_EVENT_CALLBACK_SIGNATURE callback) {
    this->onEvent(callback, NULL);
}

void Network::onEvent(NET_EVENT_CALLBACK_SIGNATURE callback, void* userData) {
    this->onEventCallbacks.push_back(std::make_tuple(callback, userData));
}

void Network::onRequest(NET_REQUEST_CALLBACK_SIGNATURE callback) {
    this->onRequest(callback, NULL);
}

void Network::onRequest(NET_REQUEST_CALLBACK_SIGNATURE callback, void* userData) {
    this->onRequestCallbacks.push_back(std::make_tuple(callback, userData));
}

void Network::processTick() {
    auto index = this->requestIndex % this->requests.size();
    auto request = this->requests[index];

    NET_EVENT event = request->readEvent();

    if (event) {
        for (auto const& req : this->onEventCallbacks) {
            NET_EVENT_CALLBACK_SIGNATURE callback;
            void* userData;
            std::tie(callback, userData) = req;

            callback(request, event, userData);
        }

        if (event == NET_EVENT_DONE) {
            this->threads[index].join();
        }
    }

    this->requestIndex++;
}

Network::Network() {
    this->requestIndex = 0;

    // std::vector<std::thread> threads;
    // this->threads = threads;

    std::vector<std::shared_ptr<Request>> requests;
    this->requests = requests;

    std::vector<std::tuple<NET_REQUEST_CALLBACK_SIGNATURE,void*>> callbacks;
    this->onRequestCallbacks = callbacks;
}