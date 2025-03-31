#include "httplib/httplib.h"
#include "httplib/request_parser.h"
#include "httplib/response.h"

void test_callback(request req, response* resp) {
    (void)req;
    resp->code = 200;
    resp->body = "Hello there!\n";
    return;
}

int main() {
    setup();

    char* route = "/greeting";
    get(route, &test_callback);

    create_server();
}
