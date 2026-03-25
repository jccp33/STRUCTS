#include "graph.h"
#include <stdio.h>

Graph* graph_create(int vertices, bool directed) {
    Graph *graph = malloc(sizeof(Graph));
    graph->num_vertices = vertices;
    graph->is_directed = directed;
    graph->adj_lists = malloc(vertices * sizeof(LinkedList*));
    for (int i = 0; i < vertices; i++) {
        graph->adj_lists[i] = list_create();
    }
    return graph;
}

void graph_add_edge(Graph *graph, int src, int dest) {
    if (src == dest) return;
    NODE *curr = graph->adj_lists[src]->head;
    while (curr) {
        if ((int)(intptr_t)curr->data == dest) return;
        curr = curr->next;
    }
    list_add_back(graph->adj_lists[src], (void*)(intptr_t)dest);
    if (!graph->is_directed) {
        list_add_back(graph->adj_lists[dest], (void*)(intptr_t)src);
    }
}

void graph_print(Graph *graph) {
    for (int i = 0; i < graph->num_vertices; i++) {
        printf("Vertice %d: ", i);
        NODE *curr = graph->adj_lists[i]->head;
        while (curr) {
            printf("%d -> ", (int)(intptr_t)curr->data);
            curr = curr->next;
        }
        printf("NULL\n");
    }
}

void graph_destroy(Graph *graph) {
    if (graph == NULL) return;
    for (int i = 0; i < graph->num_vertices; i++) {
        if (graph->adj_lists[i] != NULL) {
            list_destroy(graph->adj_lists[i], NULL);
        }
    }
    free(graph->adj_lists);
    free(graph);
}
