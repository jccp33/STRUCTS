#include <stdio.h>
#include <string.h>
#include "list.h"
#include "stack.h"
#include "queue.h"

void list_save_txt(LinkedList *list, const char *filename, void (*write_func)(FILE*, void*)) {
    if (!list || !filename || !write_func) return;
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error al abrir el archivo de lista");
        return;
    }
    fprintf(file, "# EXPORTACION DE ESTRUCTURA LINEAL (Total: %zu elementos)\n", list->size);
    NODE *curr = list->head;
    while (curr) {
        write_func(file, curr->data);
        fprintf(file, "\n");
        curr = curr->next;
    }
    fclose(file);
}

void list_load_txt(LinkedList *list, const char *filename, void* (*parse_func)(char*)) {
    FILE *file = fopen(filename, "r");
    if (!file) return;
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n') continue;
        line[strcspn(line, "\n")] = 0;
        void *data = parse_func(line);
        if (data) list_add_back(list, data);
    }
    fclose(file);
}
