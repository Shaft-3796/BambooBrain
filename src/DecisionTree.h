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


#endif //DECISIONTREE_H
