#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "error.h"
#include "node.h"
#include "list.h"
#include "stack.h"
#include "queue.h"
#include "hashtable.h"
#include "tree.h"
#include "heap.h"
#include "graph.h"
#include "persistence.h"

extern uint32_t hash_djb2(const char* str);

int main() {
    printf("--- MYLANG: Inicializando Tabla de Simbolos ---\n");

    HashTable* symbol_table = ht_create(hash_djb2);
    if (symbol_table) {
        ht_insert(symbol_table, "var", (void*)101); 
        printf("[OK] Keyword 'var' registrada con ID 101.\n");
        printf("[OK] Libreria lista para compartir.\n");
    } else {
        printErrorOnFile("main.c", __LINE__, "Fallo al vincular STRUCTS.");
    }

    printf("-----------------------------------------------\n");
    
    return 0;
}
