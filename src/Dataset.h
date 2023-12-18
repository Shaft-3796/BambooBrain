#pragma once

#include <SDL.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>


typedef struct sInstance{
    int* values;
    int class_id;
} Instance;

typedef struct sDataset{
    Instance *instances;
    int instance_count;
    int feature_count;
    int class_count;
}   Dataset;

typedef struct sSubproblemClass{
    int instance_count;
    Instance **instances;
} SubproblemClass;

typedef struct sSubproblem{
    Instance **instances;
    int instance_count;
    int capacity;
    int feature_count;
    int class_count;
    SubproblemClass *classes;
    int majority_class_id;
    int majority_class_instance_count;
} Subproblem;


/* --- Dataset parsing --- */
/**
 * @brief parse_dataset_from_file parses a dataset from a file
 * @param path the path to the file
 * @return a pointer to the dataset
 */
Dataset* parse_dataset_from_file(const char* path);

/**
 * @brief destroy_dataset frees the memory allocated for the dataset
 * @param data the dataset to free
 */
void destroy_dataset(Dataset *data);


/* --- Subproblem --- */
/**
 * @brief create_subproblem creates a subproblem from properties
 * @param max_capacity the maximum capacity of the subproblem
 * @param feature_count the number of features
 * @param class_count the number of classes
 */
Subproblem *create_subproblem(int max_capacity, int feature_count, int class_count);

/**
 * @brief insert_subproblem_instance inserts an instance into a subproblem
 * @param sp the subproblem
 * @param instance the instance to insert
 */
void insert_subproblem_instance(Subproblem *sp, Instance *instance);

/**
 * @brief free_subproblem_excess_memory frees the memory allocated for the subproblem that is not used, RAM optimization
 * @param sp the subproblem
 */
void free_subproblem_excess_memory(Subproblem *sp);

/**
 * @brief print_subproblem prints the subproblem
 * @param sp the subproblem to print
 */
void print_subproblem(Subproblem *sp);

/**
 * @brief destroy_subproblem frees the memory allocated for the subproblem
 * @param sp the subproblem to free
 */
void destroy_subproblem(Subproblem *sp);

/**
 * @brief create_subproblem_from_dataset creates a subproblem from a dataset
 * @param data the dataset
 * @return a pointer to the subproblem
 */
Subproblem *create_subproblem_from_dataset(Dataset *data);

