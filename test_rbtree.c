#include <stdio.h>
#include <stdlib.h>

#include "rbtree.h"

void main() {

//    RBTree* tree = createNewEmptyTree();
//
//    printf("### RED-BLACK TREE INSERT ###\n\n");
//
//    int num_keys, scannedElemVal;
//    printf("Number of keys to add: ");
//    scanf("%d", &num_keys);
//    while(num_keys--){
//        printf("Enter value of element to add: ");
//        scanf("%d", &scannedElemVal);
//        red_black_insert(scannedElemVal,tree);
//        printRBTree(tree->root);
//    }
//    printf("Finished insertion\n");
//    printf("Printing the tree\n\n");
//    printRBTree(tree->root);
//    //tree_print(tree->root);
//    printf("\n");
//
//    printf("### KEY SEARCH ###\n\n");
//    printf("Enter elemVal: ");
//    scanf("%d", &scannedElemVal);
//    if(tree_search(scannedElemVal,tree)){
//        printf("Element you entered: %d, was found\n",tree_search(scannedElemVal,tree)->value);
//    } else {
//        printf("Element you entered: %d, was not found\n",scannedElemVal);
//    }
//
//    printf("Find the minimum\n");
//    printf("minimum: %d\n", (tree_minimum(tree->root))->value);
//
//    printf("Test Tree Deletion\n");
//    while(tree->root) {
//        printf("Enter elemVal to delete: ");
//        scanf("%d", &scannedElemVal);
//        if(tree_search(scannedElemVal,tree)){
//            red_black_delete(tree_search(scannedElemVal,tree),tree);
//        } else {
//            printf("Value you input not in tree\n");
//        }
//
//        printRBTree(tree->root);
//    }
//    printf("\n");
    PersistentDS *persistentRBTree = initialize_RBTree_with_element(11, 100);
    print_rbtree(persistentRBTree, 0);
    rbTreeAdd(persistentRBTree, 21, 0);
    print_rbtree(persistentRBTree, 1);
    rbTreeAdd(persistentRBTree, 31, 0);
    print_rbtree(persistentRBTree, 2);
    rbTreeAdd(persistentRBTree, 31, 1);
    print_rbtree(persistentRBTree, 3);

    rbTreeAdd(persistentRBTree, -9, 3);
    print_rbtree(persistentRBTree, 4);
    rbTreeAdd(persistentRBTree, 41, 4);
    print_rbtree(persistentRBTree, 5);
    rbTreeAdd(persistentRBTree, -19, 5);
    print_rbtree(persistentRBTree, 6);
    rbTreeAdd(persistentRBTree, 51, 6);
    print_rbtree(persistentRBTree, 7);

    for (int numberToSearch = -29; numberToSearch < 100; numberToSearch += 10) {
        int search, version = 7;
        search = rbTree_search(persistentRBTree, numberToSearch, version);
        if (search) printf("The Value you searched is %d in version %d was found\n", numberToSearch, version);
        else
            printf("The Value you searched is %d in version %d was not found\n", numberToSearch, version);
    }
    int versions_deleted =0;
    for (int numberToDelete = -29; numberToDelete < 60; numberToDelete += 10) {
        int deleted, version = 7;
        deleted = rbTree_delete(persistentRBTree, numberToDelete, version+ versions_deleted);
        if (deleted) {
            versions_deleted++;
            print_rbtree(persistentRBTree, 7 + versions_deleted);
        } else {
            printf("%d is not part of the Red Black Tree. Nothing to delete\n", numberToDelete);
        }
    }
    print_version_graph(generate_version_graph(persistentRBTree));
}
