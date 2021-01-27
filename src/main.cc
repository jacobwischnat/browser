#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <functional>
#include <strings.h>
#include <stdio.h>
#include <gumbo.h>
#include <uv.h>

#include "main.h"
#include "fetch.h"
#include "types.h"
#include "html/html.h"

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
    printf("parent %lu previous %lu\n", parent, previous);
    switch (node->type) {
        case GUMBO_NODE_ELEMENT:
            {
                switch (node->v.element.tag) {
                    case GUMBO_TAG_H1:
                    {
                        HTMLElement element(ctx);
                        HTMLH1 hElement(ctx, &element);

                        element.type = GUMBO_TAG_H1;
                        element.extended = (void*) &hElement;
                        element.previousSibling = previous;

                        auto textNode = static_cast<GumboNode*>(node->v.element.children.data[0]);

                        if(textNode->type == GUMBO_NODE_TEXT) {
                            std::string stype("HTMLH1");
                            std::string text(textNode->v.text.text);

                            element.setParent(parent);
                            hElement.setText(&text);
                            hElement.draw();

                            ctx->elements->insert(std::pair<ELEMENT_KEY,void*>(element.id,(void*) &element));
                        }

                        return element.id;
                    }
                    break;

                    case GUMBO_TAG_H2:
                    {
                        HTMLElement element(ctx);
                        HTMLH2 hElement(ctx, &element);

                        element.type = GUMBO_TAG_H2;
                        element.extended = (void*) &hElement;
                        element.previousSibling = previous;

                        auto textNode = static_cast<GumboNode*>(node->v.element.children.data[0]);

                        if(textNode->type == GUMBO_NODE_TEXT) {
                            std::string stype("HTMLH2");
                            std::string text(textNode->v.text.text);

                            element.setParent(parent);
                            hElement.setText(&text);
                            hElement.draw();

                            ctx->elements->insert(std::pair<ELEMENT_KEY,void*>(element.id,(void*) &element));
                        }

                        return element.id;
                    }
                    break;

                    case GUMBO_TAG_H3:
                    {
                        HTMLElement element(ctx);
                        HTMLH3 hElement(ctx, &element);

                        element.type = GUMBO_TAG_H3;
                        element.extended = (void*) &hElement;
                        element.previousSibling = previous;

                        auto textNode = static_cast<GumboNode*>(node->v.element.children.data[0]);

                        if(textNode->type == GUMBO_NODE_TEXT) {
                            std::string stype("HTMLH3");
                            printf("%s\n", stype.c_str());
                            std::string text(textNode->v.text.text);

                            element.setParent(parent);
                            hElement.setText(&text);
                            hElement.draw();

                            ctx->elements->insert(std::pair<ELEMENT_KEY,void*>(element.id,(void*) &element));
                        }

                        return element.id;
                    }
                    break;

                    case GUMBO_TAG_H4:
                    {
                        HTMLElement element(ctx);
                        HTMLH4 hElement(ctx, &element);

                        element.type = GUMBO_TAG_H4;
                        element.extended = (void*) &hElement;
                        element.previousSibling = previous;

                        auto textNode = static_cast<GumboNode*>(node->v.element.children.data[0]);

                        if(textNode->type == GUMBO_NODE_TEXT) {
                            std::string stype("HTMLH4");
                            printf("%s\n", stype.c_str());
                            std::string text(textNode->v.text.text);

                            element.setParent(parent);
                            hElement.setText(&text);
                            hElement.draw();

                            ctx->elements->insert(std::pair<ELEMENT_KEY,void*>(element.id,(void*) &element));
                        }

                        return element.id;
                    }
                    break;

                    case GUMBO_TAG_H5:
                    {
                        HTMLElement element(ctx);
                        HTMLH5 hElement(ctx, &element);

                        element.type = GUMBO_TAG_H5;
                        element.extended = (void*) &hElement;
                        element.previousSibling = previous;

                        auto textNode = static_cast<GumboNode*>(node->v.element.children.data[0]);

                        if(textNode->type == GUMBO_NODE_TEXT) {
                            std::string stype("HTMLH5");
                            printf("%s\n", stype.c_str());
                            std::string text(textNode->v.text.text);

                            element.setParent(parent);
                            hElement.setText(&text);
                            hElement.draw();

                            ctx->elements->insert(std::pair<ELEMENT_KEY,void*>(element.id,(void*) &element));
                        }

                        return element.id;
                    }
                    break;

                    case GUMBO_TAG_H6:
                    {
                        HTMLElement element(ctx);
                        HTMLH6 hElement(ctx, &element);

                        element.type = GUMBO_TAG_H6;
                        element.extended = (void*) &hElement;
                        element.previousSibling = previous;

                        auto textNode = static_cast<GumboNode*>(node->v.element.children.data[0]);

                        if(textNode->type == GUMBO_NODE_TEXT) {
                            std::string stype("HTMLH6");
                            printf("%s\n", stype.c_str());
                            std::string text(textNode->v.text.text);

                            element.setParent(parent);
                            hElement.setText(&text);
                            hElement.draw();

                            ctx->elements->insert(std::pair<ELEMENT_KEY,void*>(element.id,(void*) &element));
                        }

                        return element.id;
                    }
                    break;

                    case GUMBO_TAG_HTML:
                    case GUMBO_TAG_HEAD:
                    {
                        auto children = new GumboVector(node->v.element.children);
                        // printf("No of children is %d\n", children->length);

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
                        std::string stype("HTMLBody");
                        HTMLElement element(ctx);
                        HTMLBody bodyElement(ctx, &element);

                        element.type = GUMBO_TAG_BODY;
                        element.extended = (void*) &bodyElement;
                        element.previousSibling = previous;

                        element.setParent(parent);
                        bodyElement.draw();

                        ctx->elements->insert(std::pair<ELEMENT_KEY,void*>(element.id,(void*) &element));

                        auto children = new GumboVector(node->v.element.children);

                        ELEMENT_KEY last = previous;
                        if (node->v.element.children.length > 0) {
                            for (int i = 0; i < children->length; i += 1) {
                                ELEMENT_KEY newLast = handle_dom(
                                    ctx,
                                    static_cast<GumboNode*>(children->data[i]),
                                    element.id,
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

                        fetch_data(
                            ctx,
                            strcat(HOST, attr->value),
                            (void*) tracking,
                            static_cast<READ_CALLBACK>([](Context* ctx, char* buffer, size_t size, void* userData) {
                            std::string stype("HTMLImage");
                            HTMLElement element(ctx);
                            HTMLImage htmlImage(ctx, &element);

                            Tracking* tracking = (Tracking*) userData;

                            printf("tracking->parent = %lu\n", tracking->parent);
                            printf("tracking->previousSibling = %lu\n", tracking->previousSibling);

                            element.type = GUMBO_TAG_IMG;
                            element.extended = (void*) &htmlImage;
                            element.previousSibling = tracking->previousSibling;
                            element.setParent(tracking->parent);

                            htmlImage.setImageData(buffer, size);
                            htmlImage.display();

                            ctx->elements->insert(std::pair<ELEMENT_KEY,void*>(element.id,(void*) &element));
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
    // auto mode = sf::VideoMode::getDesktopMode();

    Mode mode;
    mode.width = 1024;
    mode.height = 768;

    std::map<ELEMENT_KEY,void*> elements;

    sf::RenderWindow window(sf::VideoMode(mode.width, mode.height), "Browser Window"); //, sf::Style::Fullscreen);

    Context* ctx = (Context*) malloc(sizeof(Context));
    ctx->mode = &mode;
    ctx->window = &window;
    ctx->elements = &elements;
    ctx->window->clear(sf::Color::White);

    fetch_data(ctx, HOST, NULL, static_cast<READ_CALLBACK>([](Context* ctx, char* buffer, size_t size, void* userData) {
        handle_dom(ctx, gumbo_parse(buffer)->root, 0, 0);
        ctx->window->display();
    }));

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
    }

    return 0;
}