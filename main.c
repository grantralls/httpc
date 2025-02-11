#include "httplib.h"
#include <stdio.h>

void test_callback_1(request req) {
    ll_node* header = req.headers;
    while(header != NULL) {
        printf("key: %s\n", header->key);
        printf("val: %s\n", header->value);
        header = header->next;
    }
    printf("req uri: %s\n", req.uri);
}

void test_callback_2(request req) {
    printf("req uri: %s\n", req.uri);
}

int main() {
    setup();
    char route2[] = "GET /test/something/something_else";
    char route3[] = "GET /test/something";
    register_route(route2, *test_callback_1);
    register_route(route3, *test_callback_2);

    create_server();

    destroy_tree(&root);

    return 0;
}
