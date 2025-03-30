#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "request_parser.h"

int main() {
    char* header_save = NULL;
    char input[] = "Authorization: Bearer token";
    ll_node* node = get_header(input, &header_save);
    printf("%s\n", node->value);

    if(!strcmp(node->value, "Bearer token") && !strcmp(node->key, "Authorization")) {
        return 0;
    } else {
        return 1;
    }
}
