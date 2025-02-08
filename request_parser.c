#include "request_parser.h"
#include <string.h>

/*
 This function will copy the entire route found in the first line of a request. Beginning at "/" and ending at the first whitespace encountered.
 It is the responsibility of the consumer to null terminate the buffer
*/
int parse_route_from_request(char request[], char buffer[], size_t buf_size) {
    char* start = request;
    int length = 1;
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

