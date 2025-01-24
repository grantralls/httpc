#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

struct node {
    char* val;
    struct node* children;
    struct node* siblings;
    void (*callback)();
};

struct node root = {};

void root_callback() {
    printf("Send 200");
}

void setup() {
    root.val = "GET /";
    root.callback = &root_callback;
}

void register_node(struct node* root, struct node* newNode) {
    if(root->children == NULL) {
        root->children = newNode;
    }
}

void register_route(char* route, void (*callback)()) {
    struct node* newNode = malloc(sizeof(struct node));
    newNode->val = route;
    newNode->callback = callback;
    register_node(&root, newNode);
}

void print_tree(struct node* n) {
    printf("%s\n", n->val);
    n->callback();
    printf("\n");
    if(n->children != NULL) {
        print_tree(n->children);
    } else if(n->siblings != NULL) {
        print_tree(n->siblings);
    }
}

void test_callback() {
    printf("Test callback");
}

int main(int argc, char const* argv[]) {
    setup();
    register_route("GET /test", &test_callback);
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

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        perror("socket failed"); 
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to port 8080
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
