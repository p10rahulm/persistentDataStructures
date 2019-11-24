#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "vector.h"
#include "persistence.h"


void main() {
    int num_versions = 100;
    int num_elements = 5;
    PersistentDS *persistentVector = initialize_vector_with_element(11, num_versions, num_elements);
    print_vector(persistentVector, 0);
    vector_add(persistentVector, 21, 0);
    print_vector(persistentVector, 1);
    vector_add(persistentVector, 31, 0);
    print_vector(persistentVector, 2);
    vector_add(persistentVector, 31, 1);
    print_vector(persistentVector, 3);
    printf("The third element of version number 3 is %d\n", vector_read(persistentVector, 2, 3));
    vector_add(persistentVector, 41, 3);
    print_vector(persistentVector, 4);
    vector_update(persistentVector,3,51,4);
    print_vector(persistentVector, 5);
    vector_delete(persistentVector,3,5);
    print_vector(persistentVector, 6);
    vector_delete(persistentVector,2,3);
    print_vector(persistentVector, 7);
    vector_delete(persistentVector,2,1);
    print_vector(persistentVector, 8);

    printf("\n");
    versionGraph* versionG = generate_version_graph(persistentVector);
    print_version_graph(versionG);
    versionIndex *parents_list =get_parents_list(persistentVector,7);
    print_parents_list(parents_list,7);
    print_instruction_list(parents_list,7,persistentVector);


}
