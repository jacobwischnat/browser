#pragma once

#include <memory>
#include <thread>
#include <vector>
#include <utility>
#include <functional>

#include "event.h"
#include "request.h"

class NetworkEvent {
    public:
        NET_EVENT event;
        std::shared_ptr<Request> request;

        NetworkEvent(NET_EVENT, std::shared_ptr<Request>);
};

NetworkEvent::NetworkEvent(NET_EVENT event, std::shared_ptr<Request> request) {
    this->event = event;
    this->request = request;
}

class Network {
    public:

        std::shared_ptr<Request> makeRequest(std::string url, std::function<void(std::shared_ptr<Request>)> callback);

        void processTick();

        std::vector<std::shared_ptr<Request>> requests;

        EventEmitter<std::shared_ptr<Request>> eventsRequest;
        EventEmitter<std::shared_ptr<Request>> eventsRequestComplete;
        EventEmitter<std::shared_ptr<NetworkEvent>> eventsNetworkEvent;

        Network();

    private:
        int nextRequestId;
        int requestIndex;

        std::vector<std::thread> threads;
};

std::shared_ptr<Request> Network::makeRequest(std::string url, std::function<void(std::shared_ptr<Request>)> callback) {
    auto request = new Request(this->nextRequestId++, url);
    request->addRequestHeader("Accept-Encoding", "identity");

    std::shared_ptr<Request> sharedRequest(request);

    this->eventsRequestComplete.addListener([=](std::shared_ptr<Request> req, void*) { callback(req); }, NULL, EventOptOnce);

    this->requests.push_back(sharedRequest);

    this->eventsRequest.emit(sharedRequest);

    this->threads.push_back(std::move(std::thread(&Request::sendRequest, sharedRequest)));

    return sharedRequest;
}

void Network::processTick() {
    auto index = this->requestIndex % this->requests.size();
    auto request = this->requests[index];

    NET_EVENT event = request->readEvent();

    if (event) {
        std::shared_ptr<Request> sharedRequest(request);
        auto netEvent = std::make_shared<NetworkEvent>(event, sharedRequest);
        this->eventsNetworkEvent.emit(netEvent);

        if (event == NET_EVENT_DONE) {
            this->eventsRequestComplete.emit(sharedRequest);
            this->threads[index].join();
        }
    }

    this->requestIndex++;
}

Network::Network() {
    this->requestIndex = 0;

    EventEmitter<std::shared_ptr<Request>> eventsRequest;
    EventEmitter<std::shared_ptr<Request>> eventsRequestComplete;
    EventEmitter<std::shared_ptr<NetworkEvent>> eventsNetworkEvent;

    this->eventsRequest = eventsRequest;
    this->eventsNetworkEvent = eventsNetworkEvent;
    this->eventsRequestComplete = eventsRequestComplete;
}