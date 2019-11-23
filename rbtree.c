#include <time.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "rbtree.h"


//RED = 0
//BLACK = 1

RBTreeNode *createRBTreeNode(int val, int color, RBTreeNode *leftChild, RBTreeNode *rightChild, RBTreeNode *parent) {
    RBTreeNode *out = calloc(1, sizeof(RBTreeNode));
    out->value = val;
    out->parent = parent;
    out->leftChild = leftChild;
    out->rightChild = rightChild;
    out->color = color;
    return out;
}

RBTree *createNewEmptyTree() {
    RBTree *outTree = calloc(1, sizeof(RBTree));
    outTree->num_elements = 0;
    outTree->root = NULL;
    return outTree;
}

void initializeTreeWithElement(int element, RBTree *tree) {
    tree->root = calloc(1, sizeof(RBTreeNode));
    tree->root->value = element;
    tree->root->parent = NULL; // parent of root is NULL
    tree->root->color = 1;
    tree->num_elements = 1;
}

RBTree *createTreeWithElement(int elemVal) {
    RBTree *outTree = createNewEmptyTree();
    initializeTreeWithElement(elemVal, outTree);
    return outTree;
}


void tree_print(RBTreeNode *out) {
    if (out != NULL) {
        tree_print(out->leftChild);
        printf("%d\t", out->value);
        tree_print(out->rightChild);
    }
}

RBTreeNode *tree_search(int elemVal, RBTree *tree) {
    RBTreeNode *rover = tree->root;
    while (rover && rover->value != elemVal) {
        if (elemVal < rover->value) {
            rover = rover->leftChild;
        } else {
            rover = rover->rightChild;
        }
    }
    return rover;
}

RBTreeNode *tree_minimum(RBTreeNode *treeRoot) {
    while (treeRoot->leftChild != NULL) {
        treeRoot = treeRoot->leftChild;
    }
    return treeRoot;
}


void red_black_insert(int elemVal, RBTree *tree) {
    RBTreeNode *newNode, *rover, *pre_rover;

    newNode = calloc(1, sizeof(RBTreeNode));
    newNode->value = elemVal;
    newNode->color = 0;
    newNode->leftChild = NULL;
    newNode->rightChild = NULL;

    rover = tree->root;
    pre_rover = NULL;

    /*
     * Go through the tree untill a leaf(NULL) is reached. pre_rover is used for keeping
     * track of the last non-NULL node which will be newNode's parent.
     */
    while (rover) {
        pre_rover = rover;
        if (newNode->value <= rover->value) {
            rover = rover->leftChild;
        } else {
            rover = rover->rightChild;
        }
    }
    if (!pre_rover) {
        tree->root = newNode;
    } else if (newNode->value <= pre_rover->value) {
        pre_rover->leftChild = newNode;
    } else {
        pre_rover->rightChild = newNode;
    }
    newNode->parent = pre_rover;
    red_black_insert_fixup(newNode, tree);
    tree->num_elements++;
}


void red_black_insert_fixup(RBTreeNode *nodeToInsert, RBTree *tree) {
    while (nodeToInsert->parent && nodeToInsert->parent->color == 0) {
        /* nodeToInsert's parent is left child of nodeToInsert's grand parent*/
        if (nodeToInsert->parent->parent && nodeToInsert->parent == nodeToInsert->parent->parent->leftChild) {

            /* nodeToInsert's grand parent's right child is RED */
            if (nodeToInsert->parent->parent->rightChild && nodeToInsert->parent->parent->rightChild->color == 0) {
                nodeToInsert->parent->color = 1;
                nodeToInsert->parent->parent->rightChild->color = 1;
                nodeToInsert->parent->parent->color = 0;
                nodeToInsert = nodeToInsert->parent->parent;
            }

                /* nodeToInsert's grand parent's right child is not RED */
            else {

                /* nodeToInsert is nodeToInsert's parent's right child */
                if (nodeToInsert->parent->rightChild && nodeToInsert == nodeToInsert->parent->rightChild) {
                    nodeToInsert = nodeToInsert->parent;
                    left_rotate(nodeToInsert, tree);
                }

                nodeToInsert->parent->color = 1;
                nodeToInsert->parent->parent->color = 0;
                right_rotate(nodeToInsert->parent->parent, tree);
            }
        }

            /* nodeToInsert's parent is nodeToInsert's grand parent's right child */
        else {
            /* nodeToInsert's left uncle or nodeToInsert's grand parent's left child is also RED */
            if (nodeToInsert->parent->parent && nodeToInsert->parent->parent->leftChild &&
                nodeToInsert->parent->parent->leftChild->color == 0) {
                nodeToInsert->parent->color = 1;
                nodeToInsert->parent->parent->leftChild->color = 1;
                nodeToInsert->parent->parent->color = 0;
                nodeToInsert = nodeToInsert->parent->parent;
            } else {
                /* nodeToInsert is nodeToInsert's parents left child */
                if (nodeToInsert->parent->leftChild && nodeToInsert == nodeToInsert->parent->leftChild) {
                    nodeToInsert = nodeToInsert->parent;
                    right_rotate(nodeToInsert, tree);
                }

                nodeToInsert->parent->color = 1;
                nodeToInsert->parent->parent->color = 0;
                left_rotate(nodeToInsert->parent->parent, tree);
            }
        }
    }
    tree->root->color = 1;
}


