#include <assert.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include "httplib/httplib.h"
#include "httplib/linkedlist.h"
#include "httplib/request_parser.h"
#include "httplib/response.h"
#include "httplib/tree.h"

#define PORT 8080
#define MAX_REQUEST_SIZE 4096

node get_root;
node post_root;

void root_callback(request req, response* resp) {
    printf("req uri: %s\n", req.uri);
    resp->code = 404;
    return;
}

void setup(void) {
    get_root.val = "root";
    get_root.callback = &root_callback;
    get_root.siblings = NULL;
    get_root.children = NULL;

    post_root.val = "root";
    post_root.callback = &root_callback;
    post_root.siblings = NULL;
    post_root.children = NULL;
}

int register_route(char route[], callback_t callback, node* root_node) {
    // get the node I am going to attach too
    node* targetNode = trace_tree(route, root_node);

    char* fsegment = strtok(route, "/");
    if(strcmp(targetNode->val, "root") != 0) {
        // update the token until it is the same as targetNode's token
        while(strcmp(fsegment, targetNode->val) != 0) {
            fsegment = strtok(NULL, "/");
        }

        // get the first token that doesn't exist in the tree
        fsegment = strtok(NULL, "/");
    } else {
        targetNode = root_node;
    }


    if(fsegment == NULL) {
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
        leafNode->val = fsegment;
        leafNode->siblings = temp;
        leafNode->children = NULL;

        targetNode->children = leafNode;
        targetNode->callback = NULL;
        targetNode = leafNode;

        fsegment = strtok(NULL, "/");
    }

    while(fsegment != NULL) {
        leafNode = malloc(sizeof(node));
        leafNode->val = fsegment;
        leafNode->siblings = NULL;
        leafNode->children = NULL;
        leafNode->callback = NULL;

        targetNode->children = leafNode;
        targetNode = targetNode->children;

        fsegment = strtok(NULL, "/");
    }

    leafNode->callback = callback;
    return RR_OK;
}

int get(char route[], callback_t callback) {
    return register_route(route, callback, &get_root);
}

int post(char route[], callback_t callback) {
    return register_route(route, callback, &post_root);
}

void* handle_connection(void* sock_desc) {
    // pthreads require the param be a void pointer
    int new_socket = *(int*)sock_desc;

    char request_buffer[MAX_REQUEST_SIZE] = { 0 };
    ssize_t bytes_read = recv(new_socket, request_buffer, sizeof(request_buffer) - 1, 0);

    if(bytes_read < 0) {
        perror("httplib.c:handle_connection failed to read bytes");
        exit(EXIT_FAILURE);
    }

    if(bytes_read == 0) {
        close(new_socket);
        pthread_exit(NULL);
        return NULL;
    }

    request req;
    req.headers = NULL;

    response resp;
    resp.headers = NULL;
    memset(resp.body, '\0', sizeof(resp.body));

    // Take the request buffer and parse it into a request struct
    if(create_request(request_buffer, &req) == -1) {
        resp.code = 400;
    } else {
        node* n = trace_tree_exact(req.uri, &get_root);
        if(n == NULL || n->callback == NULL) {
            resp.code = 404;
        } else {
            n->callback(req, &resp);
        }
    }

    // Take the response headers and turn it into a char pointer
    char* headers = ll_create_headers(resp.headers, 0);

    // Create a response buffer with enough space to hold all the data
    int headers_size = strlen(headers);
    int response_size = strlen(resp.body) + 19 + headers_size + strlen("OK");
    char response_buffer[response_size];
    response_buffer[response_size - 1] = '\0';

    // Take the response struct, headers, and put them into the request buffer
    unparse_response(&resp, headers, request_buffer);

    if(send(new_socket, response_buffer, strlen(response_buffer), 0) == -1) {
        perror("httplib.c:handle_connection failed to send");
    }

    ll_destroy(req.headers);
    ll_destroy(resp.headers);

    close(new_socket);
    free(sock_desc);
    pthread_exit(NULL);

    return NULL;
}

int http_listen(void) {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1; 
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        perror("socket failed"); 
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("httplib.c:http_liste failed to bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("httplib.c:http_listen failed to listen");
        exit(EXIT_FAILURE);
    }

    while(1) {
        int* new_socket;
        if((new_socket = malloc(sizeof(int))) == NULL) {
            perror("httplib.c:http_listen failed to allocate memory for new_socket");
            exit(EXIT_FAILURE);
        }

        if ((*new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("httplib.c:http_listen failed to accept");
            exit(EXIT_FAILURE);
        }

        pthread_t pid;
        if(pthread_create(&pid, NULL, &handle_connection, new_socket)) {
            perror("httplib.c:http_listen failed to create thread");
            exit(EXIT_FAILURE);
        }

        if(pthread_detach(pid)) {
            perror("httplib.c:http_listen failed to detach");
        }
    }

    // closing the listening socket
    if(shutdown(server_fd, SHUT_RDWR)) {
        perror("httplib.c:http_listen failed to shutdown server");
    }

    return 0;
}
