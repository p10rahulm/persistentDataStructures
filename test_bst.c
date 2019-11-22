#include <stdio.h>
#include <stdlib.h>

#include "bst.h"

int main(void) {
    PersistentDS *persistentBST = initialize_BST_with_element(11, 100);
    print_bst(persistentBST, 0);
    bst_add(persistentBST, 21, 0);
    print_bst(persistentBST, 1);
    bst_add(persistentBST, 7, 0);
    print_bst(persistentBST, 2);
    bst_add(persistentBST, 7, 1);
    print_bst(persistentBST, 3);
    bst_add(persistentBST, 45, 3);
    print_bst(persistentBST, 4);
    bst_add(persistentBST, 19, 4);
    print_bst(persistentBST, 5);
    for (int numberToSearch = 5; numberToSearch < 22; ++numberToSearch) {
        int search, version = 5;
        search = bst_search(persistentBST, numberToSearch, version);
        if (search) printf("The Value you searched is %d in version %d was found\n", numberToSearch, version);
        else
            printf("The Value you searched is %d in version %d was not found\n", numberToSearch, version);
    }
    int deleted,item_to_delete;
    print_bst(persistentBST, 5);
    item_to_delete = 19;
    deleted = bst_delete(persistentBST,item_to_delete,5);
    if(deleted){print_bst(persistentBST, 6);} else{printf("Nothing to delete");}
    //Delete non existent node
    item_to_delete = 20;
    deleted = bst_delete(persistentBST,item_to_delete,6);
    if(deleted){print_bst(persistentBST, 7);} else{printf("%d is not part of the BST. Nothing to delete",item_to_delete);}
    //Delete root
    item_to_delete = 11;
    deleted = bst_delete(persistentBST,item_to_delete,6);
    if(deleted){print_bst(persistentBST, 7);} else{printf("%d is not part of the BST. Nothing to delete",item_to_delete);}
    //Delete root
    item_to_delete = 21;
    deleted = bst_delete(persistentBST,item_to_delete,7);
    if(deleted){print_bst(persistentBST, 8);} else{printf("%d is not part of the BST. Nothing to delete",item_to_delete);}
    //Delete root
    item_to_delete = 45;
    deleted = bst_delete(persistentBST,item_to_delete,8);
    if(deleted){print_bst(persistentBST, 9);} else{printf("%d is not part of the BST. Nothing to delete",item_to_delete);}
    //Delete root
    item_to_delete = 7;
    deleted = bst_delete(persistentBST,item_to_delete,9);
    if(deleted){print_bst(persistentBST, 10);} else{printf("%d is not part of the BST. Nothing to delete",item_to_delete);}
    //Delete root
    item_to_delete = 12;
    deleted = bst_delete(persistentBST,item_to_delete,9);
    if(deleted){print_bst(persistentBST, 10);} else{printf("%d is not part of the BST. Nothing to delete",item_to_delete);}
    //Add
    bst_add(persistentBST, 21, 10);
    print_bst(persistentBST, 11);


}