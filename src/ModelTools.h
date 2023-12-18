#pragma once

#include "Model.h"
#include "DecisionTreeTools.h"
#include "dynamic/PredictFromModel.h"


/* @brief count nodes in a model
 * @param model the model
 * @return the number of nodes
 */
int count_model_nodes(const Model* model);

/**
 * @brief predict_all_from_model predicts the class of all instances in a dataset
 * @param args the arguments for the predict_from_model function
 * @param model the model
 * @param data the dataset
 * @return a list of predicted class ids
 */
int* predict_all_from_model(PredictFromModelArgs *args, Model *model, Dataset *data);

/**
 * @brief evaluate_model evaluates the accuracy of a model on a dataset
 * @param args the arguments for the predict_from_model function
 * @param model the model
 * @param data the dataset
 * @return the accuracy between 0 and 1
 */
float evaluate_model(PredictFromModelArgs *args, Model *model, Dataset *data);

/**
 * @brief destroy_model destroys a model
 * @param model the model
 */
void destroy_model(Model *model);
