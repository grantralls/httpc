#ifndef HTTP_LIB
#define HTTP_LIB

/**
 * @file httplib.h
 */

#include "internal/request_parser.h"
#include "internal/response.h"
#include "internal/tree.h"

enum ERROR_CODES { RR_DUPLICATE_ROUTE, RR_OK };

extern node get_root;
extern node post_root;

/**
 * @brief add an endpoint for a "GET" request at the given endpoint
 */
int get(char route[], callback_t callback);
int post(char route[], callback_t callback);

/**
 * @brief adds a route to the URI tree
 * @param route an HTTP verb with a route, separated by a space. (eg. "GET /some/complete/route")
 * @param callback the function to be ran when a request comes in for the given route
 * @param root_node the root in which to add this new node, typically either the get root or post root
 * @return one of @ref ERROR_CODES
 *
 * @internal
 */
int register_route(char route[], callback_t callback, node* root_node);
/**
 * TODO: Create the server
 */
int create_server(void);
/**
 * @brief this needs to be ran before registering any routes.
 */
void setup(void);

#endif
