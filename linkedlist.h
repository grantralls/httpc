#ifndef HTTPLIB_LL
#define HTTPLIB_LL

typedef struct ll_node {
    char* key;
    char* value;
    struct ll_node* next;
} ll_node;

void push_back(ll_node* root, char* key, char* val);
ll_node* get_last(ll_node* root);

#endif
