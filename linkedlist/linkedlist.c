#include "linkedlist.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ll_push_back(ll_node* root, char* key, char* val) {
    ll_node* last = ll_get_last(root);
    ll_node* new_node = ll_create(key, val);
    new_node->key = key;
    new_node->value = val;
    last->next = new_node;
}

ll_node* ll_create(char* key, char* val) {
    ll_node* new_node = malloc(sizeof(ll_node));
    if(new_node == NULL) {
        perror("linkedlist.c:ll_create");
        exit(EXIT_FAILURE);
    }

    new_node->key = key;
    new_node->value = val;
    new_node->next = NULL;
    return new_node;
}

ll_node* ll_get_last(ll_node* root) {
    if(root->next == NULL) {
        return root;
    }

    return ll_get_last(root->next);
}

ll_node* ll_clone(ll_node* root) {
    ll_node* new_root = malloc(sizeof(ll_node));

    if(new_root == NULL) {
        perror("linkedlist.c:ll_clone");
        exit(EXIT_FAILURE);
    }

    new_root->key = root->key;
    new_root->value = root->value;
    new_root->next = NULL;

    ll_node* temp = new_root;
    ll_node* root_temp = root;

    while(root_temp != NULL) {
        temp->key = root->key;
        temp->value = root->value;
        root_temp = root_temp->next;
        if(root_temp != NULL) {
            temp->next = malloc(sizeof(ll_node));

            if(temp->next == NULL) {
                perror("linkedlist.c:ll_clone");
                exit(EXIT_FAILURE);
            }

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

char* ll_to_header(ll_node* node) {
    // plus 3 for the ": " and "\n"
    int len = strlen(node->key) + strlen(node->value) + 3;
    char* res = calloc(len, sizeof(char));
    strncat(res, node->key, len - 1);
    strncat(res, ": ", len - 1);
    strncat(res, node->value, len - 1);
    strncat(res, "\n", len - 1);
    return res;
}

/**
 * Unless you know what you are doing, set size to 0
 */
char* ll_create_headers(ll_node* node, size_t size) {
    if(node == NULL) {
        char* buffer = calloc(size, sizeof(char));
        if(buffer == NULL) {
            perror("linkedlist.c:ll_create_headers");
            exit(EXIT_FAILURE);
        }
    }

    int buffer_size = size + strlen(node->key) + strlen(node->value) + 3;
    char* buffer = ll_create_headers(node->next, size + strlen(node->key) + strlen(node->value) + 3);

    (void)strncat(buffer, node->key, buffer_size - 1);
    (void)strncat(buffer, ":", buffer_size - 1);
    (void)strncat(buffer, node->value, buffer_size - 1);
    (void)strncat(buffer, "\r\n", buffer_size - 1);

    return buffer;
}
