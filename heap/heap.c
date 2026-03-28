#include "heap.h"
#include "error.h"
#include <stdlib.h>

Heap* heap_create(size_t capacity, int (*compare)(void*, void*)) {
    if (capacity == 0) {
        printErrorOnFile("heap.c", __LINE__, "heap_create: capacity must be > 0");
        return NULL;
    }
    if (compare == NULL) {
        printErrorOnFile("heap.c", __LINE__, "heap_create: compare callback is NULL");
        return NULL;
    }
    Heap *heap = malloc(sizeof(Heap));
    if (heap == NULL) {
        printErrorOnFile("heap.c", __LINE__, "heap_create: malloc(Heap) failed");
        return NULL;
    }
    heap->capacity = capacity;
    heap->size = 0;
    heap->compare = compare;
    heap->data = malloc(sizeof(void*) * capacity);
    if (heap->data == NULL) {
        printErrorOnFile("heap.c", __LINE__, "heap_create: malloc(data) failed");
        free(heap);
        return NULL;
    }
    return heap;
}

static void swap(void **a, void **b) {
    void *temp = *a;
    *a = *b;
    *b = temp;
}

static void sift_up(Heap *heap, size_t idx) {
    while (idx > 0) {
        size_t parent = (idx - 1) / 2;
        if (heap->compare(heap->data[idx], heap->data[parent]) < 0) {
            swap(&heap->data[idx], &heap->data[parent]);
            idx = parent;
        } else break;
    }
}

static void sift_down(Heap *heap, size_t idx) {
    size_t smallest = idx;
    size_t left = 2 * idx + 1;
    size_t right = 2 * idx + 2;
    if (left < heap->size && heap->compare(heap->data[left], heap->data[smallest]) < 0)
        smallest = left;
    if (right < heap->size && heap->compare(heap->data[right], heap->data[smallest]) < 0)
        smallest = right;
    if (smallest != idx) {
        swap(&heap->data[idx], &heap->data[smallest]);
        sift_down(heap, smallest);
    }
}

void heap_push(Heap *heap, void *data) {
    if (heap == NULL) {
        printErrorOnFile("heap.c", __LINE__, "heap_push: heap is NULL");
        return;
    }
    if (heap->compare == NULL) {
        printErrorOnFile("heap.c", __LINE__, "heap_push: comparator function is NULL");
        return;
    }
    if (heap->size == heap->capacity) {
        size_t new_capacity = heap->capacity == 0 ? 1 : heap->capacity * 2;
        void **new_data = realloc(heap->data, sizeof(void*) * new_capacity);
        if (new_data == NULL) {
            printErrorOnFile("heap.c", __LINE__, "heap_push: realloc failed");
            return;
        }
        heap->data = new_data;
        heap->capacity = new_capacity;
    }
    heap->data[heap->size] = data;
    sift_up(heap, heap->size);
    heap->size++;
}

void* heap_pop(Heap *heap) {
    if (heap == NULL) {
        printErrorOnFile("heap.c", __LINE__, "heap_pop: heap is NULL");
        return NULL;
    }
    if (heap->size == 0) {
        return NULL;
    }
    void *top = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    sift_down(heap, 0);
    return top;
}

void* heap_peek(Heap *heap) {
    if (heap == NULL) {
        printErrorOnFile("heap.c", __LINE__, "heap_peek: heap is NULL");
        return NULL;
    }
    if (heap->size == 0) {
        return NULL;
    }
    return heap->data[0];
}

bool heap_is_empty(Heap *heap) {
    return (heap == NULL);
}

void heap_destroy(Heap *heap, void (*destroy_data)(void*)) {
    if (heap == NULL) {
        printErrorOnFile("heap.c", __LINE__, "heap_destroy: heap is NULL");
        return;
    }
    if (destroy_data != NULL) {
        for (size_t i = 0; i < heap->size; i++) {
            destroy_data(heap->data[i]);
        }
    }
    free(heap->data);
    free(heap);
}
