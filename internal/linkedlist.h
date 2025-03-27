/**
 * @file linkedlist.h
 */

#ifndef HTTPLIB_LL
#define HTTPLIB_LL

#include <stddef.h>
typedef struct ll_node {
    char* key;
    char* value;
    struct ll_node* next;
} ll_node;

void ll_push_back(ll_node* root, char* key, char* val);
ll_node* ll_get_last(ll_node* root);
ll_node* ll_create(char* key, char* value);
void ll_destroy(ll_node* root);
ll_node* ll_clone(ll_node* root);
/**
 * @brief takes a node and converts it into a header line
 */
char* ll_to_header(ll_node* node);
/**
 * @brief Takes a linked list and turns the entire chain into a char* of HTTP headers.
 * @param node the head of the headers
 * @param size always set this to zero
 */
char* ll_create_headers(ll_node* node, size_t size);

#endif
