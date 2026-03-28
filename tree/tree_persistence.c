#include <stdio.h>
#include "tree.h"
#include "error.h"

static void tree_save_recursive(TreeNode *node, FILE *file, void (*write_func)(FILE*, void*)) {
    if (node == NULL) return;
    tree_save_recursive(node->left, file, write_func);
    write_func(file, node->data);
    tree_save_recursive(node->right, file, write_func);
}

void tree_save_txt(BinaryTree *tree, const char *filename, void (*write_func)(FILE*, void*)) {
    if (tree == NULL) {
        printErrorOnFile("tree_persistence.c", __LINE__, "tree_save_txt: tree is NULL");
        return;
    }
    if (filename == NULL) {
        printErrorOnFile("tree_persistence.c", __LINE__, "tree_save_txt: filename is NULL");
        return;
    }
    if (write_func == NULL) {
        printErrorOnFile("tree_persistence.c", __LINE__, "tree_save_txt: write_func is NULL");
        return;
    }
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printErrorOnFile("tree_persistence.c", __LINE__, "tree_save_txt: fopen failed");
        return;
    }
    if (!file) return;
    fprintf(file, "# EXPORTACIÓN DE ÁRBOL BINARIO (ORDENADO)\n");
    tree_save_recursive(tree->root, file, write_func);
    fclose(file);
}
