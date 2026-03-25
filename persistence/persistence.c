#include <stddef.h>
#include <stdbool.h>
#include "persistence.h"
#include "list.h"
#include "hashtable.h"
#include "graph.h"

LoadedStructure load_from_file(const char *filename, StructureType type, void* (*parser)(char*)) {
    LoadedStructure result;
    result.type = type;
    result.data = NULL;
    switch (type) {
        case TYPE_LIST:
        case TYPE_STACK:
        case TYPE_QUEUE:
            result.data = list_create();
            list_load_txt((LinkedList*)result.data, filename, parser);
            break;
        case TYPE_HASHTABLE:
            result.data = ht_create(hash_djb2);
            ht_load_txt((HashTable*)result.data, filename, parser);
            break;
        case TYPE_GRAPH:
            result.data = graph_create(4, false);
            int (*graph_parser)(char*) = (int (*)(char*))(void*)parser;
            graph_load_txt((Graph*)result.data, filename, graph_parser);
            break;
        case TYPE_TREE:
            // TODO: Implementar carga de árbol desde archivo
            break;
        case TYPE_HEAP:
            // TODO: Implementar carga de montículo desde archivo
            break;
        default:
            break;
    }
    return result;
}
