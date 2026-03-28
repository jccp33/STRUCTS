CC = gcc
# Añadimos -fPIC para compatibilidad universal con Shared Objects
CFLAGS = -Wall -Wextra -Iinclude -fPIC
OBJ_DIR = build
BIN_DIR = bin
LIB_DIR = lib

# 1. Lista de archivos fuente
SRCS = error/error.c \
	   node/node.c \
       list/list.c list/list_persistence.c \
       stack/stack.c \
       queue/queue.c \
       hashtable/hashtable.c hashtable/ht_persistence.c \
       tree/tree.c tree/tree_persistence.c \
       heap/heap.c heap/heap_persistence.c \
       graph/graph.c graph/graph_algorithms.c graph/graph_persistence.c \
       persistence/persistence.c \
       main.c

OBJS = $(SRCS:%.c=$(OBJ_DIR)/%.o)

TARGET = test_library
TARGET_DLL = $(BIN_DIR)/structs.dll
TARGET_SO = $(BIN_DIR)/libstructs.so
TARGET_LIB = $(LIB_DIR)/libstructs.a

all: $(TARGET)

# Enlazado del ejecutable de prueba
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
	@echo "--------------------------------------------------"
	@echo "¡Compilación exitosa! Ejecutable: $(TARGET)"
	@echo "--------------------------------------------------"

# Compilación de objetos (Crea carpetas automáticamente)
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# --- DISTRIBUCIÓN WINDOWS ---
dll: $(OBJS)
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(LIB_DIR)
	$(CC) -shared -o $(TARGET_DLL) $(OBJS) -Wl,--out-implib,$(TARGET_LIB)
	@echo "[OK] Windows: DLL en $(BIN_DIR) y Lib de importación en $(LIB_DIR)"

# --- DISTRIBUCIÓN LINUX ---
so: $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) -shared -o $(TARGET_SO) $(OBJS)
	@echo "[OK] Linux: Shared Object generado en $(TARGET_SO)"

# --- LIMPIEZA ---
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR) $(TARGET)
	@echo "Limpieza completada."

.PHONY: all clean dll so
