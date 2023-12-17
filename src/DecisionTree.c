//
// Created by loe on 14/12/23.
//

#include "DecisionTree.h"

int getMajClass(Subproblem* sp) {
    int maj = -1;
    int majID = -1;
    for (int i = 0; i < sp->classCount; ++i) {
        if (sp->classes[i].instanceCount > maj) {
            maj = sp->classes[i].instanceCount;
            majID = i;
        }
    }
    return majID;
}

float getPurity(Subproblem* sp, int majID) {
    if (!sp->instanceCount) return 0.0;

    return (float)sp->classes[majID].instanceCount / (float)sp->instanceCount;
}

DecisionTreeNode* DecisionTree_create(Subproblem* sp, int currentDepth, int maxDepth, float pruningThreshold) {
    DecisionTreeNode* node = (DecisionTreeNode*) calloc(1, sizeof(DecisionTreeNode));

    int majID = getMajClass(sp);
    if (getPurity(sp, majID) >= pruningThreshold || currentDepth >= maxDepth) {
        node->classID = majID;
        return node;
    }
    node->split = Split_compute(sp);

    Subproblem *spl = Subproblem_create(1, sp->featureCount, sp->classCount);
    Subproblem *spr = Subproblem_create(1, sp->featureCount, sp->classCount);

    // Split sp into 2 sub problems
    for (int i = 0; i < sp->instanceCount; ++i) {
        int value = sp->instances[i]->values[node->split.featureID];
        if ((float)value <= node->split.threshold) {
            Subproblem_insert(spl, sp->instances[i]);
        }
        else {
            Subproblem_insert(spr, sp->instances[i]);
        }
    }

    node->left = DecisionTree_create(spl, currentDepth+1, maxDepth, pruningThreshold);
    node->right = DecisionTree_create(spr, currentDepth+1, maxDepth, pruningThreshold);

    Subproblem_destroy(spl);
    Subproblem_destroy(spr);

    return node;
}

int Decision_nodeCount(DecisionTreeNode* node) {
    if (!node) return 0;

    return 1 + Decision_nodeCount(node->left) + Decision_nodeCount(node->right);
}

void DecisionTree_destroy(DecisionTreeNode* decisionTree) {
    if (!decisionTree) return;

    DecisionTree_destroy(decisionTree->left);
    DecisionTree_destroy(decisionTree->right);

    decisionTree->left = NULL;
    decisionTree->right = NULL;
    free(decisionTree);
}
