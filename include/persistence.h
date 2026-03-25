#ifndef PERSISTENCE_H
#define PERSISTENCE_H
#include "list.h"
#include "hashtable.h"
#include "graph.h"

typedef enum {
    TYPE_LIST,      // 0
    TYPE_GRAPH,     // 1
    TYPE_HASHTABLE, // 2
    TYPE_TREE,      // 3
    TYPE_HEAP       // 4
} StructureType;

typedef struct {
    void *data;
    StructureType type;
} LoadedStructure;

typedef void* (*GenericParser)(char*);

LoadedStructure load_from_file(const char *filename, StructureType type, GenericParser parser);

#endif
