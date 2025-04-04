#include "httplib/request_parser.h"
#include "httplib/linkedlist.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
 * This function will copy the entire route found in the first line of a request. Beginning at "/" and ending at the first whitespace encountered.
 * It is the responsibility of the consumer to null terminate the buffer.
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
ll_node* get_header(char* header_line, char** header_save) {
    ll_node* next = malloc(sizeof(ll_node));
    if(next == NULL) {
        return NULL;
    }
    next->next = NULL;
    next->key = NULL;
    next->value = NULL;

    char* key = strtok_r(header_line, ": ", header_save);

    while(**header_save == ' ') {
        (*header_save)++;
    }

    // Malformed request
    if(key == NULL) {
        return NULL;
    }
    next->key = key;
    next->value = *header_save;
    return next;
}
/**
 * Given the first header line, the headers root, and the line save pointer, add each header to the headers linked list
 */
int get_headers(char headers_buffer[], request* req) {
    ll_node root;
    if(headers_buffer == NULL) {
        return -1;
    }
    ll_node* last = &root;
    char* header_save = NULL;
    char* line_save = NULL;
    char* header_line = strtok_r(headers_buffer, "\r\n", &line_save);

    // handle headers
    while(header_line != NULL) {
        // add line
        ll_node* next = get_header(header_line, &header_save);
        if(next == NULL) {
            return -1;
        }
        last->next = next;
        last = next;
        header_line = strtok_r(NULL, "\r\n", &line_save);
    }
    req->headers = root.next;
    return 0;
}
/**
 * Given an [http request line](https://www.rfc-editor.org/rfc/rfc1945#section-5.1), break out the method & uri, then add it to the request struct. Use headers_buffer as a save pointer. This will make it so it points to the headers, as they come right after the first "\r\n".
 * @internal
 */
int get_request_line(char** headers_buffer, char request_buffer[], request* req) {
    char* request_line = strtok_r(request_buffer, "\r\n", headers_buffer);
    if(request_line == NULL) {
        return -1;
    }
    char* save_ptr = NULL;
    char* method = strtok_r(request_line, " ", &save_ptr);
    if(method == NULL) {
        return -1;
    }
    req->method = get_method(method);
    char* uri = strtok_r(NULL, " ", &save_ptr);
    if(uri == NULL) {
        return -1;
    }
    req->uri = uri;
    return 0;
}
/**
 * This function is the first step to create a request out of a raw buffer that comes from the socket.
 */
int create_request(char request_buffer[], request* req) {
    assert(req != NULL);
    assert(strlen(request_buffer) > 0);

    char* body = strstr(request_buffer, "\r\n\r\n");
    if(body == NULL) {
        return -1;
    }
    // this null terminator will stop the header strtok scan from going into the body section
    *body = '\0';

    char* headers_buffer = NULL;
    if(get_request_line(&headers_buffer, request_buffer, req) == -1) {
        exit(EXIT_FAILURE);
        return -1;
    }
    if(get_headers(headers_buffer, req) == -1) {
        exit(EXIT_FAILURE);
        return -1;
    }

    return 0;
}
