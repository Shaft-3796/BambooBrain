#ifndef DECISIONTREE_H
#define DECISIONTREE_H
#include "Split.h"

typedef struct sDecisionTreeNode{
    struct sDecisionTreeNode* left;
    struct sDecisionTreeNode* right;
    Split split;
    int classID;
} DecisionTreeNode;

/**
 * @brief DecisionTree_create creates a decision tree from a subproblem
 * @param sp the subproblem
 * @param currentDepth the current depth of the tree
 * @param maxDepth the maximum depth of the tree
 * @param prunningThreshold the prunning treshold
 * @return a pointer to the decision tree
 */
DecisionTreeNode* DecisionTree_create(Subproblem* sp, int currentDepth, int maxDepth, float prunningThreshold);

/**
 * @brief Decision_nodeCount counts the number of nodes in a decision tree
 * @param node the node
 * @return the number of nodes
 */
int Decision_nodeCount(DecisionTreeNode* node);

/**
 * @brief DecisionTree_destroy destroys a decision tree
 * @param decisionTree the decision tree
 */
void DecisionTree_destroy(DecisionTreeNode *decisionTree);

/**
 * @brief DecisionTree_predict predicts the class of an instance
 * @param tree the decision tree
 * @param instance the instance
 * @return the class id
 */
int DecisionTree_predict(DecisionTreeNode *tree, Instance* instance);

/**
 * @brief DecisionTree_predictAll predicts the class of all instances in a dataset
 * @param tree the decision tree
 * @param data the dataset
 * @return a list of predicted class ids
 */
int* DecisionTree_predictAll(DecisionTreeNode *tree, Dataset *data);

/**
 * @brief DecisionTree_evaluate evaluates the accuracy of a decision tree on a dataset
 * @param tree the decision tree
 * @param data the dataset
 * @return the accuracy between 0 and 1
 */
float DecisionTree_evaluate(DecisionTreeNode *tree, Dataset *data);

/**
 * @brief Dataset_bagging creates a subproblem from a dataset by randomly selecting instances
 * @param data the dataset
 * @param proportion the proportion of instances to select
 * @return a subproblem
 */
Subproblem *Dataset_bagging(Dataset *data, float proportion);

#endif //DECISIONTREE_H
