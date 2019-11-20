#include <stdio.h>
#include <stdlib.h>
#include "ll.h"


int main(void) {
    PersistentDS* persistentLL = initialize_sll_with_element(11,100);
    print_sll(persistentLL,0);
}