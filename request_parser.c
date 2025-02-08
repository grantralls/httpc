#include "request_parser.h"
#include <stdio.h>
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

void create_request(char request_buffer[], request* req) {
    char* line = strtok(request_buffer, "\r\n");
    char* request_line = line;
    line = strtok(NULL, "\r\n");

    // handle headers
    while(line != NULL) {
        printf("header: %s\n", line);
        line = strtok(NULL, "\r\n");
    }

    req->uri = request_line;
    // handle the request line
    char* request_line_item = strtok(request_line, " ");
    while(request_line_item != NULL) {
        request_line_item = strtok(NULL, " ");
    }

}
