#ifndef HTTP_LIB
#define HTTP_LIB

/**
 * @file httplib.h
 */

#include "request_parser.h"
#include "response.h"
typedef void (*callback_t)(request, response*);

enum ERROR_CODES { RR_DUPLICATE_ROUTE, RR_OK };

/**
 * If we imagine A single URI is a linked list of nodes, then every registered URI on the server is an N-ary tree of nodes. This N-ary tree is how I've implemented URIs in this library.
 */
typedef struct node {
    /// a URI [fsegment](https://www.rfc-editor.org/rfc/rfc1945#section-3.2.1)
    char* val;
    /// All nodes, whose routes include this node's val in their fsegment
    struct node* children;
    /// All nodes, whose routes include this node's, parent's, val in their fsegment
    struct node* siblings;
    /// The function that will be ran when a request comes
    callback_t callback;
} node;

extern node root;

/**
 * @brief gets the node that matches a given val in a DFS manner
 * @param n the root of the tree to start searching
 * @param val the string to look for
 */
node* find_node_by_val(node* n, char* val);
/**
 * @brief given a route, return the deepest node that exists in the tree that satisfies the route
 * @param route the full route that you wish to find in the tree
 */
node* trace_tree(char route[]);
/**
 * @brief returns the node of the last segment in the URI. If a complete trace isn't found, return null.
 */
node* trace_tree_exact(char route[]);
/**
 * @brief adds a route to the URI tree
 * @param route an HTTP verb with a route, separated by a space. (eg. "GET /some/complete/route")
 * @param callback the function to be ran when a request comes in for the given route
 * @return one of @ref ERROR_CODES
 */
int register_route(char route[], callback_t callback);
/**
 * @brief Given a root node, deallocate the entire tree
 */
void destroy_tree(node* n);
/**
 * @brief Given a root node, print the entire tree in a DFS manner.
 */
void print_tree(node* n, int level);
/**
 * TODO: Create the server
 */
int create_server(void);
/**
 * @brief this needs to be ran before registering any routes.
 */
void setup(void);

#endif
