#include "PredictFromTree.h"

/**
 * @brief predict_from_tree_and_threshold predicts the class of an instance
 * @param tree the decision tree
 * @param instance the instance
 * @return the class id
 */
int predict_from_tree_and_threshold(const DecisionTreeNode *tree, const Instance* instance) {
    if(tree->class_id != -1) return tree->class_id;  // Leaf node

    if(instance->values[tree->split.feature_id] <= tree->split.threshold)
        return predict_from_tree_and_threshold(tree->left, instance);

    return predict_from_tree_and_threshold(tree->right, instance);
}

/**
 * @brief predict_from_tree predicts the class of an instance based on a tree
 * @param args the arguments for the predict_from_tree function
 * @return the class id
 */
int predict_from_tree(const PredictFromTreeArgs *args){
    switch(args->mode) {
        case PREDICT_FROM_TREE_MODE_THRESHOLD:
            return predict_from_tree_and_threshold(args->tree, args->instance);
        default:
            printf("Error: unknown predict mode\n");
        return -1;
    }
}