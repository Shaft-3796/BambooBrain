#pragma once

#include "../Config.h"
#include "../Dataset.h"
#include "../DecisionTree.h"


/**
 * @brief predict_from_tree_and_threshold predicts the class of an instance based on a threshold (PREDICT_FROM_TREE_MODE_THRESHOLD)
 * @param config the configuration for the predict_from_tree function
 * @param tree the tree
 * @param instance the instance
 * @return the class id
 */
int predict_from_tree_and_threshold(const Config *config, const DecisionTreeNode *tree, const Instance *instance);


/**
 * @brief predict_from_tree_and_sigmoid_scor predicts the class of an instance based on a sigmoid score (PREDICT_FROM_TREE_MODE_SIGMOID_SCORE)
 * @param config the configuration for the predict_from_tree function
 * - sigmoid_lambda the lambda parameter for the sigmoid function
 * @param tree the tree
 * @param instance the instance
 * @return the class id
 */
int predict_from_tree_and_sigmoid_score(const Config *config, const DecisionTreeNode *tree, const Instance *instance);