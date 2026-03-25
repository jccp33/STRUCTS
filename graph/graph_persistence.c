#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "graph.h"

void graph_save_txt(Graph *graph, const char *filename, char* (*get_node_name)(int)) {
    FILE *file = fopen(filename, "w");
    if (!file) return;
    fprintf(file, "# PERSISTENCIA DE GRAFO (Nodos: %d, Dirigido: %s)\n", 
            graph->num_vertices, graph->is_directed ? "SI" : "NO");
    fprintf(file, "Origen -> Destinos\n");
    for (int i = 0; i < graph->num_vertices; i++) {
        bool has_connections = false;
        NODE *curr = graph->adj_lists[i]->head;
        while (curr) {
            int dest_id = (int)(intptr_t)curr->data;
            if (graph->is_directed || i < dest_id) {
                if (!has_connections) {
                    fprintf(file, "%s -> ", get_node_name ? get_node_name(i) : "ID");
                    has_connections = true;
                }
                fprintf(file, "%s", get_node_name ? get_node_name(dest_id) : "ID");
                NODE *next_node = curr->next;
                bool further = false;
                while(next_node) {
                    if (graph->is_directed || i < (int)(intptr_t)next_node->data) {
                        further = true; break;
                    }
                    next_node = next_node->next;
                }
                if (further) fprintf(file, ", ");
            }
            curr = curr->next;
        }
        if (has_connections) fprintf(file, "\n");
    }
    fclose(file);
}

char* trim_spaces(char *str) {
    if (!str) return NULL;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return str;
    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    end[1] = '\0';
    return str;
}

void graph_load_txt(Graph *graph, const char *filename, int (*get_id_by_name)(char*)) {
    FILE *file = fopen(filename, "r");
    if (!file) return;
    char line[512];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n' || strncmp(line, "Origen", 6) == 0) continue;
        line[strcspn(line, "\n")] = 0;
        char *arrow = strstr(line, "->");
        if (!arrow) continue;
        *arrow = '\0';
        char *src_part = line;
        char *dest_part = arrow + 2;
        char *src_name = trim_spaces(src_part);
        int src_id = get_id_by_name(src_name);
        if (src_id != -1) {
            char *token = strtok(dest_part, ",");
            while (token) {
                char *dest_name = trim_spaces(token);
                int dest_id = get_id_by_name(dest_name);
                if (dest_id != -1) {
                    graph_add_edge(graph, src_id, dest_id);
                }
                token = strtok(NULL, ",");
            }
        }
    }
    fclose(file);
}
