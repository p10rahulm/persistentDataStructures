#include <time.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "genomeApplication.h"


/*
 *      ---------------------------------------------------------------------------------------------------------------
 *      GENOME APPLICATION: USING PERSISTENT DATA STRUCTURES TO CREATE GENOME VERSIONING
 *      ---------------------------------------------------------------------------------------------------------------
 *      1.  All Life forms on earth are made of Genomes that can be encoded as ACGT
 *      2.  Each of the A, C, G and T correspond to a particular chemical compound.
 *      3.  These compounds can polymerize into long chains, thus encoding information
 *      4.  While we may think that these polymers can take any form, these are limited by the Central Dogma.
 *      5.  The Central Dogma of molecular biology claims that all information in genes becomes living molecules
 *          through two processes - transcription and translation.
 *          This information flow can be understood as:
 *          DNA -> RNA -> Protein
 *          Where DNA -> RNA happens through transcription and RNA -> Protein happens through Translation
 *      6.  All Proteins are made from 21 Amino Acids. Each of these amino acids has a corresponding genetic code.
 *      7.  A string of amino acids can sometimes fold into something useful. Some of these become the molecules of life
 *      8.  Evolution over billions of generations has selected for sets of useful life molecules.
 *      9.  Since there is a one-one map between bio-molecules and genome, we can theoretically construct genomes that
 *          produce useful bio molecules. There exists some such least set of bio-molecules such that the process of
 *          the construction and propagation is self sustained. This is the minimum set of molecules required for life.
 *          The minimal genome required for such a theoretical minimum set of bio-molecules, may be called a
 *          minimal genome.
 *      10. Minimal Genome can be constructed by experiment.
 *              a.  Check for common subsequences present in ALL life. This is our minimal subsequence.
 *              b.  Construct a set of useful subsequences.
 *              c.  Dynamically add these common subsequences, not present in all life, and check for living property.
 *
 *
 *
 *
 *
 */


void main() {
    printf("Hello World!");
    int num_snips = 30;
    GenomeTable* gt =initializeGenomeTable(num_snips);
    printGenomeTable(gt);
    for (int i = 0; i < num_snips; ++i) {
        printf("%d. ",i);
        printAlive(gt->genomeSnips[i].GeneSequence);
    }
    interface();


}

void interface(){
    int programSwitch = 1;
    GenomeTable* programGenome =NULL;
    PersistentDS* GenomeVersions =NULL;
    while (programSwitch){
        printf("\n-------------------------------------------------------------------------------------------------------"
              "\nWELCOME TO THE PROGRAM FOR MANAGING YOUR MINIMUM GENOME SEQUENCES. TRY CREATING AN ORGANISM THAT'S ALIVE!"
              "\n-------------------------------------------------------------------------------------------------------"
              "\n"
              "\nSo what would you like to do today?"
              "\n 1. Generate Genome Table"
              "\n 2. Create a new version"
              "\n 3. Modify an old version"
              "\n 4. View Changelog of all versions"
              "\n 5. Check Alive (experimental!)"
              "\n 9. Exit"
              "\n");
        int userChoice,c;
        c = getchar();
        fflush(stdin);
        //while(getchar() != '\n' && c != EOF);//Flush Buffers
        userChoice = c -'0';
        if(userChoice==9){
            programSwitch=0;
            printf("Thank you for using Persistent Genome Sequence Analyzer!\n Have a great day!\n");
            continue;
        }
        if(userChoice<1||userChoice>5){printf("Please input a choice between 1 and 5.\n");continue;}
        if(userChoice!=1 && !programGenome){printf("Please First Generate a Genome Table (by choosing option 1)\n");continue;}
        if(userChoice==1 && programGenome){printf("You have already generated a Genome Table!\n");continue;}
        if(userChoice==1 && !programGenome){
            programGenome = initializeGenomeTable(30);
            printf("\n-------------------------------------------------------------------------------------------------"
                   "\nWe have analyzed 1 million species of organisms. The Program genome successfully generated. "
                   "\nThe common snippets generated after parsing through 1 million species are as below"
                   "\n-------------------------------------------------------------------------------------------------");
            printGenomeTable(programGenome);
            continue;
        }

        if(programGenome && !GenomeVersions && userChoice!=2){
            printf("Please Generate a new version of genome (by choosing option 2) first\n");continue;
        }
        if(programGenome && userChoice==2){
            GenomeVersions = generateNewVersion(GenomeVersions,programGenome);
            continue;
        }

        if(programGenome && GenomeVersions && userChoice==3){
            modifyGenome(GenomeVersions,programGenome);
            continue;
        }

        if(programGenome && GenomeVersions && userChoice==4){
            viewGenomeVersions(GenomeVersions,programGenome);
            continue;
        }
        if(programGenome && GenomeVersions && userChoice==5){
            viewAlive(GenomeVersions,programGenome);
            continue;
        }





    }
}
