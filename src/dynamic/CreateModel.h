#pragma once

#include "../Model.h"
#include "Bagging.h"
#include "CreateTree.h"

/**
 * @brief CreateModelArgs the arguments for the create_model function
 * Common:
 * @param data the dataset
 * Specific:
 * @param bagging_args used for MODEL_MODE_RANDOM_FOREST
 * @param create_tree_args used for MODEL_MODE_RANDOM_FOREST
 * @param tree_count used for MODEL_MODE_RANDOM_FOREST
 */
typedef struct CreateModelArgs {
    ModelMode mode;
    /* Common arguments */
    Dataset *data;
    /* Specific arguments */
    BaggingArgs *bagging_args;
    CreateTreeArgs *create_tree_args;
    int tree_count;

} CreateModelArgs;

/**
 * @brief create_model creates a model
 * @param args the arguments for the model_create function
 * @return a pointer to the model
 */
Model *create_model(CreateModelArgs *args);
