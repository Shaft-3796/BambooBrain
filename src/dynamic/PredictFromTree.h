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
 * Common:
 * @param tree the tree
 * @param instance the instance
 * Specific:
 */
typedef struct PredictFromTreeArgs {
    PredictFromTreeMode mode;
    /* Common arguments */
    DecisionTreeNode *tree;
    Instance *instance;
    /* Specific arguments */
} PredictFromTreeArgs;

/**
 * @brief predict_from_tree predicts the class of an instance based on a tree
 * @param args the arguments for the predict_from_tree function
 * @return the class id
 */
int predict_from_tree(const PredictFromTreeArgs *args);
