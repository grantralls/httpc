/**
 * @file response.h
 */

#ifndef HTTPLIB_RESPONSE
#define HTTPLIB_RESPONSE

#include "linkedlist.h"

#define MAX_RESPONSE_BODY 4096

typedef struct response {
    char body[MAX_RESPONSE_BODY];
    struct ll_node* headers;
    int code;
} response;

/**
 * @brief transform a request struct into a valid html response using the buffer
 */
void unparse_response(response* resp, char* headers, char* buffer);

#endif
