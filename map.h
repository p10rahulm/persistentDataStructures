#ifndef PERSISTENCEFOREXISTENCE_MAP_H
#define PERSISTENCEFOREXISTENCE_MAP_H

#include "persistence.h"

typedef struct map_element {
    int element_key;
    int element_value;
    struct map_element *next;
} mapElem;

typedef struct bucket {
    int num_in_bucket;
    mapElem *head;
} Bucket;

typedef struct map {
    int num_buckets;
    Bucket *buckets;
} Map;

PersistentDS *initialize_map_with_element(int elemKey, int elemVal, int num_versions, int num_buckets);
void print_hash(PersistentDS *input, int version_num);
void map_add(PersistentDS *input, int elemKey, int elemVal, int srcVersion);
int map_read(PersistentDS *input, int elemKey, int srcVersion);
void map_update(PersistentDS *input, int elemKey, int elemVal, int srcVersion);
void map_delete(PersistentDS *input, int index_to_delete, int srcVersion);

#endif //PERSISTENCEFOREXISTENCE_MAP_H
