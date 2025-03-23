/**
 * @file tree.h
 */

#ifndef URI_TREE
#define URI_TREE

#include "../request_parser/request_parser.h"
#include "../response/response.h"
typedef void (*callback_t)(request, response*);

/**
 * # Conceptual
 * If we imagine A single URI is a linked list of nodes, then every registered URI on the server is an N-ary tree of nodes. This N-ary tree is how I've implemented URIs in this library.
 *
 * # Example
 * In the example below, some will have a child of silly, and silly will have a *sibling* of goofy. Both silly and goofy have their own children of uri (Note how every node will only point to one other node as "children", even though conceptually there can be many children via siblings). Assume every node has a callback, then the following endpoints will be served.
 *
 * 1. /some
 * 2. /some/silly
 * 3. /some/silly/uri
 * 4. /some/goofy
 * 5. /some/goofy/uri
 *
 * __Legend:__ \n 
 * --> : left's "children" points to right \n 
 *  | \n 
 *  v : top's "siblings" points to bottom \n 
 * ```
 * some --> silly --> uri
 *           |
 *           v
 *          goofy --> uri
 * ```
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
node* trace_tree(char route[], node* origin_node);
/**
 * @brief returns the node of the last segment in the URI. If a complete trace isn't found, return null.
 */
node* trace_tree_exact(char route[], node* root);
/**
 * @brief Given a root node, deallocate the entire tree
 */
void destroy_tree(node* n);
/**
 * @brief Given a root node, print the entire tree in a DFS manner, along with the level of each node.
 */
void print_tree(node* n, int level);
#endif
