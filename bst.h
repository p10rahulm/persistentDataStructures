#ifndef PERSISTENCEFOREXISTENCE_BST_H
#define PERSISTENCEFOREXISTENCE_BST_H

#include "persistence.h"
typedef struct bstNode {
    int value;
    struct bstNode leftChild;
    struct bstNode rightChild;
    struct bstNode parent; //we store parent for easy traversal
} BSTNode;



PersistentDS *initialize_BST_with_element(int elemVal, int num_versions);
void print_bst(PersistentDS *input, int version_num);
void bst_add(PersistentDS *input, int elemVal, int srcVersion);

#endif //PERSISTENCEFOREXISTENCE_BST_H
