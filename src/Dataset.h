#ifndef DATASET_H
#define DATASET_H

#include <SDL.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define DATASETS_PATH "../datasets/"


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


/**
 * @brief Dataset_readFromFile reads a dataset from a file
 * @param filename the name of the file to read
 * @return a pointer to the dataset
 */
Dataset* Dataset_readFromFile(char* filename);

/**
 * @brief Dataset_destroy frees the memory allocated for the dataset
 * @param dataset the dataset to free
 */
void Dataset_destroy(Dataset *dataset);

Subproblem *Dataset_getSubproblem(Dataset *data);
Subproblem *Subproblem_create(int maximumCapacity, int featureCount, int classCount);
void Subproblem_destroy(Subproblem *subproblem);
void Subproblem_insert(Subproblem *subproblem, Instance *instance);
void Subproblem_print(Subproblem *subproblem);


#endif //DATASET_H
