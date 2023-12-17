#include "DecisionTree.h"

int get_main_class(Subproblem *sp) {
    int mainId = 0;
    for(int classId=1; classId<sp->classCount; ++classId) {
        if(sp->classes[classId].instanceCount > sp->classes[classId-1].instanceCount) {
            mainId = classId;
        }
    }
    return mainId;
}

// If the subproblem is pure enough, we return the main class id, else we return -1
int test_node_purity(Subproblem *sp, float prunningTreshold) {
    const int mainId = get_main_class(sp);
    return (float)sp->classes[mainId].instanceCount / (float)sp->instanceCount >= prunningTreshold ? mainId : -1;
}

/**
 * @brief DecisionTree_create creates a decision tree from a subproblem
 * @param sp the subproblem
 * @param currentDepth the current depth of the tree
 * @param maxDepth the maximum depth of the tree
 * @param prunningThreshold the prunning treshold
 * @return a pointer to the decision tree
 */
DecisionTreeNode* DecisionTree_create(Subproblem *sp, int currentDepth, int maxDepth, float prunningThreshold) {
    DecisionTreeNode *node = (DecisionTreeNode*)calloc(1, sizeof(DecisionTreeNode));

    // Max depth reached
    if(currentDepth >= maxDepth) { node->classID = get_main_class(sp); return node;}

    // Subproblem is pure enough
    node->classID = test_node_purity(sp, prunningThreshold);  // Set the classID to -1 allow to easily check if the node is not a leaf
    if(node->classID != -1) return node;

    // Split the subproblem
    Subproblem *spl = Subproblem_create(sp->instanceCount, sp->featureCount, sp->classCount);
    Subproblem *spr = Subproblem_create(sp->instanceCount, sp->featureCount, sp->classCount);

    node->split = Split_compute(sp);

    Split_subproblem(sp, node->split.featureID, node->split.threshold, spl, spr);

    node->left = DecisionTree_create(spl, currentDepth+1, maxDepth, prunningThreshold);
    node->right = DecisionTree_create(spr, currentDepth+1, maxDepth, prunningThreshold);

    Subproblem_destroy(spl);
    Subproblem_destroy(spr);

    return node;
}

/**
 * @brief Decision_nodeCount counts the number of nodes in a decision tree
 * @param node the node
 * @return the number of nodes
 */
int Decision_nodeCount(DecisionTreeNode* node) {
    if (!node) return 0;
    return 1 + Decision_nodeCount(node->left) + Decision_nodeCount(node->right);
}

/**
 * @brief DecisionTree_destroy destroys a decision tree
 * @param decisionTree the decision tree
 */
void DecisionTree_destroy(DecisionTreeNode *decisionTree) {
    if(!decisionTree) return;

    DecisionTree_destroy(decisionTree->left);
    DecisionTree_destroy(decisionTree->right);

    free(decisionTree);
}
