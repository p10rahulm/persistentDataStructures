#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "persistence.h"

/* For the vector, we will allow operations
 * We will have the operations allowed as:
 * 1.Create
 * 2.Read
 * 3.Update
 * 4.Delete
 *
 * Persistence would be through a version graph.
*/


VersionNode *initialize_versions(int num_versions) {
    VersionNode *versions = (VersionNode *) calloc(num_versions, sizeof(VersionNode));
    for (int i = 0; i < num_versions; ++i) {
        versions[i].version_number = i;
        versions[i].description = calloc(100, sizeof(char));
        versions[i].structure_head = (void *) NULL;
        versions[i].time_of_last_access = time(0);
        versions[i].time_of_last_update = time(0);
        versions[i].parent_version_number = i;
    }
    return versions;
}

PersistentDS *create_persistent_ds(int num_versions) {
    PersistentDS *out = calloc(1, sizeof(PersistentDS));
    out->last_updated_version_number = -1;
    out->num_versions = num_versions;
    out->versions = initialize_versions(num_versions);
    return out;
}


void printtime(time_t inputtime) {
    char buffer[26];
    struct tm *tm_info;
    tm_info = localtime(&inputtime);
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    puts(buffer);
}

void printVersionNodeDetails(VersionNode *inputVersion) {
    printf("Version Number : %d\n", inputVersion->version_number);
    printf("Description : %s\n", inputVersion->description);
    printf("Time of Last Update : ", inputVersion->time_of_last_update);
    printtime(inputVersion->time_of_last_update);
    printf("Time of Last Access : ", inputVersion->time_of_last_access);
    printtime(inputVersion->time_of_last_access);
    printf("Parent Version Number : %d\n", inputVersion->parent_version_number);
}



VersionIndex* createNewVersionIndex(int version, VersionIndex* next){
    VersionIndex* newIndex = calloc(1, sizeof(VersionIndex));
    newIndex->version = version;
    newIndex->next = next;
    return newIndex;
}

void add_to_parent(int version,int parent_version,versionGraph* vg){
    vg->childVersionArray[parent_version].num_children++;
    VersionIndex* newIndex = createNewVersionIndex(version, vg->childVersionArray[parent_version].head);
    vg->childVersionArray[parent_version].head = newIndex;
}
void add_to_version_graph(versionGraph* vg,PersistentDS * pds){
    num_versions = pds->last_updated_version_number+1;
    for (int i = 0; i < num_versions; ++i) {
        add_to_parent(i,pds->versions[i].parent_version_number,vg);
    }
}

versionGraph* generate_version_graph(PersistentDS * inputDS) {
    versionGraph* out = calloc(1, sizeof(versionGraph));
    out->num_versions = inputDS->last_updated_version_number+1;
    out->childVersionArray = calloc(out->num_versions, sizeof(childVersions));
    add_to_version_graph(out,inputDS);
    return out;
}

void print_version_graph(versionGraph * vg) {
    printf("Version\t||\tChildren--->\n")
    for (int i = 0; i < vg->num_versions; ++i) {
        printf("%d\t||\t",i);
        VersionIndex* rover = vg->childVersionArray[i].head;
        while(rover){
            printf("%d\t",rover->version);
            rover=rover->next;
        }
        printf("\n");
    }
}

