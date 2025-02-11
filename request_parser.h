#ifndef HTTPLIB_REQUEST_PARSER

/**
 * @file request_parser.h
 */

#define HTTPLIB_REQUEST_PARSER
#include "linkedlist.h"
#include <stddef.h>
typedef enum methods { GET, POST, HEAD } methods;
typedef struct request {
    char* uri;
    struct ll_node* headers;
    methods method;
} request;
/**
 * @brief parse a route from a request
 * @param request the request line of the HTTP request
 * @param buffer the destination buffer to put the route
 * @param buf_size the size (in bytes) of the buffer
 * @return number of bytes written. 0 if the route is bigger than the buffer
 */
int parse_route_from_request(char request[], char buffer[], size_t buf_size);
/**
 * @brief given a request buffer, populate the request struct
 * @param request_buffer the buffer to use as a source
 * @param req the request struct to populate
 * @return -1 if there was an error, 0 if there wasn't
 */
int create_request(char request_buffer[], request* req);
#endif
