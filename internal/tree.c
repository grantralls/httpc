#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief search the current node and all siblings for a value, do not traverse any deeper in the tree
 */
node* find_tok(char val[], node* n) {
    while(n != NULL && strcmp(val, n->val) != 0) {
        n = n->siblings;
    }

    return n;
}

node* find_node_by_val(node* n, char* val) {
    if(!strcmp(n->val, val)) {
        return n;
    }
    if(n->children != NULL) {
        return find_node_by_val(n->children, val);
    } else if(n->siblings != NULL) {
        return find_node_by_val(n->siblings, val);
    }

    return NULL;
}

/**
 * Given a route, this function will traverse the tree in a DFS manner, using the route as a guide. As the tree is traversed, currNode is updated to be the most recently found node. When a new currNode is unable to be found, or we've reach the end of the route, return currNode.
 */
node* trace_tree(char route[], node* origin_node) {
    char copiedRoute[strlen(route) + 1];
    strcpy(copiedRoute, route);
    char* tok = strtok(copiedRoute, "/");
    node* currNode = origin_node;

    while(tok != NULL) {
        node* newCurrNode = find_tok(tok, currNode->children);
        if(newCurrNode != NULL) {
            currNode = newCurrNode;
        } else {
            break;
        }
        tok = strtok(NULL, "/");
    }

    return currNode;
}

/**
 * This is very similar to @ref trace_tree. The key difference is, a node is only returned when the entire route was able to be traced on the tree. This will typically be used for, finding the handling node for an incoming request. 
 */
node* trace_tree_exact(char route[], node* root) {
    char copiedRoute[strlen(route) + 1];
    strcpy(copiedRoute, route);
    char* tok = strtok(copiedRoute, "/");
    node* ptr = root;

    while(tok != NULL) {
        node* newPtr = find_tok(tok, ptr->children);
        if(newPtr == NULL) {
            return NULL;
        }
        ptr = newPtr;
        tok = strtok(NULL, "/");
    }

    return ptr;
}

void destroy_tree(node* n) {
    if(n->children != NULL) {
        destroy_tree(n->children);
    }
    if(n->siblings != NULL) {
        destroy_tree(n->siblings);
    }
    if(n->siblings != NULL) {
        free(n->siblings);
    }
    if(n->children != NULL) {
        free(n->children);
    }
}

void print_tree(node* n, int level) {
    printf("Level: %d val: %s\n", level, n->val);
    if(n->children != NULL) {
        print_tree(n->children, level + 1);
    }
    if(n->siblings != NULL) {
        print_tree(n->siblings, level);
    }
}
