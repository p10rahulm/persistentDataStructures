#include <time.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "dll.h"

typedef struct dllnode {
    int value;
    struct dllnode *next;
    struct dllnode *prev;
} DLLNode;

typedef struct DLL {
    int num_elements;
    DLLNode* head;
    DLLNode* tail;
} DLL;


DLLNode* createDLLNode(int elemVal, DLLNOde *prevDLLNOde, DLLNOde *nextDLLNOde) {
    DListNode *out = calloc(1, sizeof(DListNode));
    out->value = elemVal;
    out->prev = prevDLLNOde;
    out->next = nextDLLNOde;
    return out;
}

PersistentDS *initialize_persistent_dll(int num_versions) {
    PersistentDS *initialized = create_persistent_ds(num_versions);
    snprintf(initialized->versions[0].description, 100, "Base Version number: %d", 0);
    initialized->versions[0].structure_head = NULL;
    return initialized;
}

PersistentDS *initialize_dll_with_element(int elemVal, int num_versions) {
    PersistentDS *out = initialize_persistent_dll(num_versions);
    out->versions[0].structure_head = calloc(1, sizeof(DLL));
    DLL* structure =out->versions[0].structure_head;
    structure->num_elements =1;
    structure->head = createDLLNode(elemVal,NULL,NULL);
    structure->tail = structure->head;
    out->last_updated_version_number = 0;
    return out;
}


void dllVersionCopy(PersistentDS *input, int srcVersion) {
    input->last_updated_version_number++;

    input->versions[input->last_updated_version_number].parent_version_number = srcVersion;
    input->versions[input->last_updated_version_number].time_of_last_update = time(0);
    input->versions[input->last_updated_version_number].time_of_last_access = time(0);
    snprintf(input->versions[input->last_updated_version_number].description, 100, "Version Number: %d",
             input->last_updated_version_number);

    DLL *last_structure = input->versions[srcVersion].structure_head;
    DLL *current_structure = input->versions[input->last_updated_version_number].structure_head;
    DLLNode* lastnode = last_structure->head;
    DLLNode *current_node=NULL;
    DLLNode* current_prev = NULL;
    while (last_structure) {
        current_node = createDLLNode(last_structure->value,NULL,NULL);
        if(current_prev){
            current_prev->next = current_node;
            current_node->prev = current_prev;
        } else {
            current_structure->head = current_node;
        }
        current_structure->tail = current_node;
        current_prev = current_node;
        last_structure = last_structure->next;
    }
}


void print_dll(PersistentDS *input, int version_num) {
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


    DLL *DL_structure = requiredVersion->structure_head;
    DLL *structure = DL_structure->head;
    if (structure) {
        printf("Doubly Linked List Elements:");
    }
    while (structure) {
        printf("\t%d\t", structure->value);
        structure = structure->next;
    }
    printf("\n--------------------------------------------------------------------------------\n");
}


void dll_add(PersistentDS *input, int elemVal, int srcVersion) {
    if (input->num_versions == input->last_updated_version_number + 1) {
        printf("You have reached the limit of number of versions you can create");
        return;
    }
    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("The version you want to change does not exist");
        return;
    }
    dllVersionCopy(input, srcVersion);
    DLL* structure =input->versions[input->last_updated_version_number].structure_head;
    structure->num_elements++;
    DLLNode* newnode = createListNode(elemVal,NULL,NULL);
    newnode->next = structure->head;
    structure->head = newnode;
}

int dll_read(PersistentDS *input, int elemIndex, int srcVersion) {
    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("Please check your Version number input: %d\n",srcVersion);
        return INT_MIN;
    }
    DLL*currStructure = input->versions[srcVersion].structure_head;
    if(currStructure->num_elements < elemIndex + 1){
        printf("Please check the Index no %d. It does not exist.\n",elemIndex);
    }
    DLLNode *rover = currStructure->head
    int tempIndex = elemIndex;
    while (rover) {
        if(tempIndex==0){
            return rover->value;
        }
        rover = rover->next;
        tempIndex--;
    }
    printf("Please check the Index no %d. It does not exist.\n",elemIndex);
    return INT_MIN;
}


void dll_update(PersistentDS *input, int elemIndex, int elemVal, int srcVersion) {
    if (input->num_versions == input->last_updated_version_number + 1) {
        printf("You have reached the limit of number of versions you can create");
        return;
    }

    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("Please check your Version number input\n");
        return;
    }
    //Check if index found
    DLL *structure = input->versions[srcVersion].structure_head;
    if(structure->num_elements < elemIndex + 1){
        printf("Please check the Element Index. It does not exist.\n");
        return ;
    }
    dllVersionCopy(input, srcVersion);
    DLL*currStructure = input->versions[srcVersion].structure_head;
    DLLNode *rover = currStructure->head
    int tempIndex = elemIndex;
    while (rover) {
        if(tempIndex==0){
            rover->value = elemVal;
            return;
        }
        rover = rover->next;
        tempIndex--;
    }
}

int dll_delete(PersistentDS *input, int elemIndexToDelete, int srcVersion) {
    if (input->num_versions == input->last_updated_version_number + 1) {
        printf("You have reached the limit of number of versions you can create");
        return;
    }

    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("Please check your Version number input\n");
        return;
    }
    //Check if index found
    DLL *structure = input->versions[srcVersion].structure_head;
    if(structure->num_elements < elemIndexToDelete + 1){
        printf("Please check the Element Index. It does not exist.\n");
        return ;
    }
    dllVersionCopy(input, srcVersion);
    DLL*currStructure = input->versions[srcVersion].structure_head;
    DLLNode *rover = currStructure->head
    int tempIndex = elemIndex;
    while (rover) {
        if(tempIndex==0){
            rover->value = elemVal;
            return;
        }
        rover = rover->next;
        tempIndex--;
    }
}


