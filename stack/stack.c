#include "stack.h"

Stack* stack_create() {
    return list_create();
}

void stack_push(Stack *stack, void *data) {
    list_add_front(stack, data);
}

void* stack_pop(Stack *stack) {
    if (stack_is_empty(stack)) return NULL;
    void *data = stack->head->data;
    list_remove_node(stack, stack->head, NULL);
    return data;
}

void* stack_peek(Stack *stack) {
    if (stack_is_empty(stack)) return NULL;
    return stack->head->data;
}

bool stack_is_empty(Stack *stack) {
    return list_is_empty(stack);
}

void stack_destroy(Stack *stack, void (*destroy_data)(void*)) {
    list_destroy(stack, destroy_data);
}
