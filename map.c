#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "map.h"

typedef struct map_element {
    int element_key;
    int element_value
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


Map *init_map(int num_buckets) {
    Map *out = (Map *) calloc(1, sizeof(Map));
    out->num_buckets = num_buckets;
    out->buckets = calloc(num_buckets, sizeof(Bucket));
    for (int i = 0; i < num_buckets; ++i) {
        out->buckets[i].head = NULL;
        out->buckets[i].num_in_bucket = 0;
    }
    return out;
}

mapElem *createNode(int elemKey, int elemVal, mapElem *nextElem) {
    mapElem *out = calloc(1, sizeof(mapElem));
    out->element_key = elemKey;
    out->element_value = elemVal;
    out->next = nextElem;
    return out;
}

int getBucketNum(int elemVal, int numBuckets) {
    return elemVal % numBuckets;
}


PersistentDS *initialize_persistent_hashmap(int num_versions, int num_buckets) {
    PersistentDS *initialized = create_persistent_ds(num_versions);
    snprintf(initialized->versions[0].description, 100, "Base Version number: %d", 0);
    initialized->versions[0].structure_head = init_map(
    int num_buckets);
    return initialized;
}

void add_to_hash(Map *structure, int bucket_num, int elemKey, int elemVal) {
    structure->buckets[bucket_num].head;
    mapElem *newnode = createNode(elemKey, elemVal, structure->buckets[bucket_num].head)
    structure->buckets[bucket_num].head = newnode;
    structure->buckets[bucket_num].num_in_bucket++;
    return;
}

PersistentDS *initialize_with_element(int elemKey, int elemVal, int num_versions, int num_buckets) {
    PersistentDS *out = initialize_persistent_hashmap(num_versions, num_elements);
    Map *structure = out->versions[0].structure_head;
    int bucket_num = getBucketNum(element, num_buckets);
    add_to_hash(structure, bucket_num, elemKey, elemVal)
    out->last_updated_version_number = 0;
    return out;
}


void vectorVersionCopy(PersistentDS *input, int srcVersion, int destVersion) {
    input->last_updated_version_number++;

    input->versions[input->last_updated_version_number].parent_version_number = srcVersion;
    input->versions[input->last_updated_version_number].time_of_last_update = time(0);
    input->versions[input->last_updated_version_number].time_of_last_access = time(0);
    snprintf(input->versions[input->last_updated_version_number].description, 100, "Version Number: %d", destVersion);


    Map *last_structure = input->versions[srcVersion].structure_head;
    input->versions[input->last_updated_version_number].structure_head = init_map(last_structure->num_buckets);
    Map *current_structure = input->versions[input->last_updated_version_number].structure_head;

    current_structure->num_buckets = last_structure->num_buckets;
    for (int i = 0; i < current_structure->num_buckets; ++i) {
        current_structure->buckets[i].num_in_bucket = last_structure->buckets[i].num_in_bucket;
        mapElem* lastRover = last_structure->buckets[i].head;
        mapElem* currNode,currPrev;

        int index = 0;
        if(!lastRover){ continue;}
        while(lastRover){
            mapElem* currNode = createNode(lastRover->element_key,lastRover->element_value,NULL);
            if(index==0){
                current_structure->buckets[i].head = currNode;
            } else{
                currPrev.next = currNode;
            }
            currPrev = currNode;
            index++;
            lastRover = lastRover->next;
        }
    }
}


void print_hash(PersistentDS *input, int version_num) {
    if (version_num < 0 || version_num > input->last_updated_version_number) {
        printf("Please check version to print. The suggested version no: %d does not exist\n", version_num);
        return;
    }
    int last_updated_version = input->last_updated_version_number;
    printf("\n--------------------------------------------------------------------------------\n");
    printf("Last Updated Version Num = %d\n", last_updated_version);
    if (version_num > last_updated_version) { printf("So many versions do not exist\n"); }


    VersionNode *requiredVersion = &(input->versions[version_num]);
    printVersionNodeDetails(requiredVersion);


    Map *structure = requiredVersion->structure_head;

    int num_buckets = structure->num_buckets;
    printf("Num Buckets: %d\n", num_buckets);

    for (int i = 0; i < num_buckets; ++i) {
        mapElem *rover = structure->buckets[i].head;
        printf("\nBucketNum: %d", i);
        if(!rover){ continue;}
        while (rover) {
            printf("(key=%d,val=%d)\t", rover->element_key, rover->element_value);
            rover = rover->next;
        }
    }
    printf("\n--------------------------------------------------------------------------------\n");
}


void vector_add(PersistentDS *input, int element, int srcVersion) {
    if (input->num_versions == input->last_updated_version_number + 1) {
        printf("You have reached the limit of number of versions you can create");
        return;
    }

    Vector *last_structure = input->versions[srcVersion].structure_head;
    if (last_structure->num_elements == last_structure->last_index + 1) {
        printf("The Vector is full. You cannot add more elements. Please increase the array size, or delete some elements and retry");
        return;
    }
    if (srcVersion > input->last_updated_version_number) {
        printf("The version you want to change does not exist");
        return;
    }

    vectorVersionCopy(input, srcVersion, input->last_updated_version_number + 1);
    Vector *current_structure = input->versions[input->last_updated_version_number].structure_head;
    int *current_elem_array = current_structure->elements_array;
    current_structure->last_index++;
    current_elem_array[current_structure->last_index] = element;
}

int vector_read(PersistentDS *input, int element_index, int srcVersion) {
    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("Please check your Version number input\n");
        return 0;
    }
    Vector *current_structure = input->versions[srcVersion].structure_head;
    input->versions[srcVersion].time_of_last_access = time(0);
    if (current_structure->last_index > element_index || element_index < 0) {
        printf("Please check your index number\n");
        return 0;
    }
    int *elem_array = current_structure->elements_array;
    return elem_array[element_index];
}

void vector_update(PersistentDS *input, int index_to_update, int updated_element, int srcVersion) {
    if (input->num_versions == input->last_updated_version_number + 1) {
        printf("You have reached the limit of number of versions you can create");
        return;
    }
    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("The version you want to change does not exist");
        return;
    }

    Vector *source_structure = input->versions[srcVersion].structure_head;
    if (source_structure->last_index < index_to_update || index_to_update < 0) {
        printf("Please check your index number\n");
        return;
    }

    vectorVersionCopy(input, srcVersion, input->last_updated_version_number + 1);
    Vector *current_structure = input->versions[input->last_updated_version_number].structure_head;
    int *current_elem_array = current_structure->elements_array;

    current_elem_array[index_to_update] = updated_element;
}

void vector_delete(PersistentDS *input, int index_to_delete, int srcVersion) {
    if (input->num_versions == input->last_updated_version_number + 1) {
        printf("You have reached the limit of number of versions you can create");
        return;
    }
    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("The version you want to change does not exist");
        return;
    }

    Vector *source_structure = input->versions[srcVersion].structure_head;
    if (source_structure->last_index < index_to_delete || index_to_delete < 0) {
        printf("Please check your index number\n");
        return;
    }

    vectorVersionCopy(input, srcVersion, input->last_updated_version_number + 1);
    Vector *current_structure = input->versions[input->last_updated_version_number].structure_head;
    int *current_elem_array = current_structure->elements_array;

    for (int i = index_to_delete; i < current_structure->last_index; ++i) {
        current_elem_array[i] = current_elem_array[i + 1];
    }
    current_elem_array[current_structure->last_index] = 0;
    current_structure->last_index--;
}


