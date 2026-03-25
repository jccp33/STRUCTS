#include <stdio.h>
#include <string.h>
#include "hashtable.h"

void ht_save_txt(HashTable *ht, const char *filename, void (*write_value)(FILE*, void*)) {
    FILE *file = fopen(filename, "w");
    if (!file) return;
    fprintf(file, "# ESTRUCTURA HASH TABLE (Tamaño: %zu, Elementos: %zu)\n", ht->size, ht->count);
    fprintf(file, "Bucket,Key,Value\n");
    for (size_t i = 0; i < ht->size; i++) {
        if (ht->buckets[i] != NULL) {
            NODE *curr = ht->buckets[i]->head;
            while (curr) {
                HTEntry *entry = (HTEntry*)curr->data;
                fprintf(file, "%zu,%s,", i, entry->key);
                write_value(file, entry->value);
                fprintf(file, "\n");
                curr = curr->next;
            }
        }
    }
    fclose(file);
}

void ht_load_txt(HashTable *ht, const char *filename, void* (*parse_value)(char*)) {
    FILE *file = fopen(filename, "r");
    if (!file) return;
    char line[512];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || strncmp(line, "Bucket", 6) == 0 || line[0] == '\n') continue;
        line[strcspn(line, "\n")] = 0;
        strtok(line, ","); // Saltar bucket ID
        char *key = strtok(NULL, ",");
        char *remaining = strtok(NULL, ""); 
        if (key && remaining) {
            void *value = parse_value(remaining);
            if (value) ht_insert(ht, key, value);
        }
    }
    fclose(file);
}
