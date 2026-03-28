#include "list.h"
#include "error.h"
#include <stdlib.h>

LinkedList* list_create() {
    LinkedList *list = (LinkedList*)malloc(sizeof(LinkedList));
    if (list == NULL) {
        printErrorOnFile("list.c", __LINE__, "list_create: malloc(LinkedList) failed");
        return NULL;
    }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

void list_add_front(LinkedList *list, void *data) {
    if (list == NULL) {
        printErrorOnFile("list.c", __LINE__, "list_add_front: list is NULL");
        return;
    }
    NODE *new_node = create_node(data);
    if (!new_node) {
        printErrorOnFile("list.c", __LINE__, "list_add_front: create_node failed");
        return;
    }
    if (list->head == NULL) {
        list->head = list->tail = new_node;
    } else {
        new_node->next = list->head;
        list->head->prev = new_node;
        list->head = new_node;
    }
    list->size++;
}

void list_add_back(LinkedList *list, void *data) {
    if (list == NULL) {
        printErrorOnFile("list.c", __LINE__, "list_add_back: list is NULL");
        return;
    }
    NODE *new_node = create_node(data);
    if (!new_node) {
        printErrorOnFile("list.c", __LINE__, "list_add_back: create_node failed");
        return;
    }
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
    if (list == NULL) {
        printErrorOnFile("list.c", __LINE__, "list_destroy: list is NULL");
        return;
    }
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
    if (list == NULL) {
        printErrorOnFile("list.c", __LINE__, "list_remove_node: list is NULL");
        return;
    }
    if (node == NULL) {
        printErrorOnFile("list.c", __LINE__, "list_remove_node: node is NULL");
        return;
    }
    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
    }
    if (destroy_data != NULL) {
        destroy_data(node->data);
    }
    destroy_node(node);
    list->size--;
}

NODE* list_find(LinkedList *list, void *data, int (*compare)(void*, void*)) {
    if (list == NULL) {
        printErrorOnFile("list.c", __LINE__, "list_find: list is NULL");
        return NULL;
    }
    if (compare == NULL) {
        printErrorOnFile("list.c", __LINE__, "list_find: compare function is NULL");
        return NULL;
    }
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
    if (list == NULL) {
        printErrorOnFile("list.c", __LINE__, "list_is_empty: list is NULL");
        return true;
    }
    return (list->head == NULL);
}

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

NODE* merge_sort_recursive(NODE *head, int (*compare)(void*, void*)) {
    if (!head || !head->next) return head;
    NODE *second = split(head);
    head = merge_sort_recursive(head, compare);
    second = merge_sort_recursive(second, compare);
    return merge(head, second, compare);
}

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
    if (list == NULL) {
        printErrorOnFile("list.c", __LINE__, "list_foreach: list is NULL");
        return;
    }
    if (action == NULL) {
        printErrorOnFile("list.c", __LINE__, "list_foreach: action callback is NULL");
        return;
    }
    NODE *curr = list->head;
    while (curr != NULL) {
        action(curr->data);
        curr = curr->next;
    }
}
