#ifndef DATASET_H
#define DATASET_H

#include <SDL.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

// --- Base Structures ---
typedef struct sInstance{
    int* values;
    int classID;
} Instance;

typedef struct sDataset{
    Instance *instances;
    int instanceCount;
    int featureCount;
    int classCount;
}   Dataset;

typedef struct sSubproblemClass{
    int instanceCount;
    Instance **instances;
} SubproblemClass;

typedef struct sSubproblem{
    Instance **instances;
    int instanceCount;
    int capacity;
    int featureCount;
    int classCount;
    SubproblemClass *classes;
} Subproblem;

// --- Base functions ---
Dataset* Dataset_readFromFile(char* filename);
void Dataset_destroy(Dataset *data);

Subproblem *Dataset_getSubproblem(Dataset *data);
Subproblem *Subproblem_create(int maximumCapacity, int featureCount, int classCount);
void Subproblem_destroy(Subproblem *subproblem);
void Subproblem_insert(Subproblem *subproblem, Instance *instance);
void Subproblem_print(Subproblem *subproblem);


#endif //DATASET_H
