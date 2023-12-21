#pragma once

#include "../Dataset.h"
#include "../Model.h"
#include "PredictFromTree.h"


/**
 * @brief predict_from_random_forest_majority predicts the class of an instance based on a random forest by majority (PREDICT_FROM_MODEL_MODE_RANDOM_FOREST_MAJORITY)
 * @param config the configuration for the predict_from_model function
 * - predict_from_tree_config: the configuration for the predict_from_tree function
 * @param model the model
 * @param instance the instance
 * @return the predictions with only main_prediction or everything depending on the configuration
 */
Predictions *predict_from_random_forest_majority(const Config *config, const Model *model, const Instance *instance);

/**
* @brief predict_from_tree predicts the class of an instance based on a single tree (PREDICT_FROM_MODEL_MODE_SINGLE_TREE)
* @param config the configuration for the predict_from_model function
* - predict_from_tree_config: the configuration for the predict_from_tree function
* @param model the model
* @param instance the instance
* @return the predictions with only main_prediction or everything depending on the configuration
*/
Predictions *predict_from_model_single_tree(const Config *config, const Model *model, const Instance *instance);
