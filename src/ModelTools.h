#pragma once

#include "Model.h"
#include "Predictions.h"
#include "DecisionTreeTools.h"
#include "dynamic/CreateModel.h"
#include "dynamic/PredictFromModel.h"


/**
 * @brief Load a model, create it if persistence is not enabled or if the file does not exist.
 * Save it to the file if persistence is enabled.
 * @param config the configuration
 * @param train_data_path the path to the training data
 * @param model_path the path to enable persistence, NULL to disable persistence,
 * if the file does not exist, the model will be created and saved to the file
 */
Model *load_model(const Config *config, const char *train_data_path, const char *model_path);


/* @brief count nodes in a model
 * @param model the model
 * @return the number of nodes
 */
int count_model_nodes(const Model* model);

/**
 * @brief predict_all_from_model predicts the class of all instances in a dataset
 * @param config the configuration
 * @param model the model
 * @param data the dataset
 * @return a list of predictions
 */
Predictions **predict_all_from_model(const Config *config, const Model *model, const Dataset *data);

/**
 * @brief evaluate_model evaluates the accuracy of a model on a dataset
 * @param config the configuration
 * @param model the model
 * @param data the dataset
 * @return the accuracy between 0 and 1
 */
float evaluate_model(const Config *config, const Model *model, const Dataset *data);

/**
 * @brief destroy_model destroys a model
 * @param model the model
 */
void destroy_model(Model *model);
