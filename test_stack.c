#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "stack.h"


int main(void) {
    PersistentDS* persistentStack = initialize_stack_with_element(1,100);
    print_stack(persistentStack,0);
    stack_push(persistentStack,2,0);
    stack_push(persistentStack,3,1);
    stack_push(persistentStack,5,2);
    stack_push(persistentStack,7,3);
    print_stack(persistentStack,4);
    //Show the top of version 3
    int version,top;
    version = 3;
    top = stack_look(persistentStack,version);
    if(top!=INT_MIN){        printf("The Top of Version %d is %d\n",version,top);    }
    version = 2;
    top = stack_look(persistentStack,version);
    if(top!=INT_MIN){        printf("The Top of Version %d is %d\n",version,top);    }
    //Non Existent Version
    version = 7;
    top = stack_look(persistentStack,version);
    if(top!=INT_MIN){        printf("The Top of Version %d is %d\n",version,top);    }
    //Pop
    int popped;
    popped = stack_pop(persistentStack,4);
    if(popped!=INT_MIN){printf("The popped element is %d", popped);}
    print_stack(persistentStack,5);

    popped = stack_pop(persistentStack,5);
    if(popped!=INT_MIN){printf("The popped element is %d", popped);}
    print_stack(persistentStack,6);

    popped = stack_pop(persistentStack,6);
    if(popped!=INT_MIN){printf("The popped element is %d", popped);}
    print_stack(persistentStack,7);

    popped = stack_pop(persistentStack,7);
    if(popped!=INT_MIN){printf("The popped element is %d", popped);}
    print_stack(persistentStack,8);

    popped = stack_pop(persistentStack,8);
    if(popped!=INT_MIN){printf("The popped element is %d", popped);}
    print_stack(persistentStack,9);

    popped = stack_pop(persistentStack,9);
    if(popped!=INT_MIN){printf("The popped element is %d", popped);}
    print_stack(persistentStack,10);

    printf("\n");
    versionGraph* versionG = generate_version_graph(persistentStack);
    print_version_graph(versionG);
    versionIndex *parents_list =get_parents_list(persistentStack,9);
    print_parents_list(parents_list,9);
    print_instruction_list(parents_list,9,persistentStack);
}