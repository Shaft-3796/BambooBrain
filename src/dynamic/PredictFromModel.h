#pragma once

#include "../Model.h"
#include "PredictFromTree.h"

/**
 * @brief PredictFromModelMode the mode of the predict_from_model function
 * PREDICT_FROM_MODEL_MODE_RANDOM_FOREST_MAJORITY: predict from a random forest by majority
 * PREDICT_FROM_MODEL_MODE_TREE: predict from a tree
 */
typedef enum PredictFromModel {
    PREDICT_FROM_MODEL_MODE_RANDOM_FOREST_MAJORITY,
    PREDICT_FROM_MODEL_MODE_SINGLE_TREE,
} PredictFromModelMode;

/**
 * @brief PredictFromModelArgs the arguments for the predict_from_model function
 */
typedef struct PredictFromModelArgs {
} PredictFromModelArgs;

/**
 * @brief PredictFromModelConfig the configuration for the PredictFromModel function
 * @param mode the mode of the predict_from_model function
 * @param predict_from_model_function the function to call to predict from a model
 * @param predict_from_tree_config the configuration for the predict_from_tree function
 * For PREDICT_FROM_MODEL_MODE_RANDOM_FOREST_MAJORITY
 * For PREDICT_FROM_MODEL_MODE_SINGLE_TREE
 */
typedef struct PredictFromModelConfig {
    const PredictFromModelMode mode;
    float (*predict_from_model_function)(const struct PredictFromModelConfig *config, const PredictFromModelArgs *args, const Model *model, const Instance *instance);

    PredictFromTreeConfig *predict_from_tree_config;
} PredictFromModelConfig;

/**
 * @brief predict_from_random_forest_majority predicts the class of an instance based on a random forest by majority (PREDICT_FROM_MODEL_MODE_RANDOM_FOREST_MAJORITY)
 * @param config the configuration for the predict_from_model function
 * - predict_from_tree_config: the configuration for the predict_from_tree function
 * @param args mdoe specific arguments for the predict_from_model function
 * No arguments are expected.
 * @param model the model
 * @param instance the instance
 * @return the class id
 */
float predict_from_random_forest_majority(const PredictFromModelConfig *config, const PredictFromModelArgs *args, const Model *model, const Instance *instance);

/**
* @brief predict_from_tree predicts the class of an instance based on a single tree (PREDICT_FROM_MODEL_MODE_SINGLE_TREE)
* @param config the configuration for the predict_from_model function
* - predict_from_tree_config: the configuration for the predict_from_tree function
* @param args mdoe specific arguments for the predict_from_model function
* No arguments are expected.
* @param model the model
* @param instance the instance
* @return the class id
*/
float predict_from_model_single_tree(const PredictFromModelConfig *config, const PredictFromModelArgs *args, const Model *model, const Instance *instance);

/**
 * @brief predict_from_model predicts the class of an instance based on a model
 * @param config the configuration for the predict_from_model function
 * @param args mdoe specific arguments for the predict_from_model function
 * @param model the model
 * @param instance the instance
 * @return the class id
 */
inline float predict_from_model(const PredictFromModelConfig *config, const PredictFromModelArgs *args, const Model *model, const Instance *instance) {
    return config->predict_from_model_function(config, args, model, instance);
}