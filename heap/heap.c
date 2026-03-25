#include "heap.h"

Heap* heap_create(size_t capacity, int (*compare)(void*, void*)) {
    Heap *heap = malloc(sizeof(Heap));
    heap->capacity = capacity;
    heap->size = 0;
    heap->compare = compare;
    heap->data = malloc(sizeof(void*) * capacity);
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
    if (heap->size == heap->capacity) {
        heap->capacity *= 2;
        heap->data = realloc(heap->data, sizeof(void*) * heap->capacity);
    }
    heap->data[heap->size] = data;
    sift_up(heap, heap->size);
    heap->size++;
}

void* heap_pop(Heap *heap) {
    if (heap->size == 0) return NULL;
    void *top = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    sift_down(heap, 0);
    return top;
}

void* heap_peek(Heap *heap) {
    if (heap == NULL || heap->size == 0) {
        return NULL;
    }
    return heap->data[0];
}

bool heap_is_empty(Heap *heap) {
    if (heap == NULL) return true;
    return heap->size == 0;
}

void heap_destroy(Heap *heap, void (*destroy_data)(void*)) {
    if (heap == NULL) return;
    if (destroy_data != NULL) {
        for (size_t i = 0; i < heap->size; i++) {
            destroy_data(heap->data[i]);
        }
    }
    free(heap->data);
    free(heap);
}
