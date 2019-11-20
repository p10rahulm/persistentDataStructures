#include <time.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "ll.h"


ListNode *createListNode(int elemVal, ListNode *nextListNode) {
    ListNode *out = calloc(1, sizeof(ListNode));
    out->value = elemVal;
    out->next = nextListNode;
    return out;
}

PersistentDS *initialize_persistent_sll(int num_versions) {
    PersistentDS *initialized = create_persistent_ds(num_versions);
    snprintf(initialized->versions[0].description, 100, "Base Version number: %d", 0);
    initialized->versions[0].structure_head = NULL;
    return initialized;
}

PersistentDS *initialize_sll_with_element(int elemVal, int num_versions) {
    PersistentDS *out = initialize_persistent_sll(num_versions);
    out->versions[0].structure_head = createListNode(elemVal, NULL);
    out->last_updated_version_number = 0;
    return out;
}


void sllVersionCopy(PersistentDS *input, int srcVersion) {
    input->last_updated_version_number++;

    input->versions[input->last_updated_version_number].parent_version_number = srcVersion;
    input->versions[input->last_updated_version_number].time_of_last_update = time(0);
    input->versions[input->last_updated_version_number].time_of_last_access = time(0);
    snprintf(input->versions[input->last_updated_version_number].description, 100, "Version Number: %d",
             input->last_updated_version_number);

    ListNode *last_structure = input->versions[srcVersion].structure_head;
    ListNode *current_node=NULL,*current_prev = NULL;
    while (last_structure) {
        current_node = createListNode(last_structure->value,NULL);
        if(current_prev){
            current_prev->next = current_node;
        } else {
            input->versions[input->last_updated_version_number].structure_head = current_node;
        }
        current_prev = current_node;
        last_structure = last_structure->next;
    }
}


void print_sll(PersistentDS *input, int version_num) {
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


    ListNode *structure = requiredVersion->structure_head;
    if (structure) {
        printf("Singly Linked List Elements:");
    }
    while (structure) {
        printf("\t%d\t", structure->value);
        structure = structure->next;
    }
    printf("\n--------------------------------------------------------------------------------\n");
}


void sll_add(PersistentDS *input, int elemVal, int srcVersion) {
    if (input->num_versions == input->last_updated_version_number + 1) {
        printf("You have reached the limit of number of versions you can create");
        return;
    }
    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("The version you want to change does not exist");
        return;
    }
    sllVersionCopy(input, srcVersion);
    ListNode* lastHead =input->versions[input->last_updated_version_number].structure_head;
    input->versions[input->last_updated_version_number].structure_head = createListNode(elemVal,lastHead);
}

int sll_read(PersistentDS *input, int elemIndex, int srcVersion) {
    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("Please check your Version number input: %d\n",srcVersion);
        return INT_MIN;
    }
    ListNode *rover = input->versions[srcVersion].structure_head;
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


void sll_update(PersistentDS *input, int elemIndex, int elemVal, int srcVersion) {
    if (input->num_versions == input->last_updated_version_number + 1) {
        printf("You have reached the limit of number of versions you can create");
        return;
    }

    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("Please check your Version number input\n");
        return;
    }
    //Check if index found
    int found =0;
    int tempIndex = elemIndex;
    ListNode *rover = input->versions[srcVersion].structure_head;
    while (rover) {
        if(tempIndex==0){
            found=1;
            break;
        }
        rover = rover->next;
        tempIndex--;
    }
    if(!found){
        printf("Please check the Element Index. It does not exist.\n");
        return ;
    }
    sllVersionCopy(input, srcVersion);
    rover = input->versions[input->last_updated_version_number].structure_head;
    tempIndex = elemIndex;
    while (rover) {
        if(tempIndex==0){
            rover->value=elemVal;
            return;
        }
        rover = rover->next;
        tempIndex--;
    }
}

int sll_delete(PersistentDS *input, int elemIndexToDelete, int srcVersion) {
    if (input->num_versions == input->last_updated_version_number + 1) {
        printf("You have reached the limit of number of versions you can create");
        return INT_MIN;
    }

    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("Please check your Version number input\n");
        return INT_MIN;
    }
    //Check if index found
    int found =0;
    int tempIndex = elemIndexToDelete;
    ListNode *rover = input->versions[srcVersion].structure_head;
    while (rover) {
        if(tempIndex==0){
            found=1;
            break;
        }
        rover = rover->next;
        tempIndex--;
    }
    if(!found){
        printf("Please check the Element Index. It does not exist.\n");
        return INT_MIN;
    }
    //Found
    sllVersionCopy(input, srcVersion);
    rover = input->versions[input->last_updated_version_number].structure_head;
    tempIndex = elemIndexToDelete;
    ListNode* rover_prev = NULL;
    while (rover) {
        if(tempIndex==0){
            if(rover_prev==NULL){
                input->versions[input->last_updated_version_number].structure_head = rover->next;
                int out = rover->value;
                free(rover);
                return out;
            } else {
                rover_prev->next = rover->next;
                int out = rover->value;
                free(rover);
                return out;
            }
        }
        rover_prev = rover;
        rover = rover->next;
        tempIndex--;
    }
}


