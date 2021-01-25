#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <stdio.h>
#include <strings.h>
#include <functional>
#include <gumbo.h>
#include <uv.h>

#include "main.h"
#include "fetch.h"
#include "context.h"
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

void handle_dom(Context* ctx, GumboNode* node) {
    switch (node->type) {
        case GUMBO_NODE_ELEMENT:
            {
                switch (node->v.element.tag) {
                    case GUMBO_TAG_H1:
                    {
                        HTMLH1 h1Element(ctx);

                        auto textNode = static_cast<GumboNode*>(node->v.element.children.data[0]);

                        if(textNode->type == GUMBO_NODE_TEXT) {
                            std::string text(textNode->v.text.text);

                            h1Element.setText(&text);
                            h1Element.draw();
                        }
                    }
                    break;

                    case GUMBO_TAG_HTML:
                    case GUMBO_TAG_HEAD:
                    case GUMBO_TAG_BODY:
                    {
                        if (node->v.element.tag == GUMBO_TAG_BODY) {
                            HTMLBody bodyElement(ctx);
                            bodyElement.draw();
                        }

                        auto children = new GumboVector(node->v.element.children);
                        printf("No of children is %d\n", children->length);

                        if (node->v.element.children.length > 0) {
                            for (int i = 0; i < children->length; i += 1) {
                                handle_dom(ctx, static_cast<GumboNode*>(children->data[i]));
                            }
                        }
                    }
                    break;

                    case GUMBO_TAG_IMG:
                    case GUMBO_TAG_IMAGE:
                    {
                        printf("Found image\n");
                        GumboAttribute* attr = find_attribute(node, "src");

                        fetch_data(ctx, strcat(HOST, attr->value), static_cast<READ_CALLBACK>([](Context* ctx, char* buffer, size_t size) {
                            HTMLImage htmlImage(ctx);
                            htmlImage.setImageData(buffer, size);
                            // htmlImage.display();
                        }));
                    }
                    break;

                    default:
                    break;
                }
            }
            break;

        default:
            return;
    }
}

int main(int argc, char** argv) {
    // auto mode = sf::VideoMode::getDesktopMode();
    Mode mode;
    mode.width = 1024;
    mode.height = 768;
    sf::RenderWindow window(sf::VideoMode(mode.width, mode.height), "Browser Window"); //, sf::Style::Fullscreen);
    Context* ctx = (Context*) malloc(sizeof(Context));
    ctx->mode = &mode;
    ctx->window = &window;
    ctx->window->clear(sf::Color::White);
    fetch_data(ctx, HOST, static_cast<READ_CALLBACK>([](Context* ctx, char* buffer, size_t size) {
        handle_dom(ctx, gumbo_parse(buffer)->root);
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