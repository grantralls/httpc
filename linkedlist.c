#include "linkedlist.h"
#include <stdlib.h>
#include <string.h>

void push_back(ll_node* root, char* key, char* val) {
    ll_node* last = get_last(root);
    ll_node* new_node = malloc(sizeof(ll_node));
    new_node->next = NULL;
    new_node->key = key;
    new_node->value = val;
    last->next = new_node;
}

ll_node* get_last(ll_node *root) {
    // TODO: handle when root is null
    if(root->next == NULL) {
        return root;
    }

    return get_last(root->next);
}

void destroy_list(ll_node* root) {
    // TODO: handle when root is null
    if(root->next == NULL) {
        free(root);
        return;
    }
    destroy_list(root->next);
}
