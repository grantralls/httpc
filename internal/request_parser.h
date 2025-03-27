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
 * @internal
 */
int parse_route_from_request(char request[], char buffer[], size_t buf_size);
/**
 * @brief given a request buffer, populate the request struct
 * @param request_buffer the buffer to use as a source
 * @param req the request struct to populate
 * @return -1 if there was an error, 0 if there wasn't
 * @internal
 */
int create_request(char request_buffer[], request* req);
/**
 * @param header_line a header line in the format "Key: Val"
 * @param header_save a save pointer to track the header lines
 * @return an @ref ll_node pointer representing this header
 */
ll_node* get_header(char* header_line, char** header_save);
/**
 * @param headers_buffer the entire buffer of headers, deliminated by "\r\n"
 * @param req the request struct that these headers will be added to
 * @return -1 if there was some error, 0 if no error was encountered
 */
int get_headers(char headers_buffer[], request* req);
/**
 * @param headers_buffer a save pointer that will point to the beginning of the headers.
 * @param request_buffer the buffer containing the entire request
 * @param req the request struct that will will receive the request line properties
 */
int get_request_line(char** headers_buffer, char request_buffer[], request* req);
#endif
