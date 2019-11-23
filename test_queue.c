#include <stdio.h>
#include <stdlib.h>
#include "queue.h"


int main(void) {
    PersistentDS *persistentQueue = initialize_queue_with_element(1, 100);
    print_queue(persistentQueue,0);
    //Print nonsense version
    print_queue(persistentQueue,1);
    //Test Enqueue
    queue_enqueue(persistentQueue,21,0);
    print_queue(persistentQueue,1);
    queue_enqueue(persistentQueue,31,0);
    print_queue(persistentQueue,2);
    queue_enqueue(persistentQueue,31,1);
    print_queue(persistentQueue,3);
    //Invalid version
    queue_enqueue(persistentQueue,31,10);
    //Queue look at front
    int front,version;
    version = 0;
    front = queue_front(persistentQueue,version);
    if(front!=INT_MIN){printf("Front of queue in version %d is %d\n",version, front);}
    version = 1;
    front = queue_front(persistentQueue,version);
    if(front!=INT_MIN){printf("Front of queue in version %d is %d\n",version, front);}
    version = 2;
    front = queue_front(persistentQueue,version);
    if(front!=INT_MIN){printf("Front of queue in version %d is %d\n",version, front);}
    version = 3;
    front = queue_front(persistentQueue,version);
    if(front!=INT_MIN){printf("Front of queue in version %d is %d\n",version, front);}
    version = 10;
    front = queue_front(persistentQueue,version);
    if(front!=INT_MIN){printf("Front of queue in version %d is %d\n",version, front);}
    //Dequeue Op
    int dequeued_elem;
    version = 3;
    dequeued_elem = queue_dequeue(persistentQueue,version);
    if(dequeued_elem!=INT_MIN){
        printf("The Element removed from Queue from version %d is %d.\nThe new version is now\n",version,dequeued_elem);
        print_queue(persistentQueue,version+1);
    }
    version = 4;
    dequeued_elem = queue_dequeue(persistentQueue,version);
    if(dequeued_elem!=INT_MIN){
        printf("The Element removed from Queue from version %d is %d.\nThe new version is now\n",version,dequeued_elem);
        print_queue(persistentQueue,version+1);
    }
    version = 5;
    dequeued_elem = queue_dequeue(persistentQueue,version);
    if(dequeued_elem!=INT_MIN){
        printf("The Element removed from Queue from version %d is %d.\nThe new version is now\n",version,dequeued_elem);
        print_queue(persistentQueue,version+1);
    }
    version = 6;
    dequeued_elem = queue_dequeue(persistentQueue,version);
    if(dequeued_elem!=INT_MIN){
        printf("The Element removed from Queue from version %d is %d.\nThe new version is now\n",version,dequeued_elem);
        print_queue(persistentQueue,version+1);
    }
    version = 7;
    dequeued_elem = queue_dequeue(persistentQueue,version);
    if(dequeued_elem!=INT_MIN){
        printf("The Element removed from Queue from version %d is %d.\nThe new version is now\n",version,dequeued_elem);
        print_queue(persistentQueue,version+1);
    }

    printf("\n");
    versionGraph* versionG = generate_version_graph(persistentQueue);
    print_version_graph(versionG);
    versionIndex *parents_list =get_parents_list(persistentQueue,6);
    print_parents_list(parents_list,6);
    print_instruction_list(parents_list,6,persistentQueue);


}