//BELOW FUNCTION FOR PRINTING TREE TAKEN FROM STACKOVERFLOW: Copyright "user1571409"
int printRBTreeRecursive(RBTreeNode *treeNode, int is_left, int offset, int depth, char s[20][255]) {
    char b[20];
    int width = 5;

    if (!treeNode) return 0;

    sprintf(b, "(%03d)", treeNode->value);

    int left = printRBTreeRecursive(treeNode->leftChild, 1, offset, depth + 1, s);
    int right = printRBTreeRecursive(treeNode->rightChild, 0, offset + left + width, depth + 1, s);

#ifdef COMPACT
    for (int i = 0; i < width; i++)
        s[depth][offset + left + i] = b[i];

    if (depth && is_left) {

        for (int i = 0; i < width + right; i++)
            s[depth - 1][offset + left + width/2 + i] = '-';

        s[depth - 1][offset + left + width/2] = '.';

    } else if (depth && !is_left) {

        for (int i = 0; i < left + width; i++)
            s[depth - 1][offset - width/2 + i] = '-';

        s[depth - 1][offset + left + width/2] = '.';
    }
#else
    for (int i = 0; i < width; i++)
        s[2 * depth][offset + left + i] = b[i];

    if (depth && is_left) {

        for (int i = 0; i < width + right; i++)
            s[2 * depth - 1][offset + left + width / 2 + i] = '-';

        s[2 * depth - 1][offset + left + width / 2] = '+';
        s[2 * depth - 1][offset + left + width + right + width / 2] = '+';

    } else if (depth && !is_left) {

        for (int i = 0; i < left + width; i++)
            s[2 * depth - 1][offset - width / 2 + i] = '-';

        s[2 * depth - 1][offset + left + width / 2] = '+';
        s[2 * depth - 1][offset - width / 2 - 1] = '+';
    }
#endif

    return left + width + right;
}

//BELOW FUNCTION FOR PRINTING TREE TAKEN FROM STACKOVERFLOW: Copyright "user1571409"
void printRBTree(RBTreeNode *treeNode) {
    char s[20][255];
    for (int i = 0; i < 20; i++)
        sprintf(s[i], "%80s", " ");

    printRBTreeRecursive(treeNode, 0, 0, 0, s);

    for (int i = 0; i < 5; i++)
        printf("%s\n", s[i]);
}


void left_rotate(RBTreeNode *first, RBTree *tree) {
    RBTreeNode *second;

    /* Make second's left child first's right child */
    second = first->rightChild;
    first->rightChild = second->leftChild;
    if (second->leftChild) {
        second->leftChild->parent = first;
    }

    /* Make first's parent second's parent and second, first's parent's child */
    second->parent = first->parent;
    if (!second->parent) {
        tree->root = second;
    } else if (first->parent && first == first->parent->leftChild) {
        first->parent->leftChild = second;
    } else {
        if (first->parent) {
            first->parent->rightChild = second;
        }
    }
    /* Make first, second's left child & second, first's parent */
    second->leftChild = first;
    first->parent = second;
}


void right_rotate(RBTreeNode *first, RBTree *tree) {
    RBTreeNode *second;

    /* Make second's right child first's left child */
    second = first->leftChild;
    first->leftChild = second->rightChild;
    if (second->rightChild) {
        second->rightChild->parent = first;
    }

    /* Make first's parent second's parent and second, first's parent's child */
    second->parent = first->parent;
    if (!second->parent) {
        tree->root = second;
    } else if (first->parent && first == first->parent->leftChild) {
        first->parent->leftChild = second;
    } else {
        if (first->parent) {
            first->parent->rightChild = second;
        }
    }

    /* Make second, first's parent and first, second's child */
    second->rightChild = first;
    first->parent = second;
}

