#include <stdio.h>
#include <stdlib.h>
#include "ll.h"


int main(void) {
    PersistentDS* persistentLL = initialize_sll_with_element(11,100);
    print_sll(persistentLL,0);
    sll_add(persistentLL,21,0);
    print_sll(persistentLL,1);
    sll_add(persistentLL,31,0);
    print_sll(persistentLL,2);
    sll_add(persistentLL,31,1);
    print_sll(persistentLL,3);
    //Wrong version
    print_sll(persistentLL,19);
    //Read
    for (int version = 0; version < 4; ++version) {
        for (int indx = 0; indx < 4; ++indx) {
            int val = sll_read(persistentLL,indx,version);
            if(val!=INT_MIN){
                printf("Element no %d in Version no %d is %d\n",indx,version,val);
            }
        }
    }
    sll_update(persistentLL,0,41,3);
    print_sll(persistentLL,4);
    //Wrong Version No
    sll_update(persistentLL,0,41,6);
    //Wrong Index
    sll_update(persistentLL,10,41,3);
    sll_update(persistentLL,2,51,4);
    print_sll(persistentLL,5);
    sll_add(persistentLL,71,5);
    sll_add(persistentLL,81,6);
    print_sll(persistentLL,7);
    //Test Delete
    print_sll(persistentLL,7);
    sll_delete(persistentLL,1,7);
    print_sll(persistentLL,8);
    sll_delete(persistentLL,3,8);
    print_sll(persistentLL,9);
    sll_delete(persistentLL,0,9);
    print_sll(persistentLL,10);
    //Wrong Version
    sll_delete(persistentLL,3,12);
    //Wrong Index
    sll_delete(persistentLL,3,9);
}