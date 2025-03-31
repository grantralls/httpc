#include "httplib/response.h"
#include <stdio.h>

void unparse_response(response* resp, char* buffer) {
    sprintf(buffer, "HTTP/1.0 %d OK\r\n%s\r\n%s", resp->code, ll_create_headers(resp->headers, 0), resp->body);
    return;
}
