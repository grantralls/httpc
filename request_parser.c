#include "request_parser.h"
#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 This function will copy the entire route found in the first line of a request. Beginning at "/" and ending at the first whitespace encountered.
 It is the responsibility of the consumer to null terminate the buffer
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

methods get_method(char* method) {
    if(!strncmp(method, "GET", 3)) return GET;
    if(!strncmp(method, "POST", 4)) return POST;
    if(!strncmp(method, "HEAD", 4)) return HEAD;
    return GET;
}

/**
 This function is the first step to create a request out of a raw buffer that comes from the socket.
*/
void create_request(char request_buffer[], request* req) {
    char* save_ptr2;
    char* save_ptr1;
    char* line = strtok_r(request_buffer, "\r\n", &save_ptr2);
    char* request_line = line;
    line = strtok_r(NULL, "\r\n", &save_ptr2);
    ll_node root;
    ll_node* last = &root;

    // handle headers
    while(line != NULL) {
        // add line
        ll_node* next = malloc(sizeof(ll_node));
        char* key = strtok_r(line, ": ", &save_ptr1);
        char* val = strtok_r(NULL, ": ", &save_ptr1);
        next->key = key;
        next->value = val;
        last->next = next;
        last = next;
        line = strtok_r(NULL, "\r\n", &save_ptr2);
    }

    req->headers = root.next;
    char* method = strtok(request_line, " ");
    req->method = get_method(method);
    char* uri = strtok(NULL, " ");
    req->uri = uri;

    char* request_line_item = strtok(NULL, " ");
    while(request_line_item != NULL) {
        request_line_item = strtok(NULL, " ");
    }
}
