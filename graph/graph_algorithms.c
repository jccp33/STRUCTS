#include "graph.h"
#include "queue.h"
#include <stdio.h>
#include <stdbool.h>

void graph_find_path(Graph *graph, int start_node, int end_node, char* (*get_name)(int)) {
    if (start_node == end_node) {
        printf("IN: %s.\n", get_name(start_node));
        return;
    }
    int *parent = malloc(graph->num_vertices * sizeof(int));
    bool *visited = calloc(graph->num_vertices, sizeof(bool));
    for(int i=0; i < graph->num_vertices; i++) parent[i] = -1;
    Queue *q = queue_create();
    visited[start_node] = true;
    queue_enqueue(q, (void*)(intptr_t)start_node);
    bool found = false;
    while (!queue_is_empty(q)) {
        int curr = (int)(intptr_t)queue_dequeue(q);
        if (curr == end_node) {
            found = true;
            break;
        }
        NODE *neighbor = graph->adj_lists[curr]->head;
        while (neighbor) {
            int next_id = (int)(intptr_t)neighbor->data;
            if (!visited[next_id]) {
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
    free(parent);
    free(visited);
    queue_destroy(q, NULL);
}
