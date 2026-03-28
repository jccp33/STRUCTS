#include "node.h"
#include "error.h"

NODE* create_node(void *data) {
    NODE *new_node = (NODE *)malloc(sizeof(NODE));
    if (new_node == NULL) {
        printErrorOnFile("node.c", __LINE__, "create_node: malloc(NODE) failed");
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
