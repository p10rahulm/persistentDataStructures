#include <time.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "genomeApplication.h"

char random_codon() {
    int rd = rand();
    if (rd % 4 == 0) return 'A';
    if (rd % 4 == 1) return 'C';
    if (rd % 4 == 2) return 'G';
    if (rd % 4 == 3) return 'T';
}

char *generate_random_string_ofSize(int num_codons) {
    char *out = calloc(num_codons, sizeof(char));
    for (int i = 0; i < num_codons; ++i) {
        out[i] = random_codon();
    }
    return out;
}

GenomeTable *initializeGenomeTable(int num_common_sequences) {
    GenomeTable *sequences = calloc(1, sizeof(GenomeTable));
    sequences->num_snips = num_common_sequences;
    sequences->genomeSnips = calloc(num_common_sequences, sizeof(GenomeSnip));
    srand(time(0));
    for (int i = 0; i < num_common_sequences; ++i) {
        sequences->genomeSnips[i].code = i + 1;
        sequences->genomeSnips[i].GeneSequence = generate_random_string_ofSize(SIZE_OF_GENOME_SNIP);
    }
    return sequences;
}
void printGenomeTable(GenomeTable* input) {
    int num_sequences = input->num_snips;
    for (int i = 0; i < num_sequences; ++i) {
        printf("\n%d -> %s",input->genomeSnips[i].code,input->genomeSnips[i].GeneSequence);
    }
    printf("\n");
}

int codontoint(char codon){
    if(codon=='A') return 0;
    if(codon=='C') return 1;
    if(codon=='G') return 2;
    if(codon=='T') return 3;
    return -1;
}

int checkAlive(char* genomeSequence){
    int num_chars = sizeof(genomeSequence)/ sizeof(char);
    int hash;
    for (int i = 0; i < num_chars; ++i) {
        hash += codontoint(genomeSequence[i])%ALIVE_PRIME;
    }
    if(hash%ALIVE_PRIME==(ALIVE_PRIME-1)){        return 1;    }
    return 0;
}

char* generate_genome(PersistentDS* GenomeVersions,GenomeTable* programGenome,int version_num){
    if(!GenomeVersions || !programGenome){
        printf("Error. Check the input Genome Version and Program Genome are valid.\n");
        return NULL;
    }
    if(version_num>GenomeVersions->last_updated_version_number){
        printf("Error. Check version num\n");
        return NULL;
    }

    VersionNode* gene_version = &GenomeVersions->versions[version_num];
    DLL* genome_list = gene_version->structure_head;
    int num_in_list = genome_list->num_elements;

    char* out = calloc(num_in_list*SIZE_OF_GENOME_SNIP, sizeof(char));
    DLLNode* rover = genome_list->head;
    int char_index =0;
    while(rover){
        int index = rover->value-1;
        char* copy_from = programGenome->genomeSnips[index].GeneSequence;
        for (int i = 0; i < SIZE_OF_GENOME_SNIP; ++i) {
            out[char_index] = copy_from[i];
            char_index++;
        }
        rover = rover->next;
    }
    return out;
}

void printAlive(char* s){
    printf("The genome corresponding to %s IS ",s);
    if(!checkAlive(s)){        printf("NOT ");    }
    printf("ALIVE\n");
}

PersistentDS* generateNewVersion(PersistentDS* GenomeVersions,GenomeTable* programGenome){
    printf("The Genome Table is as follows\n");
    printGenomeTable(programGenome);
    int snipChoice =INT_MIN;
    while(snipChoice < 1 || snipChoice > programGenome->num_snips){
        if(snipChoice!=INT_MIN){
            printf("Please check the entered Choice. You can choose between 1 and %d", programGenome->num_snips);
        }
        printf("Which Snip would you like to initialize with?\n");
        scanf("%d",&snipChoice);
        fflush(stdin);
    }
    printf("The Choice you made was %d\n",snipChoice);
    if(!GenomeVersions){
        GenomeVersions = initialize_dll_with_element(snipChoice,100);
    } else {
        initialize_latest_version_of_dll_with_element(GenomeVersions,snipChoice);
    }
    printf("The version you initialized was as follows\n");
    print_dll(GenomeVersions, GenomeVersions->last_updated_version_number);
    printf("The Genome Snip you chose is %s\n",programGenome->genomeSnips[snipChoice].GeneSequence);
    return GenomeVersions;
}

void viewGenomeVersions(PersistentDS* GenomeVersions,GenomeTable* programGenome){
    versionGraph* versionG = generate_version_graph(GenomeVersions);
    printf("The versions generated so far are as follows\n");
    print_version_graph(versionG);
    int versionChoice =INT_MIN;
    while(versionChoice!=-1){
        versionChoice =INT_MIN;
        while(versionChoice < -1 || versionChoice > GenomeVersions->last_updated_version_number){
            if(versionChoice!=INT_MIN){
                printf("\nPlease check the entered Choice. You can choose between 0 and %d for any version, or '-1' to exit\n", GenomeVersions->last_updated_version_number);
            }
            printf("\nWhich Version would you like to see details for? (Press -1 to exit)\n");
            scanf("%d",&versionChoice);
            fflush(stdin);
        }
        if(versionChoice==-1){ continue;}
        printf("The version you entered is %d.",versionChoice);
        versionIndex *parents_list =get_parents_list(GenomeVersions,versionChoice);
        print_parents_list(parents_list,versionChoice);
        print_instruction_list(parents_list,versionChoice,GenomeVersions);
        char* chosen_genome = generate_genome(GenomeVersions,programGenome,versionChoice);
        printf("The genome for this version is %s\n", chosen_genome);
        printAlive(chosen_genome);


    }

}


