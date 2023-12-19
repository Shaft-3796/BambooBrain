#pragma once

#include "DecisionTree.h"
#include "dynamic/ComputeSplit.h"
#include "dynamic/PredictFromTree.h"


/**
 * @brief count_decision_tree_nodes counts the number of nodes in a decision tree
 * @param node the node
 * @return the number of nodes
 */
int count_decision_tree_nodes(const DecisionTreeNode* node);

/**
 * @brief predict_all_from_tree predicts the class of all instances in a dataset
 * @param predict_from_tree_config the configuration for the predict_from_tree function
 * @param tree the tree
 * @param data the dataset
 * @return a list of predicted class ids
 */
int* predict_all_from_tree(PredictFromTreeConfig *predict_from_tree_config, DecisionTreeNode *tree, Dataset *data);

/**
 * @brief evaluate_decision_tree evaluates the accuracy of a decision tree on a dataset
 * @param predict_from_tree_config the configuration for the predict_from_tree function
 * @param tree the tree
 * @param data the dataset
 * @return the accuracy between 0 and 1
 */
float evaluate_decision_tree(PredictFromTreeConfig *predict_from_tree_config, DecisionTreeNode *tree, Dataset *data);

/**
 * @brief destroy_decision_tree destroys a decision tree
 * @param decisionTree the decision tree
 */
void destroy_decision_tree(DecisionTreeNode *decisionTree);
