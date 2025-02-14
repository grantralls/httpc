#ifndef HTTPLIB_RESPONSE
#define HTTPLIB_RESPONSE

#include "linkedlist.h"

typedef struct response {
    char* body;
    ll_node* headers;
    int code;
} response;

void unparse_response(response* resp, char* buffer);

#endif
