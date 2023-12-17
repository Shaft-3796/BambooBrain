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

/**
 * @brief DecisionTree_predict predicts the class of an instance
 * @param tree the decision tree
 * @param instance the instance
 * @return the class id
 */
int DecisionTree_predict(DecisionTreeNode *tree, Instance* instance) {
    if(tree->classID != -1) return tree->classID;  // Leaf node
    assert (tree->left && tree->right);  // Should always be true
    if(instance->values[tree->split.featureID] <= tree->split.threshold) return DecisionTree_predict(tree->left, instance);
    return DecisionTree_predict(tree->right, instance);
}

/**
 * @brief DecisionTree_predictAll predicts the class of all instances in a dataset
 * @param tree the decision tree
 * @param data the dataset
 * @return a list of predicted class ids
 */
int* DecisionTree_predictAll(DecisionTreeNode *tree, Dataset *data) {
    int *predictions = (int*)calloc(data->instanceCount, sizeof(int));
    for(int i=0; i<data->instanceCount; ++i) {
        predictions[i] = DecisionTree_predict(tree, &data->instances[i]);
    }
    return predictions;
}

/**
 * @brief DecisionTree_evaluate evaluates the accuracy of a decision tree on a dataset
 * @param tree the decision tree
 * @param data the dataset
 * @return the accuracy between 0 and 1
 */
float DecisionTree_evaluate(DecisionTreeNode *tree, Dataset *data) {
    int *predictions = DecisionTree_predictAll(tree, data);
    int correct = 0;
    for(int i=0; i<data->instanceCount; ++i) {
        if(predictions[i] == data->instances[i].classID) correct++;
    }
    free(predictions);
    return (float)correct / (float)data->instanceCount;
}

int* gen_random_integers(int from, int to, int count) {
    int *integers = (int*)calloc(count, sizeof(int));
    for(int i=0; i<count; ++i) integers[i] = rand() % (to-from) + from;
    return integers;
}

/**
 * @brief Dataset_bagging creates a subproblem from a dataset by randomly selecting instances
 * @param data the dataset
 * @param proportion the proportion of instances to select
 * @return a subproblem
 */
Subproblem *Dataset_bagging(Dataset *data, float proportion) {
    int *integers = gen_random_integers(0, data->instanceCount, (int)(data->instanceCount * proportion));
    Subproblem *sp = Subproblem_create((int)(data->instanceCount * proportion), data->featureCount, data->classCount);
    for(int i=0; i<(int)(data->instanceCount * proportion); ++i) Subproblem_insert(sp, &data->instances[integers[i]]);
    free(integers);
    return sp;
}