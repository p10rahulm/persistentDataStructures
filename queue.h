#ifndef PERSISTENCEFOREXISTENCE_QUEUE_H
#define PERSISTENCEFOREXISTENCE_QUEUE_H

#include "persistence.h"

typedef struct queuenode {
    int value;
    struct queuenode *next;
} QueueNode;

typedef struct queue {
    int num_elements;
    QueueNode *front;
    QueueNode *rear;
} Queue;

PersistentDS *initialize_queue_with_element(int elemVal, int num_versions);
void print_queue(PersistentDS *input, int version_num);
void queue_enqueue(PersistentDS *input, int elemVal, int srcVersion);
int queue_front(PersistentDS *input, int srcVersion);
int queue_dequeue(PersistentDS *input, int srcVersion);

#endif //PERSISTENCEFOREXISTENCE_QUEUE_H
