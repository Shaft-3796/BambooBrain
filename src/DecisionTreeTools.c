#include "DecisionTreeTools.h"


/**
 * @brief count_decision_tree_nodes counts the number of nodes in a decision tree
 * @param node the node
 * @return the number of nodes
 */
int count_decision_tree_nodes(const DecisionTreeNode* node) {
    if (!node) return 0;
    return 1 + count_decision_tree_nodes(node->left) + count_decision_tree_nodes(node->right);
}

/**
 * @brief predict_all_from_tree predicts the class of all instances in a dataset
 * @param args the arguments for the predict_from_tree function
 * @param tree the tree
 * @param data the dataset
 * @return a list of predicted class ids
 */
int* predict_all_from_tree(PredictFromTreeArgs *args, DecisionTreeNode *tree, Dataset *data) {
    args->tree = tree;

    int *predictions = (int*)calloc(data->instance_count, sizeof(int));

    for(int i=0; i<data->instance_count; ++i) {
        args->instance = &data->instances[i];
        predictions[i] = predict_from_tree(args);
    }
    return predictions;
}

/**
 * @brief evaluate_decision_tree evaluates the accuracy of a decision tree on a dataset
 * @param args the arguments for the predict_from_tree function
 * @param tree the tree
 * @param data the dataset
 * @return the accuracy between 0 and 1
 */
float evaluate_decision_tree(PredictFromTreeArgs *args, DecisionTreeNode *tree, Dataset *data) {
    int *predictions = predict_all_from_tree(args, tree, data);

    int correct = 0;
    for(int i=0; i<data->instance_count; ++i) {
        if(predictions[i] == data->instances[i].class_id) correct++;
    }

    free(predictions);
    return (float)correct / (float)data->instance_count;
}

/**
 * @brief destroy_decision_tree destroys a decision tree
 * @param decisionTree the decision tree
 */
void destroy_decision_tree(DecisionTreeNode *decisionTree) {
    if(!decisionTree) return;

    destroy_decision_tree(decisionTree->left);
    destroy_decision_tree(decisionTree->right);

    free(decisionTree);
}
