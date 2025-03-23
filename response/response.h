/**
 * @file response.h
 */

#ifndef HTTPLIB_RESPONSE
#define HTTPLIB_RESPONSE

#include "../linkedlist/linkedlist.h"

typedef struct response {
    char* body;
    struct ll_node* headers;
    int code;
} response;

/**
 * @brief transform a request struct into a valid html response using the buffer
 */
void unparse_response(response* resp, char* buffer);

#endif
