#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include "error.h"

void heap_save_txt(Heap *heap, const char *filename, void (*write_func)(FILE*, void*)) {
    if (heap == NULL) {
        printErrorOnFile("heap_persistence.c", __LINE__, "heap_save_txt: heap is NULL");
        return;
    }
    if (filename == NULL) {
        printErrorOnFile("heap_persistence.c", __LINE__, "heap_save_txt: filename is NULL");
        return;
    }
    if (write_func == NULL) {
        printErrorOnFile("heap_persistence.c", __LINE__, "heap_save_txt: write_func callback is NULL");
        return;
    }
    FILE *file = fopen(filename, "w");
    if (!file) {
        printErrorOnFile("heap_persistence.c", __LINE__, "heap_save_txt: cannot open file for writing");
        return;
    }
    fprintf(file, "# HEAP STATE (Priority 0 is root)\n");
    fprintf(file, "Priority,Value\n");
    for (size_t i = 0; i < heap->size; i++) {
        if (heap->data == NULL) {
            printErrorOnFile("heap_persistence.c", __LINE__, "heap_save_txt: heap data array is NULL");
            break;
        }
        fprintf(file, "%zu,", i);
        write_func(file, heap->data[i]);
        fprintf(file, "\n");
    }
    fclose(file);
}
