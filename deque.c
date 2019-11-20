#include <time.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "deque.h"

DeQueue *init_dequeue() {
    DeQueue *out = (DeQueue *) calloc(1, sizeof(DeQueue));
    out->num_elements = 0;
    out->front = NULL;
    out->rear = NULL;
    return out;
}

DeQueueNode *createDeQueueElem(int elemVal, DeQueueNode *nextDQElem, DeQueueNode *prevDQElem) {
    DeQueueNode *out = calloc(1, sizeof(DeQueueNode));
    out->value = elemVal;
    out->next = nextDQElem;
    out->prev = prevDQElem;
    return out;
}

PersistentDS *initialize_persistent_dequeue(int num_versions) {
    PersistentDS *initialized = create_persistent_ds(num_versions);
    snprintf(initialized->versions[0].description, 100, "Base Version number: %d", 0);
    initialized->versions[0].structure_head = init_dequeue();
    return initialized;
}

void add_to_dequeue(DeQueue *dqstruct, int elemVal) {
    dqstruct->num_elements++;

    DeQueueNode *oldhead = dqstruct->front;
    DeQueueNode *newnode = createDeQueueElem(elemVal, NULL, NULL);
    if (!oldhead) {
        dqstruct->rear = newnode;
    } else {
        newnode->next = oldhead;
        oldhead->prev = newnode;
    }
    dqstruct->front = newnode;
    return;
}

void add_to_dequeue_end(DeQueue *dqstruct, int elemVal) {
    dqstruct->num_elements++;
    DeQueueNode *oldrear = dqstruct->rear;
    DeQueueNode *newnode = createDeQueueElem(elemVal, NULL, NULL);
    if (!oldrear) {
        dqstruct->front = newnode;
    } else {
        newnode->prev = oldrear;
        oldrear->next = newnode;
    }
    dqstruct->rear = newnode;
    return;
}

PersistentDS *initialize_dequeue_with_element(int elemVal, int num_versions) {
    PersistentDS *out = initialize_persistent_dequeue(num_versions);
    DeQueue *structure = out->versions[0].structure_head;
    add_to_dequeue(structure, elemVal);
    out->last_updated_version_number = 0;
    return out;
}


void dequeueVersionCopy(PersistentDS *input, int srcVersion) {
    input->last_updated_version_number++;

    input->versions[input->last_updated_version_number].parent_version_number = srcVersion;
    input->versions[input->last_updated_version_number].time_of_last_update = time(0);
    input->versions[input->last_updated_version_number].time_of_last_access = time(0);
    snprintf(input->versions[input->last_updated_version_number].description, 100, "Version Number: %d",
             input->last_updated_version_number);


    DeQueue *last_structure = input->versions[srcVersion].structure_head;
    DeQueue *current_structure = init_dequeue();
    input->versions[input->last_updated_version_number].structure_head = current_structure;

    DeQueueNode *lastrover = last_structure->front;
    while (lastrover) {
        add_to_dequeue_end(current_structure, lastrover->value);
        lastrover = lastrover->next;
    }
}


void print_dequeue(PersistentDS *input, int version_num) {
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


    DeQueue *structure = requiredVersion->structure_head;
    int num_elements = structure->num_elements;
    printf("Num Elements: %d\n", num_elements);

    DeQueueNode *rover = structure->front;
    if (rover) {
        printf("DeQueue Elements:");
    }
    while (rover) {
        printf("\t%d\t", rover->value);
        rover = rover->next;
    }
    printf("\n--------------------------------------------------------------------------------\n");
}


void dq_enqueue_front(PersistentDS *input, int elemVal, int srcVersion) {
    if (input->num_versions == input->last_updated_version_number + 1) {
        printf("You have reached the limit of number of versions you can create");
        return;
    }
    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("The version you want to change does not exist");
        return;
    }
    dequeueVersionCopy(input, srcVersion);
    DeQueue *current_structure = input->versions[input->last_updated_version_number].structure_head;
    add_to_dequeue(current_structure, elemVal);
}

void dq_enqueue_rear(PersistentDS *input, int elemVal, int srcVersion) {
    if (input->num_versions == input->last_updated_version_number + 1) {
        printf("You have reached the limit of number of versions you can create");
        return;
    }
    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("The version you want to change does not exist");
        return;
    }
    dequeueVersionCopy(input, srcVersion);
    DeQueue *current_structure = input->versions[input->last_updated_version_number].structure_head;
    add_to_dequeue_end(current_structure, elemVal);
}

int dq_dequeue_front(PersistentDS *input, int srcVersion) {
    if (input->num_versions == input->last_updated_version_number + 1) {
        printf("You have reached the limit of number of versions you can create");
        return INT_MIN;
    }
    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("The version you want to change does not exist");
        return INT_MIN;
    }

    DeQueue *last_structure = input->versions[srcVersion].structure_head;
    if (!last_structure->num_elements) {
        printf("The Structure you chose is empty! Cannot Remove any Element!\n");
        return INT_MIN;
    }

    dequeueVersionCopy(input, srcVersion);
    DeQueue *current_structure = input->versions[input->last_updated_version_number].structure_head;
    current_structure->num_elements--;
    DeQueueNode *currfront = current_structure->front;
    current_structure->front = currfront->next;
    if (!current_structure->front) {
        current_structure->rear = NULL;
    } else{
        current_structure->front->prev = NULL;
    }
    int out = currfront->value;
    free(currfront);
    return out;
}

int dq_dequeue_rear(PersistentDS *input, int srcVersion) {
    if (input->num_versions == input->last_updated_version_number + 1) {
        printf("You have reached the limit of number of versions you can create");
        return INT_MIN;
    }
    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("The version you want to change does not exist");
        return INT_MIN;
    }

    DeQueue *last_structure = input->versions[srcVersion].structure_head;
    if (!last_structure->num_elements) {
        printf("The Structure you chose is empty! Cannot Remove any Element!\n");
        return INT_MIN;
    }

    dequeueVersionCopy(input, srcVersion);
    DeQueue *current_structure = input->versions[input->last_updated_version_number].structure_head;
    current_structure->num_elements--;

    DeQueueNode *curr_rear = current_structure->rear;
    current_structure->rear = curr_rear->prev;
    if (!current_structure->rear) {
        current_structure->front = NULL;
    } else{
        current_structure->rear->next = NULL;
    }
    int out = curr_rear->value;
    free(curr_rear);
    return out;
}

int dq_front(PersistentDS *input, int srcVersion) {
    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("The version you want to change does not exist");
        return INT_MIN;
    }

    DeQueue *last_structure = input->versions[srcVersion].structure_head;
    if (!last_structure->num_elements) {
        printf("The Structure you chose is empty! Cannot Remove any Element!\n");
        return INT_MIN;
    }
    return last_structure->front->value;
}


int dq_rear(PersistentDS *input, int srcVersion) {
    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("The version you want to change does not exist");
        return INT_MIN;
    }

    DeQueue *last_structure = input->versions[srcVersion].structure_head;
    if (!last_structure->num_elements) {
        printf("The Structure you chose is empty! Cannot Remove any Element!\n");
        return INT_MIN;
    }
    return last_structure->rear->value;
}