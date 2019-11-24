#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "map.h"


int main(void) {
    int num_versions = 100;
    int num_buckets = 7;
    PersistentDS *persistentMap = initialize_map_with_element(5, 25, 100, 7);
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
    map_update(persistentMap,5,30,3);
    print_hash(persistentMap, 4);
    //non existent key
    map_update(persistentMap,11,20,3);
    //Bad Version
    print_hash(persistentMap, 5);
    //Delete
    map_delete(persistentMap,250,4);
    print_hash(persistentMap, 5);
    map_delete(persistentMap,10,5);
    print_hash(persistentMap, 6);
    map_delete(persistentMap,5,6);
    print_hash(persistentMap, 7);
    //Now everything is deleted. Try deleting more stuff
    map_delete(persistentMap,5,7);
    print_hash(persistentMap, 8);

    printf("\n");
    versionGraph* versionG = generate_version_graph(persistentMap);
    print_version_graph(versionG);
    versionIndex *parents_list =get_parents_list(persistentMap,7);
    print_parents_list(parents_list,7);
    print_instruction_list(parents_list,7,persistentMap);


}