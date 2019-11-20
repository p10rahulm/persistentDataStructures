#include <stdio.h>
#include <stdlib.h>

#include "map.h"


int main(void) {
    int num_versions = 100;
    int num_buckets = 7;
    PersistentDS *persistentMap = initialize__map_with_element(5, 25, 100, 7);
    print_hash(persistentMap, 0);
    map_add(persistentMap, 0,20, 0);
    print_hash(persistentMap, 1);



}