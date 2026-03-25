#include "list.h"
#include <stdlib.h>

LinkedList* list_create() {
    LinkedList *list = (LinkedList*)malloc(sizeof(LinkedList));
    if (list) {
        list->head = NULL;
        list->tail = NULL;
        list->size = 0;
    }
    return list;
}

void list_add_front(LinkedList *list, void *data) {
    NODE *new_node = create_node(data);
    if (!new_node) return;
    if(list->head == NULL){
        list->head = list->tail = new_node;
    }else{
        new_node->next = list->head;
        list->head->prev = new_node;
        list->head = new_node;
    }
    list->size++;
}

void list_add_back(LinkedList *list, void *data) {
    NODE *new_node = create_node(data);
    if (!new_node) return;
    if (list->tail == NULL) {
        list->head = list->tail = new_node;
    } else {
        new_node->prev = list->tail;
        list->tail->next = new_node;
        list->tail = new_node;
    }
    list->size++;
}

void list_destroy(LinkedList *list, void (*destroy_data)(void*)) {
    NODE *current = list->head;
    while (current != NULL) {
        NODE *next = current->next;
        if (destroy_data != NULL) {
            destroy_data(current->data);
        }
        destroy_node(current);
        current = next;
    }
    free(list);
}

void list_remove_node(LinkedList *list, NODE *node, void (*destroy_data)(void*)) {
    if (list == NULL || node == NULL) return;
    // 1. Ajustar el puntero del nodo anterior
    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }
    // 2. Ajustar el puntero del nodo siguiente
    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
    }
    // 3. Liberar memoria
    if (destroy_data != NULL) {
        destroy_data(node->data);
    }
    destroy_node(node);
    list->size--;
}

NODE* list_find(LinkedList *list, void *data, int (*compare)(void*, void*)) {
    NODE *current = list->head;
    while (current != NULL) {
        if (compare(current->data, data) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

bool list_is_empty(LinkedList *list) {
    return (list == NULL || list->head == NULL);
}

// Función auxiliar: Mezcla dos listas ya ordenadas
NODE* merge(NODE *first, NODE *second, int (*compare)(void*, void*)) {
    if (!first) return second;
    if (!second) return first;
    if (compare(first->data, second->data) <= 0) {
        first->next = merge(first->next, second, compare);
        first->next->prev = first;
        first->prev = NULL;
        return first;
    } else {
        second->next = merge(first, second->next, compare);
        second->next->prev = second;
        second->prev = NULL;
        return second;
    }
}

// Función auxiliar: Divide la lista en dos mitades usando la técnica de 'slow and fast'
NODE* split(NODE *head) {
    NODE *fast = head, *slow = head;
    while (fast->next && fast->next->next) {
        fast = fast->next->next;
        slow = slow->next;
    }
    NODE *temp = slow->next;
    slow->next = NULL;
    if (temp) temp->prev = NULL;
    return temp;
}

// Función recursiva principal
NODE* merge_sort_recursive(NODE *head, int (*compare)(void*, void*)) {
    if (!head || !head->next) return head;
    NODE *second = split(head);
    head = merge_sort_recursive(head, compare);
    second = merge_sort_recursive(second, compare);
    return merge(head, second, compare);
}

// Función pública para tu librería
void list_sort(LinkedList *list, int (*compare)(void*, void*)) {
    if (!list || list->size < 2) return;
    list->head = merge_sort_recursive(list->head, compare);
    NODE *temp = list->head;
    while (temp && temp->next) {
        temp = temp->next;
    }
    list->tail = temp;
}

void list_foreach(LinkedList *list, void (*action)(void*)) {
    if (!list || !action) return;    
    NODE *curr = list->head;
    while (curr != NULL) {
        action(curr->data);
        curr = curr->next;
    }
}