int modifyGenomeVersion(PersistentDS* GenomeVersions,GenomeTable* programGenome, int version){

    printf("\nHow would you like to modify this genome?"
           "\n0. Press 0 to add a gene snippet"
           "\n1. Press 1 to delete a gene snippet"
           "\n2. Press 2 to update a gene snippet"
           "\n-1.Press '-1' to exit\n");
    int change_choice;
    scanf("%d",&change_choice);
    fflush(stdin);
    if(change_choice<-1||change_choice>2){
        printf("\nThe change choice you entered was invalid. Please try again\n");
        return 0;
    }
    if(change_choice==-1){ return -1;}
    if(change_choice==0){
        int snipChoice =INT_MIN;
        while(snipChoice < 1 || snipChoice > programGenome->num_snips){
            if(snipChoice!=INT_MIN){
                printf("Please check the entered Choice. You can choose between 1 and %d.\n", programGenome->num_snips);
            }
            printf("Which Snip would you like to add?\n");
            scanf("%d",&snipChoice);
            fflush(stdin);
        }
        printf("The Snippet you chose to add was %d\n",snipChoice);
        dll_add(GenomeVersions,snipChoice,version);
        return 1;
    }
    if(change_choice==1){
        int snipIndex =INT_MIN;
        DLL* dll = GenomeVersions->versions[version].structure_head;
        int num_elements = dll->num_elements;
        while(snipIndex < 0 || snipIndex >= num_elements){
            if(snipIndex!=INT_MIN){
                printf("Please check the entered index. You can choose between 0 and %d\n", num_elements-1);
            }
            printf("Which Snip Index would you like to delete?\n");
            scanf("%d",&snipIndex);
            fflush(stdin);
        }
        printf("The Snippet you chose to delete was %d\n",snipIndex);
        int delete_success = dll_delete(GenomeVersions, snipIndex, version);
        if(delete_success==INT_MIN){
            printf("The snippet index you chose to delete was not part of the version. Please try again\n");
            return 0;
        }
        printf("The element you chose %d, was successfully deleted\n",delete_success);
        return 1;
    }
    if(change_choice==2){
        int snipIndex =INT_MIN;
        DLL* dll = GenomeVersions->versions[version].structure_head;
        int num_elements = dll->num_elements;
        while(snipIndex < 0 || snipIndex >= num_elements){
            if(snipIndex!=INT_MIN){
                printf("Please check the entered index. You can choose between 0 and %d\n", num_elements-1);
            }
            printf("Which Snip Index would you like to modify?\n");
            scanf("%d",&snipIndex);
            fflush(stdin);
        }
        printf("The Snippet you chose to modify was %d\n",snipIndex);

        int snipChoice =INT_MIN;
        while(snipChoice < 1 || snipChoice > programGenome->num_snips){
            if(snipChoice!=INT_MIN){
                printf("Please check the entered Choice. You can choose between 1 and %d.\n", programGenome->num_snips);
            }
            printf("What Snip would you like to update Snip Index: %d to?\n",snipIndex);
            scanf("%d",&snipChoice);
            fflush(stdin);
        }
        printf("You chose to update Snip Index: %d to %s\n",snipIndex,programGenome->genomeSnips[snipChoice-1].GeneSequence);


        dll_update(GenomeVersions, snipIndex, snipChoice, version);
        return 1;
    }

}

void modifyGenome(PersistentDS* GenomeVersions,GenomeTable* programGenome){
    versionGraph* versionG = generate_version_graph(GenomeVersions);
    printf("The versions generated so far are as follows\n");
    print_version_graph(versionG);
    int versionChoice =INT_MIN;
    while(versionChoice!=-1){
        versionChoice =INT_MIN;
        while(versionChoice < -1 || versionChoice > GenomeVersions->last_updated_version_number){
            if(versionChoice!=INT_MIN){
                printf("\nPlease check the entered Choice. You can choose between 0 and %d for any version, or '-1' to exit\n", GenomeVersions->last_updated_version_number);
            }
            printf("\nWhich Version do you want to modify? (Press -1 to exit)\n");
            scanf("%d",&versionChoice);
            fflush(stdin);
        }
        if(versionChoice==-1){ continue;}
        printf("The version you entered is %d\n",versionChoice);
        versionIndex *parents_list =get_parents_list(GenomeVersions,versionChoice);
        print_parents_list(parents_list,versionChoice);
        print_instruction_list(parents_list,versionChoice,GenomeVersions);
        char* chosen_genome = generate_genome(GenomeVersions,programGenome,versionChoice);
        printf("The genome for this version is %s\n", chosen_genome);
        //        printAlive(chosen_genome);
        int success_in_modify = modifyGenomeVersion(GenomeVersions,programGenome, versionChoice);
        if(success_in_modify==-1){
            versionChoice=-1;
            continue;
        }
        if(success_in_modify==0){
            printf("No Modifications made.\n");
            continue;
        };
        if(success_in_modify==1){
            int last_version = GenomeVersions->last_updated_version_number;
            print_dll(GenomeVersions,last_version);
            versionIndex *parents_list =get_parents_list(GenomeVersions,last_version);
            print_parents_list(parents_list,last_version);
            print_instruction_list(parents_list,last_version,GenomeVersions);
            char* chosen_genome = generate_genome(GenomeVersions,programGenome,last_version);
            printf("The genome for this version is %s\n", chosen_genome);
        };



    }


}

void viewAlive(PersistentDS* GenomeVersions,GenomeTable* programGenome){
    int num_versions = GenomeVersions->last_updated_version_number+1;
    for (int version = 0; version < num_versions; ++version) {
        printf("\nVersion %d:",version+1);
        char* genome = generate_genome(GenomeVersions,programGenome,version);
        printf("The genome for this version is %s\n", genome);
        printAlive(genome);
        printf("\n");
    }
}


