#include "hashtable.h"
#include <string.h>

static void ht_resize(HashTable *ht);

uint32_t hash_djb2(const char *str) {
    uint32_t hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

HashTable* ht_create(uint32_t (*h_func)(const char*)) {
    HashTable *ht = malloc(sizeof(HashTable));
    ht->size = TABLE_SIZE;
    ht->count = 0;
    ht->hash_func = h_func ? h_func : hash_djb2;
    ht->buckets = calloc(ht->size, sizeof(LinkedList*));
    return ht;
}

void ht_insert(HashTable *ht, const char *key, void *data) {
    if ((float)(ht->count + 1) / ht->size > 0.7) {
        ht_resize(ht);
    }
    uint32_t index = ht->hash_func(key) % ht->size;
    if (ht->buckets[index] == NULL) {
        ht->buckets[index] = list_create();
    }
    HTEntry *entry = malloc(sizeof(HTEntry));
    entry->key = strdup(key);
    entry->value = data;
    list_add_back(ht->buckets[index], entry);
    ht->count++;
}

void* ht_get(HashTable *ht, const char *key, int (*compare)(void*, void*)) {
    if (ht == NULL || key == NULL) return NULL;
    uint32_t index = ht->hash_func(key) % ht->size;
    if (ht->buckets[index] == NULL) return NULL;
    NODE *curr = ht->buckets[index]->head;
    while (curr != NULL) {
        HTEntry *entry = (HTEntry*)curr->data;
        if (compare != NULL) {
            if (compare(entry->value, (void*)key) == 0) return entry->value;
        } else {
            if (strcmp(entry->key, key) == 0) return entry->value;
        }
        curr = curr->next;
    }
    return NULL;
}

static void ht_resize(HashTable *ht) {
    size_t old_size = ht->size;
    size_t new_size = old_size * 2;
    LinkedList **new_buckets = calloc(new_size, sizeof(LinkedList*));
    for (size_t i = 0; i < old_size; i++) {
        if (ht->buckets[i] != NULL) {
            NODE *curr = ht->buckets[i]->head;
            while (curr != NULL) {
                HTEntry *entry = (HTEntry*)curr->data;
                uint32_t new_index = ht->hash_func(entry->key) % new_size;
                if (new_buckets[new_index] == NULL) {
                    new_buckets[new_index] = list_create();
                }
                list_add_back(new_buckets[new_index], entry);
                curr = curr->next;
            }
            list_destroy(ht->buckets[i], NULL);
        }
    }
    free(ht->buckets);
    ht->buckets = new_buckets;
    ht->size = new_size;
}

static void free_ht_entry(void *data) {
    if (data == NULL) return;
    HTEntry *entry = (HTEntry*)data;
    if (entry->key) free(entry->key);
    free(entry);
}

void ht_destroy(HashTable *ht, void (*destroy_data)(void*)) {
    if (!ht) return;
    for (size_t i = 0; i < ht->size; i++) {
        if (ht->buckets[i] != NULL) {
            if (destroy_data != NULL) {
                NODE *curr = ht->buckets[i]->head;
                while (curr) {
                    HTEntry *e = (HTEntry*)curr->data;
                    destroy_data(e->value);
                    curr = curr->next;
                }
            }
            list_destroy(ht->buckets[i], free_ht_entry);
        }
    }
    free(ht->buckets);
    free(ht);
}
