#include "../httplib.h"
#include "../response/response.h"
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
    resp->code = 200;
    printf("%s\n", req.uri);

    return;
}

int main() {
    setup();
    char route2[] = "GET /test/something/something_else";
    char echo_route[] = "GET /echo";
    register_route(route2, *test_callback_1);
    register_route(echo_route, *echo_headers);

    create_server();

    destroy_tree(&root);

    return 0;
}
