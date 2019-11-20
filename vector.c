#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"



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

PersistentDS *initialize_vector_with_element(int element, int num_versions, int num_elements) {
    PersistentDS *out = initialize_persistent_vector(num_versions, num_elements);
    Vector *structure = out->versions[0].structure_head;
    int *elements_array = structure->elements_array;
    elements_array[0] = element;
    structure->last_index = 0;
    out->last_updated_version_number = 0;
    return out;
}


void print_vector(PersistentDS *input, int version_num) {
    if(version_num<0 || version_num > input->last_updated_version_number){
        printf("Please check version to print. The suggested version no: %d does not exist\n",version_num);
        return;
    }
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


void vectorVersionCopy(PersistentDS *input, int srcVersion, int destVersion) {
    input->last_updated_version_number++;

    input->versions[input->last_updated_version_number].parent_version_number = srcVersion;
    input->versions[input->last_updated_version_number].time_of_last_update = time(0);
    input->versions[input->last_updated_version_number].time_of_last_access = time(0);
    snprintf(input->versions[input->last_updated_version_number].description, 100, "Version Number: %d", destVersion);

    input->versions[input->last_updated_version_number].structure_head = (Vector *) calloc(1, sizeof(Vector));

    Vector *last_structure = input->versions[srcVersion].structure_head;
    Vector *current_structure = input->versions[input->last_updated_version_number].structure_head;

    current_structure->num_elements = last_structure->num_elements;
    current_structure->last_index = last_structure->last_index;
    current_structure->elements_array = (int *) calloc(last_structure->num_elements, sizeof(int));
    int *current_elem_array = current_structure->elements_array;
    int *last_elem_array = last_structure->elements_array;
    for (int i = 0; i <=last_structure->last_index ; ++i) {
        current_elem_array[i] =last_elem_array[i];
    }
    // memcpy(current_structure->elements_array, last_structure->elements_array, sizeof(last_structure->elements_array));

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
        current_elem_array[i] = current_elem_array[i+1];
    }
    current_elem_array[current_structure->last_index] =0;
    current_structure->last_index--;
}


