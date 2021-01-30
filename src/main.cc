#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <functional>
#include <gtk/gtk.h>
#include <strings.h>
#include <stdio.h>
#include <gumbo.h>
#include <uv.h>
#include <locale>
#include <vector>

#include "main.h"
#include "fetch.h"
#include "context.h"
#include "types.h"
#include "html/html.h"
#include "ui/treeview.h"

char HOST[] = "http://127.0.0.1:8080/";

GumboAttribute* find_attribute(GumboNode* node, const char* name) {
    auto attributes = new GumboVector(node->v.element.attributes);

    for (int i = 0; i < attributes->length; i += 1) {
        GumboAttribute* attribute = (GumboAttribute*) attributes->data[i];

        if (strcmp(attribute->name, name) == 0) {
            return attribute;
        }
    }

    return NULL;
}

ELEMENT_KEY handle_dom(Context* ctx, GumboNode* node, ELEMENT_KEY parent, ELEMENT_KEY previous) {
    auto elements = (std::map<ELEMENT_KEY,std::shared_ptr<HTMLElement>>*) ctx->elements;
    switch (node->type) {
        case GUMBO_NODE_ELEMENT:
            {
                printf("%d\n", node->v.element.tag);
                switch (node->v.element.tag) {
                    case GUMBO_TAG_HR:
                    {
                        auto element = std::make_shared<HTMLElement>(ctx);
                        element->setParent(parent);
                        element->setPreviousSibling(previous);
                        auto attributes = std::make_shared<GumboVector>(node->v.element.attributes);
                        element->setAttributes(attributes);

                        auto hrElement = HTMLHR(ctx, element);
                        element->extended = (void*) &hrElement;

                        hrElement.draw();

                        elements->insert({element->id, element});

                        return element->id;
                    }
                    break;

                    case GUMBO_TAG_LI:
                    {
                        auto element = std::make_shared<HTMLElement>(ctx);
                        element->setParent(parent);
                        element->setPreviousSibling(previous);
                        auto attributes = std::make_shared<GumboVector>(node->v.element.attributes);
                        element->setAttributes(attributes);

                        auto textNode = static_cast<GumboNode*>(node->v.element.children.data[0]);

                        if(textNode->type == GUMBO_NODE_TEXT) {
                            std::string txt(textNode->v.text.text);
                            std::wstring text(txt.begin(), txt.end());

                            HTMLLI liElement(ctx, element);
                            element->extended = (void*) &liElement;

                            liElement.setText(&text);
                            liElement.draw();

                            elements->insert({element->id, element});
                        }

                        return element->id;
                    }
                    break;

                    case GUMBO_TAG_UL:
                    {
                        printf("UL\n");
                        auto element = std::make_shared<HTMLElement>(ctx);
                        element->type = GUMBO_TAG_UL;
                        element->setParent(parent);
                        element->setPreviousSibling(previous);
                        auto attributes = std::make_shared<GumboVector>(node->v.element.attributes);
                        element->setAttributes(attributes);
                        element->display();

                        HTMLUL ulElement(ctx, element);
                        element->extended = (void*) &ulElement;

                        ulElement.draw();

                        elements->insert({element->id, element});

                        auto children = new GumboVector(node->v.element.children);
                        ELEMENT_KEY last = previous;
                        if (node->v.element.children.length > 0) {
                            for (int i = 0; i < children->length; i += 1) {
                                ELEMENT_KEY newLast = handle_dom(
                                    ctx,
                                    static_cast<GumboNode*>(children->data[i]),
                                    element->id,
                                    last);

                                if (newLast) element->appendElement(newLast);
                                if (newLast && i < children->length ) last = newLast;
                            }
                        }

                        return element->id;
                    }
                    break;

                    case GUMBO_TAG_BR:
                    {
                        auto element = std::make_shared<HTMLElement>(ctx);
                        element->setSize(500, 18);
                        element->setParent(parent);
                        element->setPreviousSibling(previous);
                        auto attributes = std::make_shared<GumboVector>(node->v.element.attributes);
                        element->setAttributes(attributes);
                        element->display();

                        elements->insert({element->id, element});

                        return element->id;
                    }
                    break;

                    case GUMBO_TAG_H1:
                    {
                        auto element = std::make_shared<HTMLElement>(ctx);
                        HTMLH1 hElement(ctx, element);

                        element->type = GUMBO_TAG_H1;
                        element->extended = (void*) &hElement;
                        element->previousSibling = previous;
                        auto attributes = std::make_shared<GumboVector>(node->v.element.attributes);
                        element->setAttributes(attributes);

                        auto textNode = static_cast<GumboNode*>(node->v.element.children.data[0]);

                        if(textNode->type == GUMBO_NODE_TEXT) {
                            std::string text(textNode->v.text.text);

                            element->setParent(parent);
                            hElement.setText(&text);
                            hElement.draw();

                            elements->insert({element->id, element});
                        }

                        return element->id;
                    }
                    break;

                    case GUMBO_TAG_H2:
                    {
                        auto element = std::make_shared<HTMLElement>(ctx);
                        HTMLH2 hElement(ctx, element);

                        element->type = GUMBO_TAG_H2;
                        element->extended = (void*) &hElement;
                        element->previousSibling = previous;
                        auto attributes = std::make_shared<GumboVector>(node->v.element.attributes);
                        element->setAttributes(attributes);

                        auto textNode = static_cast<GumboNode*>(node->v.element.children.data[0]);

                        if(textNode->type == GUMBO_NODE_TEXT) {
                            std::string text(textNode->v.text.text);

                            element->setParent(parent);
                            hElement.setText(&text);
                            hElement.draw();

                            elements->insert({element->id, element});
                        }

                        return element->id;
                    }
                    break;

                    case GUMBO_TAG_H3:
                    {
                        auto element = std::make_shared<HTMLElement>(ctx);
                        HTMLH3 hElement(ctx, element);

                        element->type = GUMBO_TAG_H3;
                        element->extended = (void*) &hElement;
                        element->previousSibling = previous;
                        auto attributes = std::make_shared<GumboVector>(node->v.element.attributes);
                        element->setAttributes(attributes);

                        auto textNode = static_cast<GumboNode*>(node->v.element.children.data[0]);

                        if(textNode->type == GUMBO_NODE_TEXT) {
                            std::string text(textNode->v.text.text);

                            element->setParent(parent);
                            hElement.setText(&text);
                            hElement.draw();

                            elements->insert({element->id, element});
                        }

                        return element->id;
                    }
                    break;

                    case GUMBO_TAG_H4:
                    {
                        auto element = std::make_shared<HTMLElement>(ctx);
                        HTMLH4 hElement(ctx, element);

                        element->type = GUMBO_TAG_H4;
                        element->extended = (void*) &hElement;
                        element->previousSibling = previous;
                        auto attributes = std::make_shared<GumboVector>(node->v.element.attributes);
                        element->setAttributes(attributes);

                        auto textNode = static_cast<GumboNode*>(node->v.element.children.data[0]);

                        if(textNode->type == GUMBO_NODE_TEXT) {
                            std::string text(textNode->v.text.text);

                            element->setParent(parent);
                            hElement.setText(&text);
                            hElement.draw();

                            elements->insert({element->id, element});
                        }

                        return element->id;
                    }
                    break;

                    case GUMBO_TAG_H5:
                    {
                        auto element = std::make_shared<HTMLElement>(ctx);
                        HTMLH5 hElement(ctx, element);

                        element->type = GUMBO_TAG_H5;
                        element->extended = (void*) &hElement;
                        element->previousSibling = previous;
                        auto attributes = std::make_shared<GumboVector>(node->v.element.attributes);
                        element->setAttributes(attributes);

                        auto textNode = static_cast<GumboNode*>(node->v.element.children.data[0]);

                        if(textNode->type == GUMBO_NODE_TEXT) {
                            std::string text(textNode->v.text.text);

                            element->setParent(parent);
                            hElement.setText(&text);
                            hElement.draw();

                            elements->insert({element->id, element});
                        }

                        return element->id;
                    }
                    break;

                    case GUMBO_TAG_H6:
                    {
                        auto element = std::make_shared<HTMLElement>(ctx);
                        HTMLH6 hElement(ctx, element);

                        element->type = GUMBO_TAG_H6;
                        element->extended = (void*) &hElement;
                        element->previousSibling = previous;
                        auto attributes = std::make_shared<GumboVector>(node->v.element.attributes);
                        element->setAttributes(attributes);

                        auto textNode = static_cast<GumboNode*>(node->v.element.children.data[0]);

                        if(textNode->type == GUMBO_NODE_TEXT) {
                            std::string text(textNode->v.text.text);

                            element->setParent(parent);
                            hElement.setText(&text);
                            hElement.draw();

                            elements->insert({element->id, element});
                        }

                        return element->id;
                    }
                    break;

                    case GUMBO_TAG_HTML:
                    case GUMBO_TAG_HEAD:
                    {
                        auto children = new GumboVector(node->v.element.children);

                        ELEMENT_KEY last = 0;
                        if (node->v.element.children.length > 0) {
                            for (int i = 0; i < children->length; i += 1) {
                                last = handle_dom(ctx, static_cast<GumboNode*>(children->data[i]), 0, last);
                            }
                        }
                    }
                    break;

                    case GUMBO_TAG_BODY:
                    {
                        auto element = std::make_shared<HTMLElement>(ctx);
                        HTMLBody bodyElement(ctx, element);

                        element->type = GUMBO_TAG_BODY;
                        element->extended = (void*) &bodyElement;
                        element->previousSibling = previous;
                        auto attributes = std::make_shared<GumboVector>(node->v.element.attributes);
                        element->setAttributes(attributes);
                        element->setParent(parent);

                        bodyElement.draw();

                        elements->insert({element->id, element});

                        auto children = new GumboVector(node->v.element.children);

                        ELEMENT_KEY last = previous;
                        if (node->v.element.children.length > 0) {
                            for (int i = 0; i < children->length; i += 1) {
                                ELEMENT_KEY newLast = handle_dom(
                                    ctx,
                                    static_cast<GumboNode*>(children->data[i]),
                                    element->id,
                                    last);

                                if (newLast && i < children->length ) last = newLast;
                            }
                        }
                    }
                    break;

                    case GUMBO_TAG_IMG:
                    case GUMBO_TAG_IMAGE:
                    {
                        printf("Found image\n");
                        GumboAttribute* attr = find_attribute(node, "src");

                        Tracking* tracking = (Tracking*) calloc(1, sizeof(Tracking));
                        tracking->parent = parent;
                        tracking->previousSibling = previous;
                        tracking->node = node;

                        fetch_data(
                            ctx,
                            std::string(HOST) + std::string(attr->value),
                            (void*) tracking,
                            static_cast<READ_CALLBACK>([](Context* ctx, char* buffer, size_t size, void* userData) {
                            auto element = std::make_shared<HTMLElement>(ctx);
                            HTMLImage htmlImage(ctx, element);

                            Tracking* tracking = (Tracking*) userData;

                            element->type = GUMBO_TAG_IMG;
                            element->extended = (void*) &htmlImage;
                            element->previousSibling = tracking->previousSibling;
                            element->setParent(tracking->parent);
                            auto attributes = std::make_shared<GumboVector>(tracking->node->v.element.attributes);
                            element->setAttributes(attributes);

                            htmlImage.setImageData(buffer, size);
                            htmlImage.display();

                            std::map<ELEMENT_KEY,std::shared_ptr<HTMLElement>>* elements = (std::map<ELEMENT_KEY,std::shared_ptr<HTMLElement>>*) ctx->elements;

                            elements->insert({element->id, element});
                        }));
                    }
                    break;

                    default:
                    break;
                }
            }
            break;

        default:
            return 0;
    }

    return 0;
}

