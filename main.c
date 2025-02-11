#include "httplib.h"
#include <stdio.h>

char* test_callback_1(request req) {
    ll_node* header = req.headers;
    while(header != NULL) {
        printf("key: %s\n", header->key);
        printf("val: %s\n", header->value);
        header = header->next;
    }
    printf("req uri: %s\n", req.uri);
    return "HTTP/1.0 200 \r\n\r\n<h1>IT'S ALIVE</h1>";
}


int main() {
    setup();
    char route2[] = "GET /test/something/something_else";
    register_route(route2, *test_callback_1);

    create_server();

    destroy_tree(&root);

    return 0;
}
