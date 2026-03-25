#include "node.h"

NODE* create_node(void *data) {
    NODE *new_node = (NODE *)malloc(sizeof(NODE));
    if (new_node == NULL) {
        return NULL;
    }
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}

void destroy_node(NODE *node) {
    if (node != NULL) {
        free(node);
    }
}
