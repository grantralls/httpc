#include "httplib/httplib.h"
#include "httplib/request_parser.h"
#include "httplib/response.h"
#include <string.h>

void test_callback(request req, response* resp) {
    (void)req;
    resp->code = 200;
    strcpy(resp->body, "Hello there!\n");
    return;
}

int main() {
    // always call setup, this instantiates the uri tree
    setup();

    // the same callback will serve the same route on both post and get
    char* route = "/get_greeting";
    get(route, &test_callback);
    post(route, &test_callback);

    // always call http_listen to start the server
    http_listen();
}
