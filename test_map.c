#include <stdio.h>
#include <stdlib.h>

#include "map.h"


int main(void) {
    int num_versions = 100;
    int num_buckets = 7;
    PersistentDS *persistentMap = initialize__map_with_element(5, 25, 100, 7);
    print_hash(persistentMap, 0);
    map_add(persistentMap, 10,20, 0);
    print_hash(persistentMap, 1);
    //Key Exists
    map_add(persistentMap, 5,21, 1);
    //Printing Non Existent version
    print_hash(persistentMap, 2);
    //Adding to old version
    map_add(persistentMap, 250,7, 0);
    print_hash(persistentMap, 2);
    map_add(persistentMap, 250,7, 1);
    print_hash(persistentMap, 3);
    //Read value
    int readvalue,key,version;
    key=250;version=3;
    readvalue = map_read(persistentMap, key, version);
    if(readvalue!=INT_MIN){        printf("The Value associated with key %d is %d\n",key,readvalue);    }
    key=99;version=3;
    readvalue = map_read(persistentMap, key, version);
    if(readvalue!=INT_MIN){        printf("The Value associated with key %d is %d\n",key,readvalue);    }
    // Bad version Check
    key=250;version=30;
    readvalue = map_read(persistentMap, key, version);
    if(readvalue!=INT_MIN){        printf("The Value associated with key %d is %d\n",key,readvalue);    }
    //Update the map




}