//
// Created by loe on 14/12/23.
//

#ifndef DECISIONTREE_H
#define DECISIONTREE_H
#include "Split.h"

typedef struct sDecisionTreeNode{
    struct sDecisionTreeNode* left;
    struct sDecisionTreeNode* right;
    Split split;
    int classID;
} DecisionTreeNode;

DecisionTreeNode* DecisionTree_create(Subproblem* sp, int currentDepth, int maxDepth, float prunningThreshold);
void DecisionTree_destroy(DecisionTreeNode *decisionTree);
int Decision_nodeCount(DecisionTreeNode* node);

#endif //DECISIONTREE_H
