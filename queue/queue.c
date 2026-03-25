#include "queue.h"

Queue* queue_create() {
    return list_create();
}

void queue_enqueue(Queue *queue, void *data) {
    list_add_back(queue, data);
}

void* queue_dequeue(Queue *queue) {
    if (queue_is_empty(queue)) return NULL;
    void *data = queue->head->data;
    list_remove_node(queue, queue->head, NULL);
    return data;
}

void* queue_peek(Queue *queue) {
    if (queue_is_empty(queue)) return NULL;
    return queue->head->data;
}

bool queue_is_empty(Queue *queue) {
    return list_is_empty(queue);
}

void queue_destroy(Queue *queue, void (*destroy_data)(void*)) {
    list_destroy(queue, destroy_data);
}
