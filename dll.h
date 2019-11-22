#ifndef PERSISTENCEFOREXISTENCE_DLL_H
#define PERSISTENCEFOREXISTENCE_DLL_H

#include "persistence.h"


typedef struct dllnode {
    int value;
    struct dllnode *next;
    struct dllnode *prev;
} DLLNode;

typedef struct DLL {
    int num_elements;
    DLLNode* head;
    DLLNode* tail;
} DLL;

PersistentDS *initialize_dll_with_element(int elemVal, int num_versions);
void print_dll(PersistentDS *input, int version_num);

void dll_add(PersistentDS *input, int elemVal, int srcVersion);
int dll_read(PersistentDS *input, int elemIndex, int srcVersion);
void dll_update(PersistentDS *input, int elemIndex, int elemVal, int srcVersion);
int dll_delete(PersistentDS *input, int elemIndexToDelete, int srcVersion);


#endif //PERSISTENCEFOREXISTENCE_DLL_H
