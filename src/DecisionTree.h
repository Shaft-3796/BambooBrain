#pragma once

#include "Split.h"

typedef struct sDecisionTreeNode{
    struct sDecisionTreeNode* left;
    struct sDecisionTreeNode* right;
    Split split;
    int class_id;
} DecisionTreeNode;