void red_black_delete(RBTreeNode *delNode, RBTree *tree) {
    RBTreeNode *rover, *rover2;
    int roverOriginalColor;

    rover = delNode;
    roverOriginalColor = rover->color;
    if (!delNode->leftChild) {
        rover2 = delNode->rightChild;
        red_black_transplant(delNode, delNode->rightChild, tree);
    } else if (!delNode->rightChild) {
        rover2 = delNode->leftChild;
        red_black_transplant(delNode, delNode->leftChild, tree);
    } else {
        rover = tree_minimum(delNode->rightChild);
        roverOriginalColor = rover->color;
        rover2 = rover->rightChild;
        if (rover->parent == delNode) {
            if (rover2) { rover2->parent = rover; }
        } else {
            red_black_transplant(rover, rover->rightChild, tree);
            rover->rightChild = delNode->rightChild;
            rover->rightChild->parent = rover;
        }
        red_black_transplant(delNode, rover, tree);
        rover->leftChild = delNode->leftChild;
        rover->leftChild->parent = rover;
        rover->color = delNode->color;
    }
    if (roverOriginalColor == 1) {
        red_black_delete_fixup(rover2, tree);
    }
    if (tree->num_elements){tree->num_elements--;}
}


void red_black_delete_fixup(RBTreeNode *nodeToFix, RBTree *tree) {
    RBTreeNode *otherNode;

    while (nodeToFix && nodeToFix != tree->root && nodeToFix->color == 1) {

        if (nodeToFix->parent && nodeToFix == nodeToFix->parent->leftChild) {
            otherNode = nodeToFix->parent->rightChild;

            if (otherNode && otherNode->color == 0) {
                otherNode->color = 1;
                nodeToFix->parent->color = 0;
                left_rotate(nodeToFix->parent, tree);
                otherNode = nodeToFix->parent->rightChild;
            }

            if (otherNode && otherNode->leftChild && otherNode->rightChild && otherNode->leftChild->color == 1 &&
                otherNode->rightChild->color == 1) {
                otherNode->color = 0;
                nodeToFix->parent->color = 1;
                nodeToFix = nodeToFix->parent;
            } else {

                if (otherNode && otherNode->rightChild && otherNode->rightChild->color == 1) {
                    otherNode->color = 0;
                    otherNode->leftChild->color = 1;
                    right_rotate(otherNode, tree);
                    otherNode = nodeToFix->parent->rightChild;
                }
                if (otherNode) { otherNode->color = nodeToFix->parent->color; }
                if (nodeToFix->parent) { nodeToFix->parent->color = 1; }
                if (nodeToFix->rightChild) { nodeToFix->rightChild->color = 1; }
                left_rotate(nodeToFix->parent, tree);
                nodeToFix = tree->root;

            }

        } else {
            if (nodeToFix->parent) {
                otherNode = nodeToFix->parent->leftChild;

                if (otherNode && otherNode->color == 0) {
                    otherNode->color = 1;
                    nodeToFix->parent->color = 1;
                    right_rotate(nodeToFix->parent, tree);
                    otherNode = nodeToFix->parent->leftChild;
                }

                if (otherNode && otherNode->leftChild->color == 1 && otherNode->rightChild->color == 1) {
                    otherNode->color = 0;
                    nodeToFix->parent->color = 1;
                    nodeToFix = nodeToFix->parent;
                } else {

                    if (otherNode && otherNode->leftChild->color == 1) {
                        otherNode->color = 0;
                        otherNode->rightChild->color = 1;
                        left_rotate(otherNode, tree);
                        otherNode = nodeToFix->parent->leftChild;
                    }
                    if (otherNode) {
                        otherNode->color = nodeToFix->parent->color;
                        otherNode->leftChild->color = 1;
                    }
                    nodeToFix->parent->color = 1;
                    right_rotate(nodeToFix->parent, tree);
                    nodeToFix = tree->root;
                }
            }
        }
    }
    if (nodeToFix) {
        nodeToFix->color = 1;
    }

}

/* replace node first with node second */
void red_black_transplant(RBTreeNode *first, RBTreeNode *second, RBTree *tree) {
    if (!first->parent) {
        tree->root = second;
    } else if (first->parent && first == first->parent->leftChild) {
        first->parent->leftChild = second;
    } else {
        if (first->parent) { first->parent->rightChild = second; }
    }
    if (second) { second->parent = first->parent; }

}


