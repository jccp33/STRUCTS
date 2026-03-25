#include <stdio.h>
#include "heap.h"

void heap_save_txt(Heap *heap, const char *filename, void (*write_func)(FILE*, void*)) {
    FILE *file = fopen(filename, "w");
    if (!file) return;
    fprintf(file, "# ESTADO DEL HEAP (Prioridad 0 es la raíz)\n");
    fprintf(file, "Prioridad,Dato\n");
    for (size_t i = 0; i < heap->size; i++) {
        fprintf(file, "%zu,", i);
        write_func(file, heap->data[i]);
        fprintf(file, "\n");
    }
    fclose(file);
}
