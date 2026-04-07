#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include "graph.h"
#include "error.h"

void graph_save_txt(Graph *graph, const char *filename, char* (*get_node_name)(int)) {
    if (graph == NULL) {
        printErrorOnFile("graph_persistence.c", __LINE__, "graph_save_txt: graph is NULL");
        return;
    }
    if (filename == NULL) {
        printErrorOnFile("graph_persistence.c", __LINE__, "graph_save_txt: filename is NULL");
        return;
    }
    FILE *file = fopen(filename, "w");
    if (!file) {
        printErrorOnFile("graph_persistence.c", __LINE__, "graph_save_txt: cannot open file for writing");
        return;
    }
    fprintf(file, "# GRAPH PERSISTENCE (Nodes: %d, Directed: %s)\n", 
    graph->num_vertices, graph->is_directed ? "YES" : "NO");
    fprintf(file, "Origen -> Destinos\n");
    for (int i = 0; i < graph->num_vertices; i++) {
        if (graph->adj_lists[i] == NULL) {
            printErrorOnFile("graph_persistence.c", __LINE__, "graph_save_txt: adjacency list is NULL for a node");
            continue;
        }
        bool has_connections = false;
        NODE *curr = graph->adj_lists[i]->head;
        while (curr) {
            int dest_id = ((Edge*)curr->data)->dest;
            if (dest_id < 0 || dest_id >= graph->num_vertices) {
                printErrorOnFile("graph_persistence.c", __LINE__, "graph_save_txt: destination node index out of range");
                curr = curr->next;
                continue;
            }
            if (graph->is_directed || i < dest_id) {
                if (!has_connections) {
                    fprintf(file, "%s -> ", get_node_name ? get_node_name(i) : "ID");
                    has_connections = true;
                }
                fprintf(file, "%s", get_node_name ? get_node_name(dest_id) : "ID");
                NODE *next_node = curr->next;
                bool further = false;
                while(next_node) {
                    if (graph->is_directed || i < ((Edge*)next_node->data)->dest) {
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
    if (graph == NULL) {
        printErrorOnFile("graph_persistence.c", __LINE__, "graph_load_txt: graph is NULL");
        return;
    }
    if (filename == NULL) {
        printErrorOnFile("graph_persistence.c", __LINE__, "graph_load_txt: filename is NULL");
        return;
    }
    if (get_id_by_name == NULL) {
        printErrorOnFile("graph_persistence.c", __LINE__, "graph_load_txt: get_id_by_name callback is NULL");
        return;
    }
    FILE *file = fopen(filename, "r");
    if (!file) {
        printErrorOnFile("graph_persistence.c", __LINE__, "graph_load_txt: cannot open file for reading");
        return;
    }
    char line[512];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n' || strncmp(line, "Origen", 6) == 0) continue;
        line[strcspn(line, "\n")] = 0;
        char *arrow = strstr(line, "->");
        if (!arrow) {
            printErrorOnFile("graph_persistence.c", __LINE__, "graph_load_txt: missing '->' in line");
            continue;
        }
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
                    graph_add_edge(graph, src_id, dest_id, 0, 0);
                } else {
                    printErrorOnFile("graph_persistence.c", __LINE__, "graph_load_txt: destination name not found in mapping");
                }
                token = strtok(NULL, ",");
            }
        } else {
            printErrorOnFile("graph_persistence.c", __LINE__, "graph_load_txt: source name not found in mapping");
        }
    }
    fclose(file);
}
