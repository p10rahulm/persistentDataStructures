#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "persistence.h"


void main() {
    int num_versions = 100;
    int num_elements = 5;
    PersistentDS *vector = initialize_with_element(11, num_versions, num_elements);
    print_vector(vector, 0);
    vector_add(vector, 21, 0);
    print_vector(vector, 1);
    vector_add(vector, 31, 0);
    print_vector(vector, 2);
    vector_add(vector, 31, 1);
    print_vector(vector, 3);
    printf("The third element of version number 3 is %d\n", vector_read(vector, 2, 3));
    vector_add(vector, 41, 3);
    print_vector(vector, 4);
    vector_update(vector,3,51,4);
    print_vector(vector, 5);
    vector_delete(vector,3,5);
    print_vector(vector, 6);
    vector_delete(vector,2,3);
    print_vector(vector, 7);
    vector_delete(vector,2,1);
    print_vector(vector, 8);

}
