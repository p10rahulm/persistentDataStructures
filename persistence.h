#ifndef PERSISTENCEFOREXISTENCE_PERSISTENCE_H
#define PERSISTENCEFOREXISTENCE_PERSISTENCE_H

typedef struct versionnode {
    int version_number;
    time_t time_of_last_access;
    time_t time_of_last_update;
    char *description;
    void *structure_head;
    //we use parent node number to track parents in version tree. If no parent, then points to itself
    int parent_version_number;
} VersionNode;

typedef struct persistent_data_structure {
    int last_updated_version_number;
    int num_versions;
    VersionNode *versions;
} PersistentDS;


VersionNode *initialize_versions(int num_versions);
PersistentDS *create_persistent_ds(int num_versions);
void printtime(time_t inputtime);
void printVersionNodeDetails(VersionNode *inputVersion);


typedef struct version_index{
    int version;
    struct version_index* next;
}VersionIndex;

typedef struct child_versions{
    int num_children;
    versionIndex* head;
} childVersions;

typedef struct version_graph{
    int num_versions;
    childVersions* childVersionArray;
} versionGraph;

versionGraph* generate_version_graph(PersistentDS * inputDS);
void print_version_graph(versionGraph * vg);


#endif //PERSISTENCEFOREXISTENCE_PERSISTENCE_H