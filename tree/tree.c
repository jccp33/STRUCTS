#include "tree.h"

BinaryTree* tree_create(int (*compare)(void*, void*)) {
    BinaryTree *tree = malloc(sizeof(BinaryTree));
    tree->root = NULL;
    tree->compare = compare;
    return tree;
}

static TreeNode* insert_recursive(TreeNode *node, void *data, int (*compare)(void*, void*)) {
    if (node == NULL) {
        TreeNode *new_node = malloc(sizeof(TreeNode));
        new_node->data = data;
        new_node->left = new_node->right = NULL;
        return new_node;
    }
    if (compare(data, node->data) < 0) {
        node->left = insert_recursive(node->left, data, compare);
    } else {
        node->right = insert_recursive(node->right, data, compare);
    }
    return node;
}

void tree_insert(BinaryTree *tree, void *data) {
    tree->root = insert_recursive(tree->root, data, tree->compare);
}

static void inorder_recursive(TreeNode *node, void (*action)(void*)) {
    if (node == NULL) return;
    inorder_recursive(node->left, action);
    action(node->data);
    inorder_recursive(node->right, action);
}

void tree_inorder(BinaryTree *tree, void (*action)(void*)) {
    inorder_recursive(tree->root, action);
}

static void destroy_recursive(TreeNode *node, void (*destroy_data)(void*)) {
    if (node == NULL) return;
    destroy_recursive(node->left, destroy_data);
    destroy_recursive(node->right, destroy_data);
    if (destroy_data != NULL) {
        destroy_data(node->data);
    }
    free(node);
}

void tree_destroy(BinaryTree *tree, void (*destroy_data)(void*)) {
    if (!tree) return;
    destroy_recursive(tree->root, destroy_data);
    free(tree);
}
