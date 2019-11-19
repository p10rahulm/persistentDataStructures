#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

typedef struct vector {
    int num_elements;
    int last_index;
    void *elements_array;
} Vector;

PersistentDS *create_persistent_ds(int num_versions) {
    PersistentDS *out = calloc(1, sizeof(PersistentDS));
    out->last_updated_version_number = -1;
    out->num_versions = num_versions;
    out->versions = initialize_versions(num_versions);
    return out;
}

PersistentDS *initialize_persistent_vector(int num_versions, int num_elements) {
    PersistentDS *initialized = create_persistent_ds(num_versions);
    snprintf(initialized->versions[0].description, 100, "Base Version number: %d", 0);
    initialized->versions[0].structure_head = (Vector *) calloc(1, sizeof(Vector));
    Vector *structure = initialized->versions[0].structure_head;
    structure->num_elements = num_elements;
    structure->last_index = -1;
    structure->elements_array = calloc(num_elements, sizeof(int));
    return initialized;
}

PersistentDS *initialize_with_element(int element, int num_versions, int num_elements) {
    PersistentDS *out = initialize_persistent_vector(num_versions, num_elements);
    Vector *structure = out->versions[0].structure_head;
    int *elements_array = structure->elements_array;
    elements_array[0] = element;
    structure->last_index = 0;
    out->last_updated_version_number = 0;
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

void print_version(PersistentDS *input, int version_num) {
    int last_updated_version = input->last_updated_version_number;
    printf("\n--------------------------------------------------------------------------------\n");
    printf("Last Updated Version Num = %d\n", last_updated_version);
    if (version_num > last_updated_version) { printf("So many versions do not exist\n"); }
    VersionNode *requiredVersion = &(input->versions[version_num]);
    printVersionNodeDetails(requiredVersion);
    Vector *structure = requiredVersion->structure_head;
    int num_elements_in_array = structure->num_elements;
    printf("Num Elements in Array: %d\n", num_elements_in_array);
    int lastindex = structure->last_index;
    printf("Last Index Updated in Array: %d\n", lastindex);
    int *elements_array = structure->elements_array;
    for (int i = 0; i < num_elements_in_array; ++i) {
        printf("%d\t", elements_array[i]);
    }
    printf("\n--------------------------------------------------------------------------------\n");
}


void versionCopy(PersistentDS *input, int srcVersion, int destVersion) {
    input->last_updated_version_number++;

    input->versions[input->last_updated_version_number].parent_version_number = srcVersion;
    input->versions[input->last_updated_version_number].time_of_last_update = time(0);
    input->versions[input->last_updated_version_number].time_of_last_access = time(0);
    snprintf(input->versions[input->last_updated_version_number].description, 100, "Version Number: %d", destVersion);

    input->versions[input->last_updated_version_number].structure_head = (Vector *) calloc(1, sizeof(Vector));

    Vector *last_structure = input->versions[srcVersion].structure_head;
    Vector *current_structure = input->versions[input->last_updated_version_number].structure_head;

    current_structure->num_elements = last_structure->num_elements;
    current_structure->last_index = last_structure->last_index + 1;
    current_structure->elements_array = (int *) calloc(last_structure->num_elements, sizeof(int));
    int *current_elem_array = current_structure->elements_array;
    int *last_elem_array = last_structure->elements_array;
    memcpy(current_structure->elements_array, last_structure->elements_array, sizeof(last_structure->elements_array));

}


PersistentDS *vector_add(PersistentDS *input, int element, int srcVersion) {
    if (input->num_versions == input->last_updated_version_number + 1) {
        printf("You have reached the limit of number of versions you can create");
        return input;
    }

    Vector *last_structure = input->versions[srcVersion].structure_head;
    if (last_structure->num_elements == last_structure->last_index + 1) {
        printf("The Vector is full. You cannot add more elements. Please increase the array size, or delete some elements and retry");
        return input;
    }
    if (srcVersion > input->last_updated_version_number) {
        printf("The version you want to change does not exist");
        return input;
    }

    versionCopy(input, srcVersion, input->last_updated_version_number + 1);
    Vector *current_structure = input->versions[input->last_updated_version_number].structure_head;
    int *current_elem_array = current_structure->elements_array;
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

PersistentDS *vector_update(PersistentDS *input, int index_to_update, int element, int srcVersion) {
    if (input->num_versions == input->last_updated_version_number + 1) {
        printf("You have reached the limit of number of versions you can create");
        return input;
    }
    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("The version you want to change does not exist");
        return input;
    }

    versionCopy(input, srcVersion, input->last_updated_version_number + 1);
    Vector *current_structure = input->versions[input->last_updated_version_number].structure_head;
    int *current_elem_array = current_structure->elements_array;
    current_elem_array[current_structure->last_index] = element;

}


void main() {
    int num_versions = 100;
    int num_elements = 5;
    PersistentDS *vector = initialize_with_element(11, num_versions, num_elements);
    print_version(vector, 0);
    vector_add(vector, 21, 0);
    print_version(vector, 1);
    vector_add(vector, 31, 0);
    print_version(vector, 2);
    vector_add(vector, 31, 1);
    print_version(vector, 3);
    printf("The third element of version number 3 is %d\n", vector_read(vector, 2, 3));


}

