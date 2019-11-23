#include <time.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "map.h"


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

mapElem *createMapElem(int elemKey, int elemVal, mapElem *nextElem) {
    mapElem *out = calloc(1, sizeof(mapElem));
    out->element_key = elemKey;
    out->element_value = elemVal;
    out->next = nextElem;
    return out;
}

int getBucketNum(int elemKey, int numBuckets) {
    return elemKey % numBuckets;
}


PersistentDS *initialize_persistent_hashmap(int num_versions, int num_buckets) {
    PersistentDS *initialized = create_persistent_ds(num_versions);
    snprintf(initialized->versions[0].description, 100, "Base Version number: %d", 0);
    initialized->versions[0].structure_head = init_map(num_buckets);
    return initialized;
}

void add_to_hash(Map *structure, int bucket_num, int elemKey, int elemVal) {
    mapElem *newnode = createMapElem(elemKey, elemVal, structure->buckets[bucket_num].head);
    structure->buckets[bucket_num].head = newnode;
    structure->buckets[bucket_num].num_in_bucket++;
    return;
}

PersistentDS *initialize_map_with_element(int elemKey, int elemVal, int num_versions, int num_buckets) {
    PersistentDS *out = initialize_persistent_hashmap(num_versions, num_buckets);
    Map *structure = out->versions[0].structure_head;
    out->versions[0].instruction=ADD_INSTRUCTION;
    out->versions[0].instruction_value = elemVal;
    out->versions[0].instruction_index = elemKey;

    int bucket_num = getBucketNum(elemKey, num_buckets);
    add_to_hash(structure, bucket_num, elemKey, elemVal);
    out->last_updated_version_number = 0;
    return out;
}


void mapVersionCopy(PersistentDS *input, int srcVersion) {
    input->last_updated_version_number++;

    input->versions[input->last_updated_version_number].parent_version_number = srcVersion;
    input->versions[input->last_updated_version_number].time_of_last_update = time(0);
    input->versions[input->last_updated_version_number].time_of_last_access = time(0);
    snprintf(input->versions[input->last_updated_version_number].description, 100, "Version Number: %d",
             input->last_updated_version_number);


    Map *last_structure = input->versions[srcVersion].structure_head;
    input->versions[input->last_updated_version_number].structure_head = init_map(last_structure->num_buckets);
    Map *current_structure = input->versions[input->last_updated_version_number].structure_head;

    current_structure->num_buckets = last_structure->num_buckets;
    for (int i = 0; i < current_structure->num_buckets; ++i) {
        current_structure->buckets[i].num_in_bucket = last_structure->buckets[i].num_in_bucket;
        mapElem *lastRover = last_structure->buckets[i].head;
        mapElem *currNode, *currPrev;

        int index = 0;
        if (!lastRover) { continue; }
        while (lastRover) {
            mapElem *currNode = createMapElem(lastRover->element_key, lastRover->element_value, NULL);
            if (index == 0) {
                current_structure->buckets[i].head = currNode;
            } else {
                currPrev->next = currNode;
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
        if (!rover) { continue; }
        while (rover) {
            printf("(key=%d,val=%d)\t", rover->element_key, rover->element_value);
            rover = rover->next;
        }
    }
    printf("\n--------------------------------------------------------------------------------\n");
}


void map_add(PersistentDS *input, int elemKey, int elemVal, int srcVersion) {
    if (input->num_versions == input->last_updated_version_number + 1) {
        printf("You have reached the limit of number of versions you can create");
        return;
    }
    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("The version you want to change does not exist");
        return;
    }

    Map *last_structure = input->versions[srcVersion].structure_head;
    int bucket_index = getBucketNum(elemKey, last_structure->num_buckets);

    // Check if element key already exists in the map
    int exists = 0;
    mapElem *lastRover = last_structure->buckets[bucket_index].head;

    while (lastRover) {
        if (lastRover->element_key == elemKey) {
            exists = 1;
            break;
        }
        lastRover = lastRover->next;
    }
    if (exists) {
        printf("The key already exists, unable to add\n");
        return;
    }

    mapVersionCopy(input, srcVersion);
    Map *current_structure = input->versions[input->last_updated_version_number].structure_head;
    add_to_hash(current_structure, bucket_index, elemKey, elemVal);

}

int map_read(PersistentDS *input, int elemKey, int srcVersion) {
    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("Please check your Version number input\n");
        return INT_MIN;
    }
    Map *structure = input->versions[srcVersion].structure_head;
    int bucket_num = getBucketNum(elemKey, structure->num_buckets);
    if (structure->buckets[bucket_num].num_in_bucket == 0) {
        printf("Please check the key entered. It wasn't found in the map.\n");
        return INT_MIN;
    }
    mapElem *rover = structure->buckets[bucket_num].head;
    while (rover) {
        if (rover->element_key == elemKey) {
            input->versions->time_of_last_access = time(0);
            return rover->element_value;
        }
        rover = rover->next;
    }
    printf("Please check the key entered. It wasn't found in the map.\n");
    return INT_MIN;
}


void map_update(PersistentDS *input, int elemKey, int elemVal, int srcVersion) {
    if (input->num_versions == input->last_updated_version_number + 1) {
        printf("You have reached the limit of number of versions you can create");
        return;
    }

    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("Please check your Version number input\n");
        return;
    }
    Map *structure = input->versions[srcVersion].structure_head;
    int bucket_num = getBucketNum(elemKey, structure->num_buckets);
    if (structure->buckets[bucket_num].num_in_bucket == 0) {
        printf("Please check the key entered. It wasn't found in the map.\n");
        return;
    }
    int found = 0;
    mapElem *rover = structure->buckets[bucket_num].head;
    while (rover) {
        if (rover->element_key == elemKey) {
            found = 1;
            break;
        }
        rover = rover->next;
    }
    if (!found) {
        printf("Please check the key entered. It wasn't found in the map.\n");
        return;
    }
    mapVersionCopy(input, srcVersion);
    Map *current_structure = input->versions[input->last_updated_version_number].structure_head;

    rover = current_structure->buckets[bucket_num].head;
    while (rover) {
        if (rover->element_key == elemKey) {
            rover->element_value = elemVal;
            return;
        }
        rover = rover->next;
    }

}

void map_delete(PersistentDS *input, int elemKeyToDelete, int srcVersion) {
    if (input->num_versions == input->last_updated_version_number + 1) {
        printf("You have reached the limit of number of versions you can create");
        return;
    }

    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("Please check your Version number input\n");
        return;
    }
    Map *structure = input->versions[srcVersion].structure_head;
    int bucket_num = getBucketNum(elemKeyToDelete, structure->num_buckets);
    if (structure->buckets[bucket_num].num_in_bucket == 0) {
        printf("Please check the key entered. It wasn't found in the map.\n");
        return;
    }
    int found = 0;
    mapElem *rover = structure->buckets[bucket_num].head;
    while (rover) {
        if (rover->element_key == elemKeyToDelete) {
            found = 1;
            break;
        }
        rover = rover->next;
    }
    if (!found) {
        printf("Please check the key entered. It wasn't found in the map.\n");
        return;
    }


    mapVersionCopy(input, srcVersion);
    Map *current_structure = input->versions[input->last_updated_version_number].structure_head;
    rover = current_structure->buckets[bucket_num].head;
    mapElem *roverlast = NULL;

    while (rover) {
        if (rover->element_key == elemKeyToDelete) {
            if (!roverlast) {
                current_structure->buckets[bucket_num].head = rover->next;
            } else {
                roverlast->next = rover->next;
            }
            free(rover);
            return;
        }
        roverlast = rover;
        rover = rover->next;
    }

}


