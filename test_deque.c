#include <stdio.h>
#include <stdlib.h>

#include "deque.h"


int main(void) {
    PersistentDS *persistentDeQueue = initialize_dequeue_with_element(11, 100);
    print_dequeue(persistentDeQueue,0);
    //Print nonsense version
    print_dequeue(persistentDeQueue,1);
    //Test Enqueue front
    dq_enqueue_front(persistentDeQueue,21,0);
    print_dequeue(persistentDeQueue,1);
    dq_enqueue_front(persistentDeQueue,31,0);
    print_dequeue(persistentDeQueue,2);
    dq_enqueue_front(persistentDeQueue,31,1);
    print_dequeue(persistentDeQueue,3);
    //Invalid version
    dq_enqueue_front(persistentDeQueue,31,10);
    //Test Enqueue rear
    dq_enqueue_rear(persistentDeQueue,41,3);
    print_dequeue(persistentDeQueue,4);
    dq_enqueue_rear(persistentDeQueue,51,3);
    print_dequeue(persistentDeQueue,5);
    dq_enqueue_rear(persistentDeQueue,51,4);
    print_dequeue(persistentDeQueue,6);
    //Invalid version
    dq_enqueue_rear(persistentDeQueue,61,7);
    //Look at front and rear
    printf("\n");
    for (int i = 0; i < 8; ++i) {
        int front = dq_front(persistentDeQueue,i);
        int rear = dq_rear(persistentDeQueue,i);
        if(front!=INT_MIN && rear!=INT_MIN){
            printf("Version No: %d. Front Element = %d, Rear Element = %d\n",i,front,rear);
        }
    }
    printf("\n");
    //Test Dequeue from front
    int version,dequeued_elem;
    for (int version = 6; version < 12; ++version) {
        int dequeued_elem = dq_dequeue_front(persistentDeQueue,version);
        if(dequeued_elem!=INT_MIN){
            printf("The Element removed from Queue from version %d is %d.\nThe new version is now\n",version,dequeued_elem);
            print_dequeue(persistentDeQueue,version+1);
        }
    }

    //Test Dequeue from Rear. First reuse version 6
    dq_enqueue_front(persistentDeQueue,61,6);
    print_dequeue(persistentDeQueue,12);

    for (int version = 12; version < 19; ++version) {
        int dequeued_elem = dq_dequeue_rear(persistentDeQueue,version);
        if(dequeued_elem!=INT_MIN){
            printf("The Element removed from Queue from version %d is %d.\nThe new version is now\n",version,dequeued_elem);
            print_dequeue(persistentDeQueue,version+1);
        }
    }

    printf("\n");
    versionGraph* versionG = generate_version_graph(persistentDeQueue);
    print_version_graph(versionG);
    versionIndex *parents_list =get_parents_list(persistentDeQueue,18);
    print_parents_list(parents_list,18);
    print_instruction_list(parents_list,18,persistentDeQueue);

}