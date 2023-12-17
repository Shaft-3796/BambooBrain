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
 * @param data the dataset to free
 */
void Dataset_destroy(Dataset *data);

/**
 * @brief Dataset_getSubproblem creates a subproblem from a dataset
 * @param data the dataset
 * @return a pointer to the subproblem
 */
Subproblem *Dataset_getSubproblem(Dataset *data);

/**
 * @brief Subproblem_create creates a subproblem from the given parameters
 * @param maximumCapacity the maximum capacity of the subproblem
 * @param featureCount the number of features
 * @param classCount the number of classes
 * @return a pointer to the subproblem
 */
Subproblem *Subproblem_create(int maximumCapacity, int featureCount, int classCount);

/**
 * @brief Subproblem_destroy frees the memory allocated for the subproblem
 * @param subproblem the subproblem to free
 */
void Subproblem_destroy(Subproblem *subproblem);

/**
 * @brief Subproblem_insert inserts an instance into the subproblem
 * @param subproblem the subproblem
 * @param instance the instance to insert
 */
void Subproblem_insert(Subproblem *subproblem, Instance *instance);

/**
 * @brief Subproblem_print prints the subproblem
 * @param subproblem the subproblem to print
 */
void Subproblem_print(Subproblem *subproblem);


#endif //DATASET_H
