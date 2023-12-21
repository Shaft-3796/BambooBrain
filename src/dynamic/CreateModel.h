#pragma once

#include "../Model.h"
#include "Bagging.h"
#include "CreateTree.h"
#include "../Storage.h"


/**
 * @brief create_random_forest creates a random forest model (MODEL_MODE_RANDOM_FOREST)
 * @param config the configuration for the create_model function
 * - bagging_config: the configuration for the bagging function
 * - create_tree_config: the configuration for the create_tree function
 * - tree_count: the number of trees in the forest
 * @param data the dataset
 * @return a pointer to the model
 */
Model *create_random_forest(const Config *config, const Dataset *data);

/**
 * @brief create_tree creates a simple decision tree model (MODEL_MODE_TREE)
 * @param config the configuration for the create_model function
 * - create_tree_config: the configuration for the create_tree function
 * @param data the dataset
 * @return a pointer to the model
 */
Model *create_tree(const Config *config, const Dataset *data);