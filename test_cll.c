#include <stdio.h>
#include <stdlib.h>

#include "cll.h"

int main(void) {
    PersistentDS* persistentCLL = initialize_cll_with_element(11,100);
    print_cll(persistentCLL,0);
    cll_add(persistentCLL,21,0);
    print_cll(persistentCLL,1);
    cll_add(persistentCLL,31,0);
    print_cll(persistentCLL,2);
    cll_add(persistentCLL,31,1);
    print_cll(persistentCLL,3);
    //Wrong version
    print_cll(persistentCLL,19);
    //Read
    for (int version = 0; version < 4; ++version) {
        for (int indx = 0; indx < 4; ++indx) {
            int val = cll_read(persistentCLL,indx,version);
            if(val!=INT_MIN){
                printf("Element no %d in Version no %d is %d\n",indx,version,val);
            }
        }
    }
    // Update
    cll_update(persistentCLL,0,41,3);
    print_cll(persistentCLL,4);
    //Wrong Version No
    cll_update(persistentCLL,0,41,6);
    //Wrong Index
    cll_update(persistentCLL,10,41,3);
    cll_update(persistentCLL,2,51,4);
    print_cll(persistentCLL,5);
    cll_add(persistentCLL,71,5);
    cll_add(persistentCLL,81,6);
    print_cll(persistentCLL,7);
    //Test Delete
    cll_delete(persistentCLL,1,7);
    print_cll(persistentCLL,8);
    cll_delete(persistentCLL,3,8);
    print_cll(persistentCLL,9);
    cll_delete(persistentCLL,0,9);
    print_cll(persistentCLL,10);
    //Wrong Version
    cll_delete(persistentCLL,3,12);
    //Wrong Index
    cll_delete(persistentCLL,3,10);
    cll_delete(persistentCLL,2,10);
    //Working Index
    cll_delete(persistentCLL,0,10);
    print_cll(persistentCLL,11);
    cll_delete(persistentCLL,0,11);
    print_cll(persistentCLL,12);
    cll_delete(persistentCLL,0,12);


    printf("\n");
    versionGraph* versionG = generate_version_graph(persistentCLL);
    print_version_graph(versionG);
    versionIndex *parents_list =get_parents_list(persistentCLL,12);
    print_parents_list(parents_list,12);
    print_instruction_list(parents_list,12,persistentCLL);

}