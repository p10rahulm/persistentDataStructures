#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "persistence.h"


/* For the vector, we will allow operations
 * We will have the operations allowed as:
 * 1.retrieve
 * 2.store
 * 3.modify
 *
 * Persistence would be through a version graph.
*/

typedef struct versionnode {
    int version_number;
    time_t time_of_last_access;
    time_t time_of_last_update;
    char* description;
    void* structure_head;
    //we use parent node number to track parents in version tree. If no parent, then points to itself
    int parent_version_number;
} VersionNode;

typedef struct persistent_data_structure{
    int last_updated_version_number;
    VersionNode* versions;
} PersistentDS;


VersionNode* initialize_versions(int num_versions){
    VersionNode* versions = (VersionNode*) calloc(num_versions,sizeof(VersionNode));
    for (int i = 0; i < num_versions; ++i) {
        versions[i].version_number = i;
        versions[i].description = (char*) calloc(100, sizeof(char));
        versions[i].structure_head = (void*) NULL;
        versions[i].time_of_last_access = time(0);
        versions[i].time_of_last_update = time(0);
        versions[i].parent_version_number = i;
    }
    return versions;
}

typedef struct vector{
    int num_elements;
    void* elements_array;
} Vector;

PersistentDS* create_persistent_ds(int num_elements){
    PersistentDS* out =  calloc(1, sizeof(PersistentDS));
    out->last_updated_version_number = 0;
    out->versions = initialize_versions(num_elements);
    return out;
}
