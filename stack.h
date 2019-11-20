#ifndef PERSISTENCEFOREXISTENCE_STACK_H
#define PERSISTENCEFOREXISTENCE_STACK_H

#include "persistence.h"

typedef struct stacknode{
    int value;
    struct stacknode* next;
} StackNode;


PersistentDS *initialize_stack_with_element(int elemVal, int num_versions);
void print_stack(PersistentDS *input, int version_num);
void stack_push(PersistentDS *input, int elemVal, int srcVersion);
int stack_look(PersistentDS *input, int srcVersion);
void stack_pop(PersistentDS *input, int elemVal, int srcVersion);



#endif //PERSISTENCEFOREXISTENCE_STACK_H
