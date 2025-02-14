#include "response.h"
#include <stdio.h>

void unparse_response(response* resp, char* buffer) {
    sprintf(buffer, "HTTP/1.0 %d \r\n\r\n", resp->code);
    return;
}
