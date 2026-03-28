#include "graph.h"
#include "queue.h"
#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void graph_find_path(Graph *graph, int start_node, int end_node, char* (*get_name)(int)) {
    if (graph == NULL) {
        printErrorOnFile("graph_algorithms.c", __LINE__, "graph_find_path: graph is NULL");
        return;
    }
    if (get_name == NULL) {
        printErrorOnFile("graph_algorithms.c", __LINE__, "graph_find_path: get_name callback is NULL");
        return;
    }
    if (start_node < 0 || start_node >= graph->num_vertices || end_node < 0 || end_node >= graph->num_vertices) {
        printErrorOnFile("graph_algorithms.c", __LINE__, "graph_find_path: start or end node index out of range");
        return;
    }
    if (start_node == end_node) {
        printErrorOnFile("graph_algorithms.c", __LINE__, "graph_find_path: start_node and end_node are equals");
        return;
    }
    int *parent = malloc(graph->num_vertices * sizeof(int));
    if (parent == NULL) {
        printErrorOnFile("graph_algorithms.c", __LINE__, "graph_find_path: malloc parent failed");
        return;
    }
    bool *visited = calloc(graph->num_vertices, sizeof(bool));
    if (visited == NULL) {
        printErrorOnFile("graph_algorithms.c", __LINE__, "graph_find_path: calloc visited failed");
        free(parent);
        return;
    }
    for (int i = 0; i < graph->num_vertices; i++) {
        parent[i] = -1;
    }
    Queue *q = queue_create();
    if (q == NULL) {
        printErrorOnFile("graph_algorithms.c", __LINE__, "graph_find_path: queue_create failed");
        free(parent);
        free(visited);
        return;
    }
    visited[start_node] = true;
    queue_enqueue(q, (void*)(intptr_t)start_node);
    bool found = false;
    while (!queue_is_empty(q)) {
        int curr = (int)(intptr_t)queue_dequeue(q);
        if (curr == end_node) {
            found = true;
            break;
        }
        if (graph->adj_lists[curr] == NULL) {
            printErrorOnFile("graph_algorithms.c", __LINE__, "graph_find_path: adjacency list is NULL for current node");
            continue;
        }
        NODE *neighbor = graph->adj_lists[curr]->head;
        while (neighbor) {
            int next_id = (int)(intptr_t)neighbor->data;
            if (next_id < 0 || next_id >= graph->num_vertices) {
                printErrorOnFile("graph_algorithms.c", __LINE__, "graph_find_path: neighbor node index out of range");
            } else if (!visited[next_id]) {
                visited[next_id] = true;
                parent[next_id] = curr;
                queue_enqueue(q, (void*)(intptr_t)next_id);
            }
            neighbor = neighbor->next;
        }
    }
    if (found) {
        printf("Route: ");
        int temp = end_node;
        while (temp != -1) {
            printf("%s %s", get_name(temp), (parent[temp] != -1) ? "<- " : "");
            temp = parent[temp];
        }
        printf("\n");
    } else {
        printf("No route found.\n");
    }
    queue_destroy(q, NULL);
    free(parent);
    free(visited);
}
