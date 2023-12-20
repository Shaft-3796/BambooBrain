#pragma once

#include "../Model.h"
#include "Bagging.h"
#include "CreateTree.h"
#include "../Storage.h"

/**
 * @brief CreateModelArgs the arguments for the create_model function
 */
typedef struct CreateModelArgs {
} CreateModelArgs;

/**
 * @brief CreateModelConfig the configuration for the CreateModel function
 * @param mode the mode of the create_model function
 * @param create_model_function the function to call to create the model
 * @param bagging_config the configuration for the bagging function
 * For MODEL_MODE_RANDOM_FOREST
 * @param create_tree_config the configuration for the create_tree function
 * For MODEL_MODE_RANDOM_FOREST
 * @param tree_count the number of trees in the model
 * For MODEL_MODE_RANDOM_FOREST
 */
typedef struct CreateModelConfig {
    const ModelMode mode;
    Model* (*create_model_function)(const struct CreateModelConfig *config, const CreateModelArgs *args, const Dataset *data);

    BaggingConfig *bagging_config;
    CreateTreeConfig *create_tree_config;
    int tree_count;
} CreateModelConfig;

/**
 * @brief create_random_forest creates a random forest model (MODEL_MODE_RANDOM_FOREST)
 * @param config the configuration for the create_model function
 * - bagging_config: the configuration for the bagging function
 * - create_tree_config: the configuration for the create_tree function
 * - tree_count: the number of trees in the forest
 * @param args mode specific arguments for the create_model function
 * No arguments are expected.
 * @param data the dataset
 * @return a pointer to the model
 */
Model *create_random_forest(const CreateModelConfig *config, const CreateModelArgs *args, const Dataset *data);

/**
 * @brief create_tree creates a simple decision tree model (MODEL_MODE_TREE)
 * @param config the configuration for the create_model function
 * - create_tree_config: the configuration for the create_tree function
 * @param args mode specific arguments for the create_model function
 * No arguments are expected.
 * @param data the dataset
 * @return a pointer to the model
 */
Model *create_tree(const CreateModelConfig *config, const CreateModelArgs *args, const Dataset *data);