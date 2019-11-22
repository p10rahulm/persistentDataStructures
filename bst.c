#include <time.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "bst.h"

// We have 2 operations, insert, search and remove


BSTNode *createBSTNode(int elemVal, BSTNode *left_child, BSTNode *right_child, BSTNode *parent) {
    BSTNode *out = calloc(1, sizeof(BSTNode));
    out->value = elemVal;
    out->leftChild = left_child;
    out->rightChild = right_child;
    return out;
}

int getNDeleteInorderSuccessor(BSTNode *root) {
    return recursiveNDeleteInOrderSuccessor(root->rightChild);
}

int recursiveNDeleteInOrderSuccessor(BSTNode *currNode) {
    if (!currNode->leftChild) {
        currNode->parent.leftChild = currNode->rightChild;
        int out = currNode->value;
        free(currNode);
        return currNode->value;
    } else {
        return recursiveInorderSucessor(currNode->leftChild);
    }
}

BSTNode *deleteElem(BSTNode *elemToDelete) {
    if (elemToDelete->leftChild && elemToDelete->rightChild) {
        int newElemVal = getNDeleteInorderSuccessor(elemToDelete);
        elemToDelete->value = newElemVal;
        return elemToDelete;
    }
    if (!elemToDelete->leftChild && elemToDelete->rightChild) {
        BSTNode* rightChild = elemToDelete->rightChild
        rightChild->parent = elemToDelete->parent;
        free(elemToDelete);
        return rightChild;
    }
    if (elemToDelete->leftChild && !elemToDelete->rightChild) {
        BSTNode* leftChild = elemToDelete->leftChild;
        leftChild->parent = elemToDelete->parent;
        free(elemToDelete);
        return leftChild;
    }
    free(elemToDelete);
    return NULL;
}

void addValuetoBST(BSTNode* root, int value_to_add){
    if(root->value>value_to_add){
        if(!root->leftChild){
            root->leftChild = createBSTNode(value_to_add,NULL,NULL,root);
        } else {
            addValuetoBST(root->leftChild,value_to_add);
        }
    } else {
        if(!root->rightChild){
            root->rightChild = createBSTNode(value_to_add,NULL,NULL,root);
        } else {
            addValuetoBST(root->rightChild,value_to_add);
        }
    }
}

PersistentDS *initialize_persistent_BST(int num_versions) {
    PersistentDS *initialized = create_persistent_ds(num_versions);
    snprintf(initialized->versions[0].description, 100, "Base Version number: %d", 0);
    initialized->versions[0].structure_head = NULL;
    return initialized;
}

PersistentDS *initialize_BST_with_element(int elemVal, int num_versions) {
    PersistentDS *out = initialize_persistent_BST(num_versions);
    out->versions[0].structure_head = createBSTNode(elemVal,NULL,NULL,NULL);
    BSTNode* structure =out->versions[0].structure_head;
    structure->parent = structure;
    out->last_updated_version_number = 0;
    return out;
}


BSTNode* copyThatTree(BSTNode* thatTreeRoot, BSTNode* thisTreeParent){
    BSTNode* out = createBSTNode(thatTreeRoot->value,NULL,NULL,NULL);
    out->parent = thatTreeParent;
    if(thatTreeRoot->leftChild){
        out->leftChild = copyThatTree(thatTreeRoot->leftChild,out);
    }
    if(thatTreeRoot->rightChild){
        out->rightChild = copyThatTree(thatTreeRoot->rightChild,out);
    }
    return out;
}

void BSTVersionCopy(PersistentDS *input, int srcVersion) {
    input->last_updated_version_number++;

    input->versions[input->last_updated_version_number].parent_version_number = srcVersion;
    input->versions[input->last_updated_version_number].time_of_last_update = time(0);
    input->versions[input->last_updated_version_number].time_of_last_access = time(0);
    snprintf(input->versions[input->last_updated_version_number].description, 100, "Version Number: %d",
             input->last_updated_version_number);


    BSTNode *last_root = input->versions[srcVersion].structure_head;
    input->versions[input->last_updated_version_number].structure_head = copyThatTree(last_root, NULL);
    BSTNode *curr_root = input->versions[input->last_updated_version_number].structure_head;
    curr_root->parent = curr_root;

}


//BELOW FUNCTION FOR PRINTING TREE TAKEN FROM STACKOVERFLOW: Copyright "user1571409"
int printTreeRecursive(BSTNode *tree, int is_left, int offset, int depth, char s[20][255]){
    char b[20];
    int width = 5;

    if (!tree) return 0;

    sprintf(b, "(%03d)", tree->value);

    int left  = printTreeRecursive(tree->leftChild,  1, offset,                depth + 1, s);
    int right = printTreeRecursive(tree->rightChild, 0, offset + left + width, depth + 1, s);

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
            s[2 * depth - 1][offset + left + width/2 + i] = '-';

        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset + left + width + right + width/2] = '+';

    } else if (depth && !is_left) {

        for (int i = 0; i < left + width; i++)
            s[2 * depth - 1][offset - width/2 + i] = '-';

        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset - width/2 - 1] = '+';
    }
    #endif

    return left + width + right;
}

//BELOW FUNCTION FOR PRINTING TREE TAKEN FROM STACKOVERFLOW: Copyright "user1571409"
void printTree(BSTNode *tree){
    char s[20][255];
    for (int i = 0; i < 20; i++)
        sprintf(s[i], "%80s", " ");

    printTreeRecursive(tree, 0, 0, 0, s);

    for (int i = 0; i < 20; i++)
        printf("%s\n", s[i]);
}

