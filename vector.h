#ifndef PERSISTENCEFOREXISTENCE_VECTOR_H
#define PERSISTENCEFOREXISTENCE_VECTOR_H

#include "persistence.h"

typedef struct vector {
    int num_elements;
    int last_index;
    void *elements_array;
} Vector;


void print_vector(PersistentDS *input, int version_num);

PersistentDS *initialize_vector_with_element(int element, int num_versions, int num_elements);
void vector_add(PersistentDS *input, int element, int srcVersion);
int vector_read(PersistentDS *input, int element_index, int srcVersion);
void vector_update(PersistentDS *input, int index_to_update, int updated_element, int srcVersion);
void vector_delete(PersistentDS *input, int index_to_delete, int srcVersion);



#endif //PERSISTENCEFOREXISTENCE_VECTOR_H