PersistentDS *initialize_persistent_RBTree(int num_versions) {
    PersistentDS *initialized = create_persistent_ds(num_versions);
    snprintf(initialized->versions[0].description, 100, "Base Version number: %d", 0);
    initialized->versions[0].structure_head = NULL;
    return initialized;
}

PersistentDS *initialize_RBTree_with_element(int elemVal, int num_versions) {
    PersistentDS *out = initialize_persistent_RBTree(num_versions);
    RBTree *structure = createNewEmptyTree();
    out->versions[0].structure_head = structure;
    red_black_insert(elemVal, structure);
    out->last_updated_version_number = 0;
    return out;
}

RBTreeNode *copyThatTreeRec(RBTreeNode *thatTreeRoot, RBTreeNode *thisTreeParent) {
    RBTreeNode *out = createRBTreeNode(thatTreeRoot->value, thatTreeRoot->color, NULL, NULL, thisTreeParent);
    if (thatTreeRoot->leftChild) {
        out->leftChild = copyThatTreeRec(thatTreeRoot->leftChild, out);
    }
    if (thatTreeRoot->rightChild) {
        out->rightChild = copyThatTreeRec(thatTreeRoot->rightChild, out);
    }
    return out;
}

void RBTreeVersionCopy(PersistentDS *input, int srcVersion) {
    input->last_updated_version_number++;

    input->versions[input->last_updated_version_number].parent_version_number = srcVersion;
    input->versions[input->last_updated_version_number].time_of_last_update = time(0);
    input->versions[input->last_updated_version_number].time_of_last_access = time(0);
    snprintf(input->versions[input->last_updated_version_number].description, 100, "Version Number: %d",
             input->last_updated_version_number);

    RBTree *last_tree = input->versions[srcVersion].structure_head;
    RBTree *curr_tree = createNewEmptyTree();
    if (last_tree->root) {
        curr_tree->root = copyThatTreeRec(last_tree->root, NULL);
    }

    input->versions[input->last_updated_version_number].structure_head = curr_tree;
}


void print_rbtree(PersistentDS *input, int version_num) {
    if (version_num < 0 || version_num > input->last_updated_version_number) {
        printf("Please check version to print. The suggested version no: %d does not exist\n", version_num);
        return;
    }
    int last_updated_version = input->last_updated_version_number;
    printf("\n--------------------------------------------------------------------------------\n");
    printf("Last Updated Version Num = %d\n", last_updated_version);
    if (version_num > last_updated_version) { printf("So many versions do not exist\n"); }


    VersionNode *requiredVersion = &(input->versions[version_num]);
    printVersionNodeDetails(requiredVersion);

    RBTree *tree = requiredVersion->structure_head;
    printf("No of nodes in tree is %d\n", tree->num_elements);
    if (tree->root) {
        printRBTree(tree->root);
    }
    printf("\n--------------------------------------------------------------------------------\n");
}


void rbTreeAdd(PersistentDS *input, int elemVal, int srcVersion) {
    if (input->num_versions == input->last_updated_version_number + 1) {
        printf("You have reached the limit of number of versions you can create");
        return;
    }
    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("The version you want to change does not exist");
        return;
    }
    RBTreeVersionCopy(input, srcVersion);
    RBTree *tree = input->versions[input->last_updated_version_number].structure_head;
    red_black_insert(elemVal, tree);
}


int rbTree_search(PersistentDS *input, int elemVal, int srcVersion) {
    if (input->num_versions == input->last_updated_version_number + 1) {
        printf("You have reached the limit of number of versions you can create");
        return 0;
    }
    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("The version you want to change does not exist");
        return 0;
    }

    RBTree *tree = input->versions[srcVersion].structure_head;
    if (!tree->root) { return 0; }
    return (tree_search(elemVal,tree)!=NULL);
}




int rbTree_delete(PersistentDS *input, int elemVal, int srcVersion) {
    if (input->num_versions == input->last_updated_version_number + 1) {
        printf("You have reached the limit of number of versions you can create");
        return 0;
    }
    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("The version you want to change does not exist");
        return 0;
    }
    if (rbTree_search(input, elemVal, srcVersion)) {
        RBTreeVersionCopy(input, srcVersion);
        RBTree *tree = input->versions[input->last_updated_version_number].structure_head;
        if (!tree->root) { return 0; }
        red_black_delete(tree_search(elemVal,tree),tree);
        return 1;

    }
    return 0;

}