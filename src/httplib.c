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

    char* tok = strtok(route, "/");
    if(strcmp(targetNode->val, "root") != 0) {
        // update the token until it is the same as targetNode's token
        while(strcmp(tok, targetNode->val) != 0) {
            tok = strtok(NULL, "/");
        }

        // get the first token that doesn't exist in the tree
        tok = strtok(NULL, "/");
    } else {
        targetNode = &get_root;
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

int get(char route[], callback_t callback) {
    return register_route(route, callback, &get_root);
}

int post(char route[], callback_t callback) {
    return register_route(route, callback, &post_root);
}

void* handle_connection(void* sock_desc) {
    int new_socket = *(int*)sock_desc;
    char buffer[4096] = { 0 };
    //ssize_t bytes_read = read(new_socket, buffer, 1023);
    ssize_t bytes_read = recv(new_socket, buffer, sizeof(buffer) - 1, 0);

    if(bytes_read < 0) {
        perror("failed to read bytes");
        exit(EXIT_FAILURE);
    }

    if(bytes_read == 0) {
        close(new_socket);
        pthread_exit(EXIT_SUCCESS);
        return NULL;
    }

    request req;
    req.headers = NULL;

    response resp;
    resp.headers = NULL;
    memset(resp.body, '\0', sizeof(resp.body));

    int res = create_request(buffer, &req);
    if(res == -1) {
        puts("sending 400");
        resp.code = 400;
    } else {
        node* n = trace_tree_exact(req.uri, &get_root);
        if(n == NULL || n->callback == NULL) {
            resp.code = 404;
        } else {
            n->callback(req, &resp);
        }
    }

    char* headers = ll_create_headers(resp.headers, 0);
    int headers_size = strlen(headers);
    int response_size = 4096;
    //char* response_buffer = "HTTP/1.0 200 OK\r\n";
    char response_buffer[response_size];
    if(snprintf(response_buffer, sizeof(response_buffer) - 1,"HTTP/1.0 %d OK\r\n%s\r\n", resp.code, headers) < 0) {
        perror("failed sprintf");
    }
    response_buffer[response_size - 1] = '\0';

    send(new_socket, response_buffer, strlen(response_buffer), 0);

    ll_destroy(req.headers);
    ll_destroy(resp.headers);

    // closing the connected socket  
    close(new_socket);
    pthread_exit(NULL);

    return NULL;
}

int http_listen(void) {
    int server_fd, new_socket;
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
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while(1) {
        int* new_socket = malloc(sizeof(int));
        if ((*new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        //handle_connection(&new_socket);
        pthread_t pid;
        if(pthread_create(&pid, NULL, &handle_connection, new_socket)) {
            perror("failed to create thread");
            exit(EXIT_FAILURE);
        }
        pthread_detach(pid);
    }
    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
    return 0;
}
