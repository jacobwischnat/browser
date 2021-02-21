#pragma once

#include <vector>
#include <functional>

enum EventOpt {
    EventOptNone,
    EventOptOnce
};

template <typename T>
class EventEmitter {
    public:
        EventEmitter();

        int addListener(std::function<void(T,void*)>, void*, EventOpt);
        int addListener(std::function<void(T,void*)>, void*);
        void emit(T);

    private:
        std::vector<std::tuple<std::function<void(T,void*)>, void*, EventOpt>> handlers;
};

template <typename T>
EventEmitter<T>::EventEmitter() {
    std::vector<std::tuple<std::function<void(T,void*)>, void*, EventOpt>> handlers;

    this->handlers = handlers;
}

template <typename T>
int EventEmitter<T>::addListener(std::function<void(T,void*)> cb, void* userData) {
    int id = this->handlers.size();

    this->handlers.push_back({cb, userData, EventOptNone});

    return id;
}

template <typename T>
int EventEmitter<T>::addListener(std::function<void(T,void*)> cb, void* userData, EventOpt opt) {
    int id = this->handlers.size();

    this->handlers.push_back({cb, userData, opt});

    return id;
}

template <typename T>
void EventEmitter<T>::emit(T eventData) {
    int index = 0;
    for (const auto handler : this->handlers) {
        std::function<void(T,void*)> callback;
        void* userData;
        EventOpt opt;
        std::tie(callback, userData, opt) = handler;

        callback(eventData, userData);

        if (opt == EventOptOnce) {
            this->handlers.erase(this->handlers.begin() + index);
        }

        index++;
    }
}