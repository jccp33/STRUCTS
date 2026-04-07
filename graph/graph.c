#include "graph.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>

Graph *graph_create(int vertices, bool directed) {
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
    graph->adj_lists = malloc(vertices * sizeof(LinkedList *));
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

void graph_add_edge(Graph *graph, int src, int dest, int weight, int index) {
    if (graph == NULL) {
        printErrorOnFile("graph.c", __LINE__, "graph_add_edge: graph is NULL");
        return;
    }
    if (src < 0 || src >= graph->num_vertices || dest < 0 || dest >= graph->num_vertices) {
        char mssg[256];
        snprintf(mssg, sizeof(mssg), "graph_add_edge: vertex index out of range. src: %d, dest: %d, verts: %d", src, dest, graph->num_vertices);
        printErrorOnFile("graph.c", __LINE__, mssg);
        return;
    }
    if (src == dest) {
        char mssg[256];
        snprintf(mssg, sizeof(mssg), "graph_add_edge: self-loop not allowed. src: %d, dest: %d", src, dest);
        printErrorOnFile("graph.c", __LINE__, mssg);
        return;
    }
    NODE *curr = graph->adj_lists[src]->head;
    while (curr) {
        if (((Edge *)curr->data)->dest == dest) {
            char mssg[256];
            snprintf(mssg, sizeof(mssg), "graph_add_edge: edge already exists. src: %d, dest: %d", src, dest);
            printErrorOnFile("graph.c", __LINE__, mssg);
            return;
        }
        curr = curr->next;
    }
    Edge *edge = malloc(sizeof(Edge));
    if (!edge)
        return;
    edge->dest = dest;
    edge->weight = weight;
    list_add_back(graph->adj_lists[src], (void *)edge, index);
    if (graph->adj_lists[src]->tail == NULL || ((Edge *)graph->adj_lists[src]->tail->data)->dest != dest) {
        printErrorOnFile("graph.c", __LINE__, "graph_add_edge: failed to add destination");
        return;
    }
    if (!graph->is_directed) {
        Edge *rev_edge = malloc(sizeof(Edge));
        if (!rev_edge)
            return;
        rev_edge->dest = src;
        rev_edge->weight = weight;
        list_add_back(graph->adj_lists[dest], (void *)rev_edge, index);
        if (graph->adj_lists[dest]->tail == NULL || ((Edge *)graph->adj_lists[dest]->tail->data)->dest != src) {
            printErrorOnFile("graph.c", __LINE__, "graph_add_edge: failed to add source in undirected graph");
        }
    }
}

void graph_print(Graph *graph) {
    if (graph == NULL) {
        printErrorOnFile("graph.c", __LINE__, "graph_print: graph is NULL");
        return;
    }
    printf("==========================================\n");
    printf("[ORIG] : DEST1(WEIGHT), DEST2(WEIGHT), ...\n");
    printf("==========================================\n");
    for (int i = 0; i < graph->num_vertices; i++) {
        if (graph->adj_lists[i] == NULL) {
            printErrorOnFile("graph.c", __LINE__, "graph_print: adjacency list is NULL for a vertex");
            continue;
        }
        printf("[%d]: ", i);
        NODE *curr = graph->adj_lists[i]->head;
        while (curr) {
            Edge *e = (Edge *)curr->data;
            printf("%d(%d), ", e->dest, e->weight);
            curr = curr->next;
        }
        printf("NULL\n");
    }
    printf("==========================================\n");
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
            list_destroy(graph->adj_lists[i], free);
        }
    }
    free(graph->adj_lists);
    free(graph);
}
