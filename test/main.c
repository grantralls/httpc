#include "../httplib.h"
#include "../internal/response.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_callback_1(request req, response* resp) {
    printf("%s\n", req.uri);
    resp->code = 200;
    char* contents = "Hello there from the body!!!!";
    char* body = calloc(strlen(contents) + 1, sizeof(char));
    if(body == NULL) {
        resp->code = 500;
        return;
    }
    strncpy(body, contents, strlen(contents));
    resp->body = body;

    return;
}

void echo_headers(request req, response* resp) {
    ll_node* header = malloc(sizeof(ll_node));
    header->key = "Content-Type";
    header->value = "text/html";

    header->next = ll_clone(req.headers);

    char* contents = "<h1>Some Content</h1>";
    char* body = calloc(strlen(contents) + 1, sizeof(char));
    strncpy(body, contents, strlen(contents));

    resp->body = body;
    resp->headers = header;
    resp->code = 200;

    printf("%s\n", req.uri);

    return;
}

int main() {
    setup();
    char route2[] = "/test/something/something_else";
    char echo_route[] = "/echo";
    char post_route[] = "/post_route";

    get(route2, *test_callback_1);
    get(echo_route, *echo_headers);
    post(post_route, *test_callback_1);

    create_server();
    destroy_tree(&get_root);

    return 0;
}
