#pragma once

#include "../Split.h"
#include "../DecisionTree.h"

/**
 * @brief PredictFromTreeMode the mode of the predict_from_tree function
 * PREDICT_FROM_TREE_MODE_THRESHOLD: predict the class of an instance based on a threshold
 */
typedef enum PredictFromTree {
    PREDICT_FROM_TREE_MODE_THRESHOLD,
} PredictFromTreeMode;

/**
 * @brief PredictFromTreeArgs the arguments for the predict_from_tree function
 */
typedef struct PredictFromTreeArgs {
} PredictFromTreeArgs;

/**
 * @brief PredictFromTreeConfig the configuration for the PredictFromTree function
 * @param mode the mode of the predict_from_tree function
 * @param predict_from_tree_function the function to call to predict the class of an instance
 */
typedef struct PredictFromTreeConfig {
    const PredictFromTreeMode mode;
    float (*predict_from_tree_function)(const struct PredictFromTreeConfig *config, const PredictFromTreeArgs *args, const DecisionTreeNode *tree, const Instance *instance);
} PredictFromTreeConfig;


/**
 * @brief predict_from_tree_and_threshold predicts the class of an instance based on a threshold (PREDICT_FROM_TREE_MODE_THRESHOLD)
 * @param config the configuration for the predict_from_tree function
 * @param args mode specific arguments for the predict_from_tree function
 * No arguments are expected.
 * @param tree the tree
 * @param instance the instance
 * @return the class id
 */
float predict_from_tree_and_threshold(const PredictFromTreeConfig *config, const PredictFromTreeArgs *args, const DecisionTreeNode *tree, const Instance *instance);