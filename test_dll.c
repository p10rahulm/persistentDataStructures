#include <stdio.h>
#include <stdlib.h>

#include "dll.h"

int main(void) {
    PersistentDS* persistentDLL = initialize_dll_with_element(11,100);
    print_dll(persistentDLL,0);
    dll_add(persistentDLL,21,0);
    print_dll(persistentDLL,1);
    dll_add(persistentDLL,31,0);
    print_dll(persistentDLL,2);
    dll_add(persistentDLL,31,1);
    print_dll(persistentDLL,3);
    //Wrong version
    print_dll(persistentDLL,19);
    //Read
    for (int version = 0; version < 4; ++version) {
        for (int indx = 0; indx < 4; ++indx) {
            int val = dll_read(persistentDLL,indx,version);
            if(val!=INT_MIN){
                printf("Element no %d in Version no %d is %d\n",indx,version,val);
            }
        }
    }
    // Update
    dll_update(persistentDLL,0,41,3);
    print_dll(persistentDLL,4);
    //Wrong Version No
    dll_update(persistentDLL,0,41,6);
    //Wrong Index
    dll_update(persistentDLL,10,41,3);
    dll_update(persistentDLL,2,51,4);
    print_dll(persistentDLL,5);
    dll_add(persistentDLL,71,5);
    dll_add(persistentDLL,81,6);
    print_dll(persistentDLL,7);
    //Test Delete
    dll_delete(persistentDLL,1,7);
    print_dll(persistentDLL,8);
    dll_delete(persistentDLL,3,8);
    print_dll(persistentDLL,9);
    dll_delete(persistentDLL,0,9);
    print_dll(persistentDLL,10);
    //Wrong Version
    dll_delete(persistentDLL,3,12);
    //Wrong Index
    dll_delete(persistentDLL,3,10);
    dll_delete(persistentDLL,2,10);
    //Working Index
    dll_delete(persistentDLL,0,10);
    print_dll(persistentDLL,11);
    dll_delete(persistentDLL,0,11);
    print_dll(persistentDLL,12);
    dll_delete(persistentDLL,0,12);

}