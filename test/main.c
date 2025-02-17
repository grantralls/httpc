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
        puts("FAILED ALLOCATION");
    }
    strncpy(body, contents, strlen(contents));
    resp->body = body;

    return;
}

int main() {
    setup();
    char route2[] = "GET /test/something/something_else";
    register_route(route2, *test_callback_1);

    create_server();

    destroy_tree(&root);

    return 0;
}
