#ifndef HTTPLIB_LL
#define HTTPLIB_LL

typedef struct ll_node {
    char* key;
    char* value;
    struct ll_node* next;
} ll_node;

void ll_push_back(ll_node* root, char* key, char* val);
ll_node* ll_get_last(ll_node* root);
ll_node* ll_create(char* key, char* value);
void ll_destroy(ll_node* root);

#endif
