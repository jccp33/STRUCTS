#ifndef NODE_H
#define NODE_H
#include <stdlib.h>
#include <stdio.h>

typedef struct NODE {
    void *data;
    struct NODE *next;
    struct NODE *prev;
    int index;
} NODE;

NODE* create_node(void *data, int index);

void destroy_node(NODE *NODE);

#endif