void print_bst(PersistentDS *input, int version_num) {
    if (version_num < 0 || version_num > input->last_updated_version_number) {
        printf("Please check version to print. The suggested version no: %d does not exist\n", version_num);
        return;
    }
    int last_updated_version = input->last_updated_version_number;
    printf("\n--------------------------------------------------------------------------------\n");
    printf("Last Updated Version Num = %d\n", last_updated_version);
    if (version_num > last_updated_version) { printf("So many versions do not exist\n"); }


    VersionNode* requiredVersion = &(input->versions[version_num]);
    printVersionNodeDetails(requiredVersion);

    BSTNode* root = requiredVersion->structure_head;
    printTree(root);
    printf("\n--------------------------------------------------------------------------------\n");
}


void bst_add(PersistentDS *input, int elemVal, int srcVersion) {
    if (input->num_versions == input->last_updated_version_number + 1) {
        printf("You have reached the limit of number of versions you can create");
        return;
    }
    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("The version you want to change does not exist");
        return;
    }

    BSTVersionCopy(input, srcVersion);
    BSTNode* currRoot =  input->versions[input->last_updated_version_number].structure_head;
    if(currRoot){
        addValuetoBST(currRoot,elemVal);
    } else{
        BSTNode* newNode = createBSTNode(elemVal,NULL,NULL,NULL);
        newNode->parent = newNode;
        input->versions[input->last_updated_version_number].structure_head = newNode;
    }
}

//int map_read(PersistentDS *input, int elemKey, int srcVersion) {
//    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
//        printf("Please check your Version number input\n");
//        return INT_MIN;
//    }
//    Map *structure = input->versions[srcVersion].structure_head;
//    int bucket_num = getBucketNum(elemKey, structure->num_buckets);
//    if (structure->buckets[bucket_num].num_in_bucket == 0) {
//        printf("Please check the key entered. It wasn't found in the map.\n");
//        return INT_MIN;
//    }
//    mapElem *rover = structure->buckets[bucket_num].head;
//    while (rover) {
//        if (rover->element_key == elemKey) {
//            input->versions->time_of_last_access = time(0);
//            return rover->element_value;
//        }
//        rover = rover->next;
//    }
//    printf("Please check the key entered. It wasn't found in the map.\n");
//    return INT_MIN;
//}
//
//
//void map_update(PersistentDS *input, int elemKey, int elemVal, int srcVersion) {
//    if (input->num_versions == input->last_updated_version_number + 1) {
//        printf("You have reached the limit of number of versions you can create");
//        return;
//    }
//
//    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
//        printf("Please check your Version number input\n");
//        return;
//    }
//    Map *structure = input->versions[srcVersion].structure_head;
//    int bucket_num = getBucketNum(elemKey, structure->num_buckets);
//    if (structure->buckets[bucket_num].num_in_bucket == 0) {
//        printf("Please check the key entered. It wasn't found in the map.\n");
//        return;
//    }
//    int found = 0;
//    mapElem *rover = structure->buckets[bucket_num].head;
//    while (rover) {
//        if (rover->element_key == elemKey) {
//            found = 1;
//            break;
//        }
//        rover = rover->next;
//    }
//    if (!found) {
//        printf("Please check the key entered. It wasn't found in the map.\n");
//        return;
//    }
//    mapVersionCopy(input, srcVersion);
//    Map *current_structure = input->versions[input->last_updated_version_number].structure_head;
//
//    rover = current_structure->buckets[bucket_num].head;
//    while (rover) {
//        if (rover->element_key == elemKey) {
//            rover->element_value = elemVal;
//            return;
//        }
//        rover = rover->next;
//    }
//
//}
//
//void map_delete(PersistentDS *input, int elemKeyToDelete, int srcVersion) {
//    if (input->num_versions == input->last_updated_version_number + 1) {
//        printf("You have reached the limit of number of versions you can create");
//        return;
//    }
//
//    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
//        printf("Please check your Version number input\n");
//        return;
//    }
//    Map *structure = input->versions[srcVersion].structure_head;
//    int bucket_num = getBucketNum(elemKeyToDelete, structure->num_buckets);
//    if (structure->buckets[bucket_num].num_in_bucket == 0) {
//        printf("Please check the key entered. It wasn't found in the map.\n");
//        return;
//    }
//    int found = 0;
//    mapElem *rover = structure->buckets[bucket_num].head;
//    while (rover) {
//        if (rover->element_key == elemKeyToDelete) {
//            found = 1;
//            break;
//        }
//        rover = rover->next;
//    }
//    if (!found) {
//        printf("Please check the key entered. It wasn't found in the map.\n");
//        return;
//    }
//
//
//    mapVersionCopy(input, srcVersion);
//    Map *current_structure = input->versions[input->last_updated_version_number].structure_head;
//    rover = current_structure->buckets[bucket_num].head;
//    mapElem *roverlast = NULL;
//
//    while (rover) {
//        if (rover->element_key == elemKeyToDelete) {
//            if (!roverlast) {
//                current_structure->buckets[bucket_num].head = rover->next;
//            } else {
//                roverlast->next = rover->next;
//            }
//            free(rover);
//            return;
//        }
//        roverlast = rover;
//        rover = rover->next;
//    }
//
//}
//
//
