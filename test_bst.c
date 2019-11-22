#include <stdio.h>
#include <stdlib.h>

#include "bst.h"

int main(void) {
    PersistentDS* persistentBST =  initialize_BST_with_element(11,100);
    print_bst(persistentBST,0);
}