#include "httplib.h"
#include <stdio.h>

void test_callback_1(void);
void test_callback_2(void);

void test_callback_1(void) {
    printf("HELLO THERE!\n");
}

void test_callback_2(void) {
    printf("HELLO THERE AGAIN!\n");
}

int main(int argc, char const* argv[]) {
    setup();
    char route2[] = "GET /test/something/something_else";
    char route3[] = "GET /test/something";
    register_route(route2, *test_callback_1);
    register_route(route3, *test_callback_2);
    print_tree(&root);
    destroy_tree(&root);

    return 0;
}
