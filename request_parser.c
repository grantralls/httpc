#include "request_parser.h"
#include "linkedlist.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
 * This function will copy the entire route found in the first line of a request. Beginning at "/" and ending at the first whitespace encountered.
 * It is the responsibility of the consumer to null terminate the buffer.
 *
 * @internal
 */
int parse_route_from_request(char request[], char buffer[], size_t buf_size) {
    char* start = request;
    size_t length = 1;
    while(*start != '/') {
        start++;
    }
    while(*(start + length) != ' ') {
        length++;
    }
    if(buf_size > length + 1) {
        strncpy(buffer, start, length);
        return length;
    }
    return 0;
}
/**
 * Given an HTTP/1.0 method in the form of a char string, convert it into a @ref methods enum value
 *
 * @internal
 */
methods get_method(char* method) {
    assert(method != NULL);
    if(!strncmp(method, "GET", 3)) return GET;
    if(!strncmp(method, "POST", 4)) return POST;
    if(!strncmp(method, "HEAD", 4)) return HEAD;
    // TODO: instead of just asserting, I should send back a 414
    assert("invalid method from the request");
    return GET;
}
/**
 * Given the first header line, the headers root, and the line save pointer, add each header to the headers linked list
 *
 * @internal
 * @return -1 if there was some error, 0 if no error was encountered
 *
 */
int get_headers(char header_buffer[], request* req) {
    ll_node root;
    if(header_buffer == NULL) {
        return -1;
    }
    ll_node* last = &root;
    char* header_save = NULL;
    char* line_save = NULL;
    char* line = strtok_r(header_buffer, "\r\n", &line_save);

    // handle headers
    while(line != NULL) {
        // add line
        ll_node* next = malloc(sizeof(ll_node));
        if(next == NULL) {
            return -1;
        }

        char* key = strtok_r(line, ": ", &header_save);
        // Malformed request
        if(key == NULL) {
            return -1;
        }

        char* val = strtok_r(NULL, ": ", &header_save);
        // Malformed request
        if(val == NULL) {
            return -1;
        }
        next->key = key;
        next->value = val;
        last->next = next;
        last = next;
        line = strtok_r(NULL, "\r\n", &line_save);
    }
    req->headers = root.next;
    return 0;
}
/**
 * Given an [http request line](https://www.rfc-editor.org/rfc/rfc1945#section-5.1), break out the method & uri, then add it to the request struct.
 *
 * @internal
 */
int get_request_line(char** s_headers_buffer, char request_buffer[], request* req) {
    char* request_line = strtok_r(request_buffer, "\r\n", s_headers_buffer);
    if(request_line == NULL) {
        return -1;
    }
    char* method = strtok(request_line, " ");
    if(method == NULL) {
        return -1;
    }
    req->method = get_method(method);
    char* uri = strtok(NULL, " ");
    if(uri == NULL) {
        return -1;
    }
    req->uri = uri;
    return 0;
}
/**
 * This function is the first step to create a request out of a raw buffer that comes from the socket.
 *
 * @internal
 * @return -1 if there was an error, 0 if there wasn't
 *
 * TODO: this function could properly be simplified
 */
int create_request(char request_buffer[], request* req) {
    char* body = strstr(request_buffer, "\r\n\r\n");
    if(body == NULL) {
        return -1;
    }
    // this null terminator will stop the header strtok scan from going into the body section
    *body = '\0';

    assert(req != NULL);
    char* headers_buffer = NULL;
    if(get_request_line(&headers_buffer, request_buffer, req) == -1) {
        return -1;
    }
    if(get_headers(headers_buffer, req) == -1) {
        return -1;
    }

    return 0;
}
