#include <stdio.h>
#include <string.h>
#include "list.h"
#include "stack.h"
#include "queue.h"
#include "error.h"

void list_save_txt(LinkedList *list, const char *filename, void (*write_func)(FILE*, void*)) {
    if (list == NULL) {
        printErrorOnFile("list_persistence.c", __LINE__, "list_save_txt: list is NULL");
        return;
    }
    if (filename == NULL) {
        printErrorOnFile("list_persistence.c", __LINE__, "list_save_txt: filename is NULL");
        return;
    }
    if (write_func == NULL) {
        printErrorOnFile("list_persistence.c", __LINE__, "list_save_txt: write_func callback is NULL");
        return;
    }
    FILE *file = fopen(filename, "w");
    if (!file) {
        printErrorOnFile("list_persistence.c", __LINE__, "list_save_txt: cannot open file for writing");
        return;
    }
    fprintf(file, "# LINEAR STRUCTURE EXPORT (Total: %zu entries)\n", list->size);
    NODE *curr = list->head;
    while (curr) {
        write_func(file, curr->data);
        fprintf(file, "\n");
        curr = curr->next;
    }
    fclose(file);
}

void list_load_txt(LinkedList *list, const char *filename, void* (*parse_func)(char*)) {
    if (list == NULL) {
        printErrorOnFile("list_persistence.c", __LINE__, "list_load_txt: list is NULL");
        return;
    }
    if (filename == NULL) {
        printErrorOnFile("list_persistence.c", __LINE__, "list_load_txt: filename is NULL");
        return;
    }
    if (parse_func == NULL) {
        printErrorOnFile("list_persistence.c", __LINE__, "list_load_txt: parse_func callback is NULL");
        return;
    }
    FILE *file = fopen(filename, "r");
    if (!file) {
        printErrorOnFile("list_persistence.c", __LINE__, "list_load_txt: cannot open file for reading");
        return;
    }
    char line[256];
    int index = 0;
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || line[0] == '\n') continue;
        line[strcspn(line, "\n")] = 0;
        void *data = parse_func(line);
        if (data) {
            list_add_back(list, data, index);
            index++;
        } else {
            printErrorOnFile("list_persistence.c", __LINE__, "list_load_txt: parse_func failed for line");
        }
    }
    fclose(file);
}
