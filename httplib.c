#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "httplib.h"
#define PORT 8080

node root;

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

//TODO: rename this
// 0 when false, 1 when true
node* find_tok(char val[], node* n) {
    while(n != NULL && strcmp(val, n->val) != 0) {
        n = n->siblings;
    }

    return n;
}

node* find_leaf_node_by_route(char route[]) {
    char copiedRoute[strlen(route)];
    strcpy(copiedRoute, route);
    char* tok = strtok(copiedRoute, "/");
    node* ptr = &root;

    while(tok != NULL) {
        node* newPtr = find_tok(tok, ptr->children);
        if(newPtr != NULL) {
            ptr = newPtr;
        } else {
            break;
        }
        tok = strtok(NULL, "/");
    }

    return ptr;
}

void root_callback(void) {
    printf("Send 200\n");
}

void setup(void) {
    root.val = "root";
    root.callback = &root_callback;
    root.siblings = NULL;
    root.children = NULL;
}

enum ERROR_CODES { RR_DUPLICATE_ROUTE, RR_OK };

/**
 * TODO: make this actually reference error codes
 * @return ERROR_CODES
 */
int register_route(char route[], callback_t callback) {
    // remove the verb
    while(route[0] != '/') {
        route++;
    }
    // get the node I am going to attach to
    node* targetNode = find_leaf_node_by_route(route);

    char* tok = strtok(route, "/");
    if(strcmp(targetNode->val, "root") != 0) {
        // update the token until it is the same as targetNode's token
        while(strcmp(tok, targetNode->val) != 0) {
            tok = strtok(NULL, "/");
        }

        // get the first token that doesn't exist in the tree
        tok = strtok(NULL, "/");
    } else {
        targetNode = &root;
    }


    if(tok == NULL) {
        if(targetNode->callback != NULL) {
            // there exists no token that doesn't already exist in the tree
            return RR_DUPLICATE_ROUTE;
        } else {
            targetNode->callback = callback;
            return RR_OK;
        }
    }

    // keep a pointer to the last created node so I can attach the callback
    node* leafNode = NULL;
    if(targetNode->children != NULL) {
        node* temp = targetNode->children;
        leafNode = malloc(sizeof(node));
        leafNode->val = tok;
        leafNode->siblings = temp;
        leafNode->children = NULL;
        targetNode->children = leafNode;
        targetNode->callback = NULL;
        targetNode = leafNode;
        tok = strtok(NULL, "/");
    }

    while(tok != NULL) {
        leafNode = malloc(sizeof(node));
        leafNode->val = tok;
        leafNode->siblings = NULL;
        leafNode->children = NULL;
        leafNode->callback = NULL;
        targetNode->children = leafNode;
        targetNode = targetNode->children;
        tok = strtok(NULL, "/");
    }

    leafNode->callback = callback;
    return RR_OK;
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

int create_server(void) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1; 
    int addrlen = sizeof(address);
    char buffer[1024] = { 0 };
    char* hello = "HTTP/1.0 200 \r\n";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        perror("socket failed"); 
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    /*valread = read(new_socket, buffer, 1024);*/
    printf("%s\n", buffer);
    send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent\n");

    // closing the connected socket  
    close(new_socket);
    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
    return 0;
}
