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

node root = {};
node newNode = {};

void root_callback() {
    printf("Send 200");
}

void new_node_callback() {

}

void setup() {
    root.val = "/";
    root.callback = &root_callback;
    newNode.val = "test";
    newNode.callback = &new_node_callback;
    root.children = &newNode;
}

// TODO: use strtok
/*void parse_uri(node* newNode) {*/
/*    char* message = newNode->val;*/
/*    while(strlen(message) != 0) {*/
/*        message++;*/
/*        int len = 0;*/
/*        while(message[len] != '\0' && message[len] != '/') {*/
/*            len++;*/
/*        }*/
/*        char newString[len];*/
/*        strncpy(newString, message, len);*/
/**/
/*        printf("message: %s\n", newString);*/
/*        node* foundRoot = find_node_by_val(newRoot, newString);*/
/*        if(foundRoot != NULL) {*/
/*            newRoot = foundRoot;*/
/*        } else {*/
/*            // create the nodes/children*/
/**/
/*            printf("%s\n", newString);*/
/*        }*/
/**/
/*        memset(newString, 0, len);*/
/*        message += len;*/
/*    }*/
/*}*/

/**
 * @brief given a tree, and an uri, return the node the new uri will attach to.
 */
void getAttachingNode() {

}

// TODO: actually make this work
void register_node(node* root, char route[]) {
    char* tok = strtok(route, "/");
    node* newRoot = root;
    while(tok != NULL) {
        node* foundRoot = find_node_by_val(newRoot, tok);
        if(foundRoot != NULL) {
            newRoot = foundRoot;
        } else {
            if(newRoot->children != NULL) {
                newRoot = newRoot->children;
                while(newRoot->siblings != NULL) {
                    newRoot = newRoot->siblings;
                }
            }
            while(tok != NULL) {
                node* createdNode = malloc(sizeof(node));
                createdNode->val = tok;
                newRoot->children = createdNode;
                newRoot = newRoot->children;
                tok = strtok(NULL, "/");
            }
        }
        tok = strtok(NULL, "/");
    }
}

// METHOD URI
// GET /some/silly/endpoint
void register_route(char route[], void (*callback)()) {
    node* newNode = malloc(sizeof(node));
    while(route[0] != '/') {
        route++;
    }
    newNode->val = route;
    newNode->callback = callback;
    register_node(&root, route);
}

/**
 * @brief traverse the tree in a depth first way
 * @param n The root of the tree
 */
void print_tree(node* n) {
    printf("%s\n", n->val);
    if(n->children != NULL) {
        print_tree(n->children);
    }
    if(n->siblings != NULL) {
        print_tree(n->siblings);
    }
}


void test_callback() {
    printf("Test callback");
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
        printf("%s\n", newString);
        message += len;
    }
}

int main(int argc, char const* argv[]) {
    setup();
    register_route("GET /test/something", &test_callback);
    node* rootPtr = &root;
    printf("%s\n", rootPtr->children->children->val);
    print_tree(&root);

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
