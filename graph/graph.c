#include "graph.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>

Graph* graph_create(int vertices, bool directed) {
    if (vertices <= 0) {
        printErrorOnFile("graph.c", __LINE__, "graph_create: invalid number of vertices");
        return NULL;
    }
    Graph *graph = malloc(sizeof(Graph));
    if (graph == NULL) {
        printErrorOnFile("graph.c", __LINE__, "graph_create: falla malloc(Graph)");
        return NULL;
    }
    graph->num_vertices = vertices;
    graph->is_directed = directed;
    graph->adj_lists = malloc(vertices * sizeof(LinkedList*));
    if (graph->adj_lists == NULL) {
        printErrorOnFile("graph.c", __LINE__, "graph_create: falla malloc(adj_lists)");
        free(graph);
        return NULL;
    }
    for (int i = 0; i < vertices; i++) {
        graph->adj_lists[i] = list_create();
        if (graph->adj_lists[i] == NULL) {
            for (int j = 0; j < i; j++) {
                list_destroy(graph->adj_lists[j], NULL);
            }
            free(graph->adj_lists);
            free(graph);
            return NULL;
        }
    }
    return graph;
}

void graph_add_edge(Graph *graph, int src, int dest) {
    if (graph == NULL) {
        printErrorOnFile("graph.c", __LINE__, "graph_add_edge: graph is NULL");
        return;
    }
    if (src < 0 || src >= graph->num_vertices || dest < 0 || dest >= graph->num_vertices) {
        printErrorOnFile("graph.c", __LINE__, "graph_add_edge: vertex index out of range");
        return;
    }
    if (src == dest) {
        printErrorOnFile("graph.c", __LINE__, "graph_add_edge: self-loop not allowed");
        return;
    }
    NODE *curr = graph->adj_lists[src]->head;
    while (curr) {
        if ((int)(intptr_t)curr->data == dest) {
            printErrorOnFile("graph.c", __LINE__, "graph_add_edge: edge already exists");
            return;
        }
        curr = curr->next;
    }
    list_add_back(graph->adj_lists[src], (void*)(intptr_t)dest);
    if (graph->adj_lists[src]->tail == NULL || (int)(intptr_t)graph->adj_lists[src]->tail->data != dest) {
        printErrorOnFile("graph.c", __LINE__, "graph_add_edge: failed to add destination");
        return;
    }
    if (!graph->is_directed) {
        list_add_back(graph->adj_lists[dest], (void*)(intptr_t)src);
        if (graph->adj_lists[dest]->tail == NULL || (int)(intptr_t)graph->adj_lists[dest]->tail->data != src) {
            printErrorOnFile("graph.c", __LINE__, "graph_add_edge: failed to add source in undirected graph");
        }
    }
}

void graph_print(Graph *graph) {
    if (graph == NULL) {
        printErrorOnFile("graph.c", __LINE__, "graph_print: graph is NULL");
        return;
    }
    for (int i = 0; i < graph->num_vertices; i++) {
        if (graph->adj_lists[i] == NULL) {
            printErrorOnFile("graph.c", __LINE__, "graph_print: adjacency list is NULL for a vertex");
            continue;
        }
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
    if (graph == NULL) {
        printErrorOnFile("graph.c", __LINE__, "graph_destroy: graph is NULL");
        return;
    }
    if (graph->adj_lists == NULL) {
        printErrorOnFile("graph.c", __LINE__, "graph_destroy: adj_lists is NULL");
        free(graph);
        return;
    }
    for (int i = 0; i < graph->num_vertices; i++) {
        if (graph->adj_lists[i] != NULL) {
            list_destroy(graph->adj_lists[i], NULL);
        }
    }
    free(graph->adj_lists);
    free(graph);
}
