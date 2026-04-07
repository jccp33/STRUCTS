#include "error.h"
#include "graph.h"
#include "hashtable.h"
#include "heap.h"
#include "list.h"
#include "node.h"
#include "persistence.h"
#include "queue.h"
#include "stack.h"
#include "tree.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern uint32_t hash_djb2(const char *str);

int main() {
    printf("--------------- TESTING STRUCTS ---------------\n\n");

    Graph *graph = graph_create(5, false);
    graph_add_edge(graph, 0, 1, 3);
    graph_add_edge(graph, 0, 2, 5);
    graph_add_edge(graph, 0, 3, 9);
    graph_add_edge(graph, 0, 4, 7);
    graph_add_edge(graph, 1, 2, 2);
    graph_add_edge(graph, 1, 3, 6);
    graph_add_edge(graph, 1, 4, 8);
    graph_add_edge(graph, 2, 3, 1);
    graph_add_edge(graph, 2, 4, 4);
    graph_add_edge(graph, 3, 4, 5);

    graph_print(graph);
    graph_destroy(graph);

    // HashTable* symbol_table = ht_create(hash_djb2);
    // if (symbol_table) {
    //     ht_insert(symbol_table, "var", (void*)101);
    //     printf("[OK] Keyword 'var' registrada con ID 101.\n");
    //     printf("[OK] Libreria lista para compartir.\n");
    // } else {
    //     printErrorOnFile("main.c", __LINE__, "Fallo al vincular STRUCTS.");
    // }

    printf("\n-----------------------------------------------\n");

    return 0;
}
