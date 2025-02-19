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

ll_node* ll_get_last(ll_node* root) {
    // TODO: handle when root is null
    if(root->next == NULL) {
        return root;
    }

    return ll_get_last(root->next);
}

ll_node* ll_clone(ll_node* root) {
    ll_node* new_root = malloc(sizeof(ll_node));
    new_root->key = root->key;
    new_root->value = root->value;

    ll_node* temp = new_root;
    ll_node* root_temp = root;

    while(root_temp != NULL) {
        temp->key = root->key;
        temp->value = root->value;
        root_temp = root_temp->next;
        if(root_temp != NULL) {
            temp->next = malloc(sizeof(ll_node));
            temp = temp->next;
        }
    }

    return new_root;
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
