#ifndef HTTP_LIB

typedef void (*callback_t)(void);

/**
 * URIs are represented as a n-ary tree of nodes. Where the val in each node is a URI .
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
 * @brief gets the leaf-most node given some route
 * @param route a complete route (eg. /some/complete/route)
 */
node* find_leaf_node_by_route(char route[]);
/**
 * @brief adds a route to the URI tree
 * @param route an HTTP verb with a route, separated by a space. (eg. "GET /some/complete/route")
 * @param callback the function to be ran when a request comes in for the given route
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

void setup(void);

#endif
