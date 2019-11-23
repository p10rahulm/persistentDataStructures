#include <time.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

//We'll use a linked list implementation with reuse of nodes


StackNode* createStackNode(int elemVal, StackNode* nextElem) {
    StackNode *out = calloc(1, sizeof(StackNode));
    out->next = nextElem;
    out->value = elemVal;
    return out;
}

PersistentDS *initialize_persistent_stack(int num_versions) {
    PersistentDS *initialized = create_persistent_ds(num_versions);
    snprintf(initialized->versions[0].description, 100, "Base Version number: %d", 0);
    initialized->versions[0].structure_head = NULL;
    return initialized;
}

PersistentDS *initialize_stack_with_element(int elemVal, int num_versions) {
    PersistentDS *out = initialize_persistent_stack(num_versions);
    out->versions[0].structure_head = createStackNode(elemVal,NULL);
    out->versions[0].instruction=ADD_INSTRUCTION;
    out->versions[0].instruction_value = elemVal;

    out->last_updated_version_number = 0;
    return out;
}

void print_stack(PersistentDS *input, int version_num) {
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


    StackNode* structure = input->versions[version_num].structure_head;
    if(structure){
        printf("Stack Elements\t->\t");
    }
    while(structure){
        printf("%d\t",structure->value);
        structure = structure->next;
    }
    printf("\n--------------------------------------------------------------------------------\n");
}

void stackVersionCopy(PersistentDS *input, int srcVersion,int instruction, int elemValue,int elemIndex) {
    input->last_updated_version_number++;
    input->versions[input->last_updated_version_number].instruction = instruction;
    input->versions[input->last_updated_version_number].instruction_value = elemValue;
    input->versions[input->last_updated_version_number].instruction_index = elemIndex;

    input->versions[input->last_updated_version_number].parent_version_number = srcVersion;
    input->versions[input->last_updated_version_number].time_of_last_update = time(0);
    input->versions[input->last_updated_version_number].time_of_last_access = time(0);
    snprintf(input->versions[input->last_updated_version_number].description, 100, "Version Number: %d", input->last_updated_version_number);
    StackNode* last_structure = input->versions[srcVersion].structure_head;
    input->versions[input->last_updated_version_number].structure_head = last_structure;
}


void stack_push(PersistentDS *input, int elemVal, int srcVersion) {
    if (input->num_versions == input->last_updated_version_number + 1) {
        printf("You have reached the limit of number of versions you can create");
        return;
    }
    if (srcVersion > input->last_updated_version_number) {
        printf("The version you want to change does not exist");
        return;
    }

    StackNode* last_structure = input->versions[srcVersion].structure_head;
    stackVersionCopy(input, srcVersion,ADD_INSTRUCTION,elemVal,0);
    StackNode* newnode = createStackNode(elemVal,last_structure);
    input->versions[input->last_updated_version_number].structure_head = newnode;
}

int stack_look(PersistentDS *input, int srcVersion) {
    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("The version you want to change does not exist");
        return INT_MIN;
    }
    StackNode* head =input->versions[srcVersion].structure_head;
    input->versions[srcVersion].time_of_last_access = time(0);
    return head->value;
}

int stack_pop(PersistentDS *input, int srcVersion) {
    if (input->num_versions == input->last_updated_version_number + 1) {
        printf("You have reached the limit of number of versions you can create");
        return INT_MIN;
    }
    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("The version you want to change does not exist");
        return INT_MIN;
    }

    StackNode* last_structure = input->versions[srcVersion].structure_head;
    if(!last_structure){
        printf("The Stack you chose is empty! Cannot Pop any Element!\n");
        return INT_MIN;
    }
    stackVersionCopy(input, srcVersion,DELETE_INSTRUCTION,0,1);
    input->versions[input->last_updated_version_number].structure_head = last_structure->next;
    return last_structure->value;
}
