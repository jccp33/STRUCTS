#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hashtable.h"
#include "error.h"

void ht_save_txt(HashTable *ht, const char *filename, void (*write_value)(FILE*, void*)) {
    if (ht == NULL) {
        printErrorOnFile("ht_persistence.c", __LINE__, "ht_save_txt: ht is NULL");
        return;
    }
    if (filename == NULL) {
        printErrorOnFile("ht_persistence.c", __LINE__, "ht_save_txt: filename is NULL");
        return;
    }
    if (write_value == NULL) {
        printErrorOnFile("ht_persistence.c", __LINE__, "ht_save_txt: write_value callback is NULL");
        return;
    }
    FILE *file = fopen(filename, "w");
    if (!file) {
        printErrorOnFile("ht_persistence.c", __LINE__, "ht_save_txt: cannot open file for writing");
        return;
    }
    fprintf(file, "# ESTRUCTURA HASH TABLE (Tamaño: %zu, Elementos: %zu)\n", ht->size, ht->count);
    fprintf(file, "Bucket,Key,Value\n");
    for (size_t i = 0; i < ht->size; i++) {
        if (ht->buckets[i] != NULL) {
            NODE *curr = ht->buckets[i]->head;
            while (curr) {
                HTEntry *entry = (HTEntry*)curr->data;
                if (entry == NULL || entry->key == NULL) {
                    printErrorOnFile("ht_persistence.c", __LINE__, "ht_save_txt: invalid entry in bucket");
                    curr = curr->next;
                    continue;
                }
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
    if (ht == NULL) {
        printErrorOnFile("ht_persistence.c", __LINE__, "ht_load_txt: ht is NULL");
        return;
    }
    if (filename == NULL) {
        printErrorOnFile("ht_persistence.c", __LINE__, "ht_load_txt: filename is NULL");
        return;
    }
    if (parse_value == NULL) {
        printErrorOnFile("ht_persistence.c", __LINE__, "ht_load_txt: parse_value callback is NULL");
        return;
    }
    FILE *file = fopen(filename, "r");
    if (!file) {
        printErrorOnFile("ht_persistence.c", __LINE__, "ht_load_txt: cannot open file for reading");
        return;
    }
    char line[512];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' || strncmp(line, "Bucket", 6) == 0 || line[0] == '\n') continue;
        line[strcspn(line, "\n")] = 0;
        //char *bucketId = strtok(line, ","); // Saltar bucket ID
        char *key = strtok(NULL, ",");
        char *remaining = strtok(NULL, ""); 
        if (key && remaining) {
            void *value = parse_value(remaining);
            if (value) {
                if (ht == NULL) {
                    printErrorOnFile("ht_persistence.c", __LINE__, "ht_load_txt: ht became NULL during processing");
                    break;
                }
                ht_insert(ht, key, value);
            } else {
                printErrorOnFile("ht_persistence.c", __LINE__, "ht_load_txt: parse_value failed");
            }
        } else {
            printErrorOnFile("ht_persistence.c", __LINE__, "ht_load_txt: invalid line format");
        }
    }
    fclose(file);
}