int main(int argc, char** argv) {
    auto mode = sf::VideoMode::getDesktopMode();

    gtk_init(&argc, &argv);

    int columns = 2;
    GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    // GtkWidget *list = gtk_tree_view_new();
    GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
    // gtk_box_pack_start(GTK_BOX(vbox), list, TRUE, TRUE, 2);
    gtk_window_set_title(GTK_WINDOW(win), "Browser - Network Stats");
    gtk_window_set_default_size(GTK_WINDOW(win), 500, 300);
    // gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(list), TRUE);

    TreeView tv;
    tv.addColumn("Method", G_TYPE_STRING);
    tv.addColumn("Host", G_TYPE_STRING);
    tv.addColumn("IP", G_TYPE_STRING);
    tv.addColumn("Path", G_TYPE_STRING);
    tv.addColumn("Status", G_TYPE_STRING);
    tv.addColumn("Type", G_TYPE_STRING);
    tv.addColumn("Size", G_TYPE_STRING);
    int rowId = 0; //tv.createRow();

    std::vector<std::string> data0;
    data0.push_back(std::string("GET"));
    data0.push_back(std::string("www.theage.com"));
    data0.push_back(std::string("10.12.3.4"));
    data0.push_back(std::string("/article/21"));
    data0.push_back(std::string("200 OK"));
    data0.push_back(std::string("text/html"));
    data0.push_back(std::string("14975"));

    std::vector<std::string> data1;
    data1.push_back(std::string("GET"));
    data1.push_back(std::string("www.theage.com"));
    data1.push_back(std::string("10.12.3.4"));
    data1.push_back(std::string("cats.png"));
    data1.push_back(std::string("200 OK"));
    data1.push_back(std::string("image/png"));
    data1.push_back(std::string("339399"));

    tv.insertRow(data0);
    tv.insertRow(data1);
    tv.update();
    tv.attachToVbox(vbox);

    gtk_container_add(GTK_CONTAINER(win), vbox);
    gtk_widget_show_all(win);

    // gtk_main();

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

    fetch_data(ctx, HOST, &elements, static_cast<READ_CALLBACK>([](Context* ctx, char* buffer, size_t size, void* userData) {
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
}