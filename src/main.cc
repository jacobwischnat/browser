#include <SFML/Graphics.hpp>
// #include <SFML/Network->hpp>

#include <functional>
#include <gtk/gtk.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <uv.h>
#include <locale>
#include <vector>
#include <thread>

#include "main.h"
#include "event.h"
#include "context.h"
#include "types.h"
#include "network.h"
#include "request.h"
#include "html/html.h"
#include "ui/treeview.h"
#include "ui/entry.h"

// char HOST[] = "http://127.0.0.1:8080/";
std::string HOST("http://localhost:8080/");
// std::string HOST("https://www.google.com/");

int main(int argc, char** argv) {
    auto mode = sf::VideoMode::getDesktopMode();

    auto network = std::make_shared<Network>();
    auto htmlParser = std::make_shared<HTMLParser>(network);

    gtk_init(&argc, &argv);

    int columns = 2;
    GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
    gtk_window_set_title(GTK_WINDOW(win), "Browser - Network Stats");
    gtk_window_set_default_size(GTK_WINDOW(win), 500, 300);

    auto tv = std::make_shared<TreeView>();
    tv->addColumn("Status", G_TYPE_STRING);
    tv->addColumn("Method", G_TYPE_STRING);
    tv->addColumn("Host", G_TYPE_STRING);
    tv->addColumn("IP", G_TYPE_STRING);
    tv->addColumn("Path", G_TYPE_STRING);
    tv->addColumn("Status", G_TYPE_STRING);
    tv->addColumn("Type", G_TYPE_STRING);
    tv->addColumn("Size", G_TYPE_STRING);

    auto handleNewRequest = [=](std::shared_ptr<Request> request, void* userData) {
        printf("New Request: %s\n", request->host.c_str());

        std::vector<std::string> data0;
        data0.push_back(std::string("Pending"));
        data0.push_back(request->method);
        data0.push_back(request->host);
        data0.push_back(std::string(""));
        data0.push_back(std::string(""));
        data0.push_back(std::string(""));
        data0.push_back(std::string(""));
        data0.push_back(std::string(""));
        tv->insertRow(data0);
    };

    network->eventsRequest.addListener(handleNewRequest, NULL);

    auto handleEventStatus = [=](std::shared_ptr<NetworkEvent> event, void* userData) {
        auto requestIndex = std::find(network->requests.begin(), network->requests.end(), event->request);
        int rowIndex = requestIndex - network->requests.begin();

        std::string eventType("UNKNOWN");
        std::string ip("");
        std::string path("");
        std::string status("");
        std::string type("");
        std::string size("");

        switch (event->event) {
            case NET_EVENT_RESOLVING_HOST:
            eventType = std::string("RESOLVING");
            break;

            case NET_EVENT_CONNECTING:
            eventType = std::string("CONNECTING");
            ip = event->request->ip;
            break;

            case NET_EVENT_WRITING:
            eventType = std::string("WRITING");
            ip = event->request->ip;
            break;

            case NET_EVENT_READING:
            eventType = std::string("READING");
            ip = event->request->ip;
            break;

            case NET_EVENT_DONE:
            eventType = std::string("DONE");
            ip = event->request->ip;
            path = event->request->path;
            status = event->request->status;
            type = event->request->contentType;
            size = event->request->size;
            break;

            default:
                return;
        }

        std::vector<std::string> data0;
        data0.push_back(eventType);
        data0.push_back(event->request->method);
        data0.push_back(event->request->host);
        data0.push_back(ip);
        data0.push_back(path);
        data0.push_back(status);
        data0.push_back(type);
        data0.push_back(size);
        tv->updateRow(rowIndex, data0);
    };

    network->eventsNetworkEvent.addListener(handleEventStatus, NULL);

    network->makeRequest(HOST, [=](std::shared_ptr<Request> request) {
        printf("Request for %s is done...\n", request->host.c_str());

        htmlParser->parse(request->body);
    });

    tv->update();

    gtk_container_add(GTK_CONTAINER(win), vbox);

    Entry ent(HOST);

    ent.attachToContainer(vbox);
    tv->attachToContainer(vbox);

    gtk_widget_show_all(win);

    while (true) {
        gtk_main_iteration_do(false);

        network->processTick();
    }

    return 0;
/*
    std::map<ELEMENT_KEY,std::shared_ptr<HTMLElement>> elements;

    sf::RenderWindow window(
        sf::VideoMode(1280, 1280),
        // sf::VideoMode(mode.width, mode.height),
        "Browser Window"); //,
        // sf::Style::Fullscreen);

    Context* ctx = (Context*) malloc(sizeof(Context));
    ctx->mode = &mode;
    ctx->window = &window;
    ctx->elements = (void*) &elements;
    ctx->window->clear(sf::Color::White);
    ctx->window->setFramerateLimit(60);

    // sfg::SFGUI sfgui;

    fetch_data(ctx, HOST.c_str(), &elements, static_cast<READ_CALLBACK>([](Context* ctx, char* buffer, size_t size, void* userData) {
        handle_dom(ctx, gumbo_parse(buffer)->root, 0, 0);
        ctx->window->display();
        // ctx->window->display();
    }));

    std::string command;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code < 26) {
                    command.push_back(event.key.code + (event.key.shift ? 65 : 97));
                } else {
                    switch (event.key.code) {
                        case 50:
                        command = command + ".";
                        break;

                        case 58:
                        if (!command.starts_with("https://")) {
                            command = "https://" + command;
                        }
                        printf("Command: %s\n", command.c_str());
                        break;

                        default:
                        printf("KeyPressed: %d\n", event.key.code);
                    }
                }
            }

            // sf::Text text;
            // sf::Font font;
            // sf::Vector2f pos(100, 100);
            // sf::String sfstring(command);
            // std::string path = "fonts/Roboto-Light.ttf";
            // font.loadFromFile(path);

            // text.setCharacterSize(90);
            // text.setFillColor(sf::Color::Black);
            // text.setStyle(sf::Text::Bold);
            // text.setString(sfstring);
            // text.setPosition(pos);
            // text.setFont(font);

            // window.draw(text);
            // window.display();
        }
    }

    return 0;
    */
}