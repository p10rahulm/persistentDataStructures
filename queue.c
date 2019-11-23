#include <time.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"


Queue *init_queue() {
    Queue *out = (Queue *) calloc(1, sizeof(Queue));
    out->num_elements = 0;
    out->front = NULL;
    out->rear = NULL;
    return out;
}

QueueNode *createQueueElem(int elemVal, QueueNode *nextQElem) {
    QueueNode *out = calloc(1, sizeof(QueueNode));
    out->value = elemVal;
    out->next = nextQElem;
    return out;
}

PersistentDS *initialize_persistent_queue(int num_versions) {
    PersistentDS *initialized = create_persistent_ds(num_versions);
    snprintf(initialized->versions[0].description, 100, "Base Version number: %d", 0);
    initialized->versions[0].structure_head = init_queue();
    return initialized;
}

void add_to_queue(Queue *qstruct, int elemVal) {
    qstruct->num_elements++;

    QueueNode *oldhead = qstruct->front;
    QueueNode *newnode = createQueueElem(elemVal, NULL);
    if(!oldhead){
      qstruct->front = newnode;
      qstruct->rear= newnode;
    } else   {
        newnode->next=oldhead;
    }
    qstruct->front = newnode;
    return;
}

void add_to_queue_end(Queue *qstruct, int elemVal) {
    qstruct->num_elements++;
    QueueNode *oldrear = qstruct->rear;
    QueueNode *newnode = createQueueElem(elemVal, NULL);
    if(oldrear){
        oldrear->next = newnode;
    } else{
        qstruct->front = newnode;
    }
    qstruct->rear = newnode;
    return;
}

PersistentDS *initialize_queue_with_element(int elemVal, int num_versions) {
    PersistentDS *out = initialize_persistent_queue(num_versions);
    Queue *structure = out->versions[0].structure_head;
    out->versions[0].instruction=ADD_INSTRUCTION;
    out->versions[0].instruction_value = elemVal;

    add_to_queue(structure, elemVal);
    out->last_updated_version_number = 0;
    return out;
}


void queueVersionCopy(PersistentDS *input, int srcVersion,int instruction, int elemValue,int elemIndex) {
    input->last_updated_version_number++;
    input->versions[input->last_updated_version_number].instruction = instruction;
    input->versions[input->last_updated_version_number].instruction_value = elemValue;
    input->versions[input->last_updated_version_number].instruction_index = elemIndex;

    input->versions[input->last_updated_version_number].parent_version_number = srcVersion;
    input->versions[input->last_updated_version_number].time_of_last_update = time(0);
    input->versions[input->last_updated_version_number].time_of_last_access = time(0);
    snprintf(input->versions[input->last_updated_version_number].description, 100, "Version Number: %d",
             input->last_updated_version_number);


    Queue *last_structure = input->versions[srcVersion].structure_head;
    Queue *current_structure = init_queue();
    input->versions[input->last_updated_version_number].structure_head = current_structure;
    current_structure->num_elements = 0;
    QueueNode *lastrover = last_structure->front;
    while (lastrover) {
        add_to_queue_end(current_structure, lastrover->value);
        lastrover = lastrover->next;

    }
}


void print_queue(PersistentDS *input, int version_num) {
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


    Queue *structure = requiredVersion->structure_head;
    int num_elements = structure->num_elements;
    printf("Num Elements: %d\n", num_elements);

    QueueNode *rover = structure->front;
    if (rover) {
        printf("Queue Elements\t->\t");
    }
    while (rover) {
        printf("%d\t", rover->value);
        rover = rover->next;
    }
    printf("\n--------------------------------------------------------------------------------\n");
}


void queue_enqueue(PersistentDS *input, int elemVal, int srcVersion) {
    if (input->num_versions == input->last_updated_version_number + 1) {
        printf("You have reached the limit of number of versions you can create");
        return;
    }
    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("The version you want to change does not exist");
        return;
    }
    queueVersionCopy(input, srcVersion,ADD_INSTRUCTION,elemVal,-1);
    Queue *current_structure = input->versions[input->last_updated_version_number].structure_head;
    add_to_queue_end(current_structure, elemVal);
}

int queue_dequeue(PersistentDS *input, int srcVersion) {
    if (input->num_versions == input->last_updated_version_number + 1) {
        printf("You have reached the limit of number of versions you can create");
        return INT_MIN;
    }
    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("The version you want to change does not exist");
        return INT_MIN;
    }

    Queue *last_structure = input->versions[srcVersion].structure_head;
    if (!last_structure->num_elements) {
        printf("The Structure you chose is empty! Cannot Remove any Element!\n");
        return INT_MIN;
    }

    queueVersionCopy(input, srcVersion,DELETE_INSTRUCTION,0,1);
    Queue *current_structure = input->versions[input->last_updated_version_number].structure_head;
    current_structure->num_elements--;
    QueueNode* currFront = current_structure->front;
    current_structure->front = currFront->next;
    if(!current_structure->front){        current_structure->rear=NULL;    }
    int out = currFront->value;
    free(currFront);
    return out;
}

int queue_front(PersistentDS *input, int srcVersion) {
    if (srcVersion > input->last_updated_version_number || srcVersion < 0) {
        printf("The version you want to change does not exist");
        return INT_MIN;
    }

    Queue *last_structure = input->versions[srcVersion].structure_head;
    if (!last_structure->num_elements) {
        printf("The Structure you chose is empty! Cannot Remove any Element!\n");
        return INT_MIN;
    }
    return last_structure->front->value;
}
