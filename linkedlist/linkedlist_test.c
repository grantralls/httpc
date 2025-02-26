#include "linkedlist.h"
#include <stdio.h>

int main() {
    ll_node node = {};
    node.key = "Some-key";
    node.value = "some-value";
    ll_node next_node = {};
    next_node.key = "next-key";
    next_node.value = "next-value";
    next_node.next = NULL;

    node.next = &next_node;

    char* headers = create_headers(&node, 0);
    printf("%s\n", headers);
    return 1;
}
