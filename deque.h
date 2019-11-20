#ifndef PERSISTENCEFOREXISTENCE_DEQUE_H
#define PERSISTENCEFOREXISTENCE_DEQUE_H

#include "persistence.h"

typedef struct dequeuenode {
    int value;
    struct dequeuenode *next;
    struct dequeuenode *prev;
} DeQueueNode;

typedef struct dequeue {
    int num_elements;
    DeQueueNode *front;
    DeQueueNode *rear;
} DeQueue;

PersistentDS *initialize_dequeue_with_element(int elemVal, int num_versions);
void print_dequeue(PersistentDS *input, int version_num);
void dq_enqueue_front(PersistentDS *input, int elemVal, int srcVersion);
void dq_enqueue_rear(PersistentDS *input, int elemVal, int srcVersion);

int dq_dequeue_front(PersistentDS *input, int srcVersion);
int dq_dequeue_rear(PersistentDS *input, int srcVersion);
int dq_front(PersistentDS *input, int srcVersion);
int dq_rear(PersistentDS *input, int srcVersion);

#endif //PERSISTENCEFOREXISTENCE_DEQUE_H
