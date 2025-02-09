#ifndef HTTPLIB_REQUEST_PARSER

/**
 * @file request_parser.h
 */

#define HTTPLIB_REQUEST_PARSER
#include <stddef.h>

typedef struct header {
    char* key;
    char* value;
    struct header* next;
} header;
typedef enum methods { GET, POST, HEAD } methods;
typedef struct request {
    char* uri;
    struct header* headers;
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
void create_request(char request_buffer[], request* req);
#endif
