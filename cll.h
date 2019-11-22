#ifndef PERSISTENCEFOREXISTENCE_CLL_H
#define PERSISTENCEFOREXISTENCE_CLL_H
#include "persistence.h"


typedef struct cllnode {
    int value;
    struct cllnode *next;
    struct cllnode *prev;
} CLLNode;

typedef struct CLL {
    int num_elements;
    CLLNode* head;
    CLLNode* tail;
} CLL;

PersistentDS *initialize_cll_with_element(int elemVal, int num_versions);
void print_cll(PersistentDS *input, int version_num);

void cll_add(PersistentDS *input, int elemVal, int srcVersion);
int cll_read(PersistentDS *input, int elemIndex, int srcVersion);
void cll_update(PersistentDS *input, int elemIndex, int elemVal, int srcVersion);
int cll_delete(PersistentDS *input, int elemIndexToDelete, int srcVersion);

#endif //PERSISTENCEFOREXISTENCE_CLL_H
