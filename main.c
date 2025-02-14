#include "httplib.h"
#include "response.h"
#include <stdio.h>
#include <stdlib.h>

void test_callback_1(request req, response* resp) {
    ll_node* header = req.headers;
    while(header != NULL) {
        printf("key: %s\n", header->key);
        printf("val: %s\n", header->value);
        header = header->next;
    }
    printf("req uri: %s\n", req.uri);
    resp->code = 200;

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
