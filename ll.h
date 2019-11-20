#ifndef PERSISTENCEFOREXISTENCE_LL_H
#define PERSISTENCEFOREXISTENCE_LL_H

#include "persistence.h"

typedef struct listnode {
    int value;
    struct listnode *next;
} ListNode;


PersistentDS *initialize_sll_with_element(int elemVal, int num_versions);
void print_sll(PersistentDS *input, int version_num);
void sll_add(PersistentDS *input, int elemVal, int srcVersion);
int sll_read(PersistentDS *input, int elemIndex, int srcVersion);
void sll_update(PersistentDS *input, int elemIndex, int elemVal, int srcVersion);
int sll_delete(PersistentDS *input, int elemIndexToDelete, int srcVersion);


#endif //PERSISTENCEFOREXISTENCE_LL_H
