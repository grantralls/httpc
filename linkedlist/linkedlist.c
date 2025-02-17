#include "linkedlist.h"
#include <stdlib.h>
#include <string.h>

void ll_push_back(ll_node* root, char* key, char* val) {
    ll_node* last = ll_get_last(root);
    ll_node* new_node = ll_create(key, val);
    new_node->next = NULL;
    new_node->key = key;
    new_node->value = val;
    last->next = new_node;
}

ll_node* ll_create(char* key, char* val) {
    ll_node* new_node = malloc(sizeof(ll_node));
    if(new_node == NULL) {
        return NULL;
    }

    new_node->key = key;
    new_node->value = val;
    return new_node;
}

ll_node* ll_get_last(ll_node *root) {
    // TODO: handle when root is null
    if(root->next == NULL) {
        return root;
    }

    return ll_get_last(root->next);
}

void ll_destroy(ll_node* root) {
    if(root == NULL) {
        return;
    }
    if(root->next != NULL) {
        ll_destroy(root->next);
    }

    free(root);
    return;
}
