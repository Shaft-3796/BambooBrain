#pragma once

#include "../Model.h"
#include "PredictFromTree.h"

/**
 * @brief PredictFromModelMode the mode of the predict_from_model function
 * PREDICT_FROM_MODEL_MODE_RANDOM_FOREST_MAJORITY: predict from a random forest by majority
 */
typedef enum PredictFromModel {
    PREDICT_FROM_MODEL_MODE_RANDOM_FOREST_MAJORITY,
} PredictFromModelMode;

/**
 * @brief PredictFromModelArgs the arguments for the predict_from_model function
 * Common:
 * @param predict_from_tree_args the arguments for the predict_from_tree function
 * @param model the model
 * @param instance the instance
 * Specific:
 */
typedef struct PredictFromModelArgs {
    PredictFromModelMode mode;
    /* Common arguments */
    PredictFromTreeArgs *predict_from_tree_args;
    Model *model;
    Instance *instance;
    /* Specific arguments */
} PredictFromModelArgs;

/**
 * @brief Predict the class of an instance from a model
 * @param args the arguments for the predict_from_model function
 * @return the class id
 */
int predict_from_model(PredictFromModelArgs *args);
