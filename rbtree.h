#ifndef PERSISTENCEFOREXISTENCE_RBTREE_H
#define PERSISTENCEFOREXISTENCE_RBTREE_H

#include "persistence.h"


typedef struct red_black_tree_node {
    int value;
    int color;
    struct red_black_tree_node *parent;
    struct red_black_tree_node *leftChild;
    struct red_black_tree_node *rightChild;
} RBTreeNode;

typedef struct RBTree {
    int num_elements;
    RBTreeNode *root;
} RBTree;

void left_rotate(RBTreeNode *x, RBTree *tree);

void right_rotate(RBTreeNode *x, RBTree *tree);

void tree_print(RBTreeNode *x);

void red_black_insert(int elemVal, RBTree *tree);

void red_black_insert_fixup(RBTreeNode *z, RBTree *tree);

RBTreeNode *tree_search(int elemVal, RBTree *tree);

RBTreeNode *tree_minimum(RBTreeNode *x);

void red_black_transplant(RBTreeNode *u, RBTreeNode *v, RBTree *tree);

void red_black_delete(RBTreeNode *z, RBTree *tree);

void red_black_delete_fixup(RBTreeNode *x, RBTree *tree);

RBTree *createNewEmptyTree();

void printRBTree(RBTreeNode *treeNode);


PersistentDS *initialize_RBTree_with_element(int elemVal, int num_versions);

void print_rbtree(PersistentDS *input, int version_num);

void rbTreeAdd(PersistentDS *input, int elemVal, int srcVersion);

int rbTree_search(PersistentDS *input, int elemVal, int srcVersion);

int rbTree_delete(PersistentDS *input, int elemVal, int srcVersion);

#endif //PERSISTENCEFOREXISTENCE_RBTREE_H
