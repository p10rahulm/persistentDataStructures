#ifndef PERSISTENCEFOREXISTENCE_GENOMEAPPLICATION_H
#define PERSISTENCEFOREXISTENCE_GENOMEAPPLICATION_H

#include "persistence.h"
#include "dll.h"

#define SIZE_OF_GENOME_SNIP 25
#define ALIVE_PRIME 17



typedef struct common_genomic_string{
    int code;
    char* GeneSequence;
} GenomeSnip;

typedef struct genome_table{
    int num_snips;
    GenomeSnip* genomeSnips;
} GenomeTable;

void printGenomeTable(GenomeTable* input);
GenomeTable *initializeGenomeTable(int num_common_sequences);
int checkAlive(char* genomeSequence);


void interface();
PersistentDS* generateNewVersion(PersistentDS* GenomeVersions,GenomeTable* programGenome);
char* generate_genome(PersistentDS* GenomeVersions,GenomeTable* programGenome,int version_num);
void printAlive(char* s);
void viewAlive(PersistentDS* GenomeVersions,GenomeTable* programGenome);
void viewGenomeVersions(PersistentDS* GenomeVersions,GenomeTable* programGenome);
void modifyGenome(PersistentDS* GenomeVersions,GenomeTable* programGenome);
int modifyGenomeVersion(PersistentDS* GenomeVersions,GenomeTable* programGenome, int version);

#endif //PERSISTENCEFOREXISTENCE_GENOMEAPPLICATION_H
