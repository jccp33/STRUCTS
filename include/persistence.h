#ifndef PERSISTENCE_H
#define PERSISTENCE_H

typedef enum {
    TYPE_LIST,      // 0
    TYPE_STACK,     // 1
    TYPE_QUEUE,     // 2
    TYPE_HASHTABLE, // 3
    TYPE_GRAPH      // 4
} StructureType;

typedef struct {
    void *data;
    StructureType type;
} LoadedStructure;

typedef void* (*GenericParser)(char*);

LoadedStructure load_from_file(const char *filename, StructureType type, GenericParser parser);

#endif
