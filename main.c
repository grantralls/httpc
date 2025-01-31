#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

typedef struct node {
    char* val;
    struct node* children;
    struct node* siblings;
    void (*callback)();
} node;

node root;

/**
* @brief Searches the tree in a DFS manner to find the node with the val
* @param root the root of the tree
* @param val the val we want to look for
* @returns a pointer to a node if one was found, NULL if none was found
*/
node* find_node_by_val(node* root, char* val) {
    if(!strcmp(root->val, val)) {
        return root;
    }
    if(root->children != NULL) {
        return find_node_by_val(root->children, val);
    } else if(root->siblings != NULL) {
        return find_node_by_val(root->siblings, val);
    }

    return NULL;
}

/**
 * Given a route, split that route into a tree path, deliminated by "/".
 * Walk the tree as far as possible and return the last found node
 */
node* find_leaf_node_by_route(char route[]) {
    char copiedRoute[strlen(route)];
    strcpy(copiedRoute, route);
    char* tok = strtok(copiedRoute, "/");
    node* ptr = &root;

    while(tok != NULL) {
        node* newPtr = find_node_by_val(ptr, tok);
        if(newPtr != NULL) {
            ptr = newPtr;
        } else {
            break;
        }
        tok = strtok(NULL, "/");
    }

    return ptr;
}


void root_callback() {
    printf("Send 200\n");
}

void setup() {
    root.val = "root";
    root.callback = &root_callback;
    root.siblings = NULL;
    root.children = NULL;
}

// TODO: If I create a route /some/silly/route then another one of /some/silly, this explodes
/**
 * @brief given a route, add it to the uri tree
 */
void register_route(char route[], void (*callback)()) {
    // remove the verb
    while(route[0] != '/') {
        route++;
    }
    // get the node I am going to attach to
    node* targetNode = find_leaf_node_by_route(route);

    // update the token until it is the same as targetNode's token
    char* tok = strtok(route, "/");
    while(strcmp(tok, targetNode->val) != 0) {
        tok = strtok(NULL, "/");
    }

    // get the first token that doesn't exist in the tree
    tok = strtok(NULL, "/");

    if(tok == NULL) {
        if(targetNode->callback != NULL) {
            // there exists no token that doesn't already exist in the tree
            printf("The given route already has a path: %s\n", route);
            exit(-1);
        } else {
            targetNode->callback = callback;
            return;
        }
    }

    // keep a pointer to the last created node so I can attach the callback
    node* leafNode;
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

/**
 * @brief traverse the tree in a depth first way
 * @param n The root of the tree
 */
void print_tree(node* n) {
    printf("%s\n", n->val);
    if(n->callback != NULL) {
        n->callback();
    }
    if(n->children != NULL) {
        print_tree(n->children);
    }
    if(n->siblings != NULL) {
        print_tree(n->siblings);
    }
}

// "/some/uri/resource"
void print_each_entry(char* message) {
    while(strlen(message) != 0) {
        message++;
        int len = 0;
        while(message[len + 1] != '\0' && message[len] != '/') {
            len++;
        }
        char newString[len];
        strncpy(newString, message, len);
        message += len;
    }
}

void test_callback_1() {
    printf("HELLO THERE!\n");
}

void test_callback_2() {
    printf("HELLO THERE AGAIN!\n");
}

int main(int argc, char const* argv[]) {
    setup();
    char route2[] = "GET /root/test/something/something_else";
    char route3[] = "GET /root/test/something";
    register_route(route2, *test_callback_2);
    register_route(route3, *test_callback_2);
    destroy_tree(&root);

    return 0;
}

int create_server() {
    int server_fd, new_socket, valread;
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
    valread = read(new_socket, buffer, 1024);
    printf("%s\n", buffer);
    send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent\n");

    // closing the connected socket  
    close(new_socket);
    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
    return 0;
}
